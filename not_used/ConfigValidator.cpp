#include "ConfigValidator.h"

#include <iostream>

#include "Config.h"

bool ConfigValidator::Validate(const ConfigData &config) {
  try {
    // Add more validations as needed
    return true;
  } catch (const std::exception &e) {
    std::cerr << "Validation failed: " << e.what() << std::endl;
    return false;
  }
}

bool ConfigValidator::ValidateAgainstSchema(const ConfigData &config,
                                            const YAML::Node &schema) {
  // Implement dynamic validation logic here (e.g., using Lua or schema-based
  // validation)
  return true;  // Placeholder
}

// Recursive function to convert YAML node to Lua table using Sol2
sol::table ConfigValidator::PushYamlToLua(sol::state &lua,
                                          const YAML::Node &node) {
  sol::table lua_table = lua.create_table();

  for (auto it = node.begin(); it != node.end(); ++it) {
    std::string key;
    try {
      key = it->first.as<std::string>();
    } catch (const YAML::BadConversion &e) {
      // Handle invalid key conversion
      key = "invalid_key";  // Or handle appropriately
    }

    const YAML::Node &value = it->second;

    try {
      if (value.IsScalar()) {
        if (value.Tag() == "tag:yaml.org,2002:int") {
          lua_table[key] = value.as<int>();
        } else if (value.Tag() == "tag:yaml.org,2002:float") {
          lua_table[key] = value.as<double>();
        } else if (value.Tag() == "tag:yaml.org,2002:bool") {
          lua_table[key] = value.as<bool>();
        } else if (value.IsNull()) {
          lua_table[key] = sol::lua_nil;
        } else {
          lua_table[key] = value.as<std::string>();
        }
      } else if (value.IsMap()) {
        lua_table[key] = PushYamlToLua(lua, value);
      } else if (value.IsSequence()) {
        sol::table array_table = lua.create_table();
        int index = 1;
        for (const auto &element : value) {
          if (element.IsScalar()) {
            if (element.Tag() == "tag:yaml.org,2002:int") {
              array_table[index++] = element.as<int>();
            } else if (element.Tag() == "tag:yaml.org,2002:float") {
              array_table[index++] = element.as<double>();
            } else if (element.Tag() == "tag:yaml.org,2002:bool") {
              array_table[index++] = element.as<bool>();
            } else if (element.IsNull()) {
              array_table[index++] = sol::lua_nil;
            } else {
              array_table[index++] = element.as<std::string>();
            }
          } else {
            array_table[index++] = PushYamlToLua(lua, element);
          }
        }
        lua_table[key] = array_table;
      }
    } catch (const YAML::BadConversion &e) {
      // Handle conversion error, possibly logging and setting Lua to nil or a
      // default value
      lua_table[key] = sol::lua_nil;
    }
  }

  return lua_table;
}

// Load YAML configuration into Lua
void ConfigValidator::LoadConfigToLua(sol::state &lua,
                                      const YAML::Node &config) {
  sol::table lua_config = PushYamlToLua(lua, config);
  lua["config"] = lua_config;
  // Debugging: Print Lua table contents
  std::cout << "Lua 'config' table contents:\n";
  for (const auto &pair : lua_config) {
    std::string key = pair.first.as<std::string>();
    sol::object value = pair.second;
    std::cout << key << " = ";
    switch (value.get_type()) {
      case sol::type::lua_nil:
        std::cout << "nil";
        break;
      case sol::type::boolean:
        std::cout << (value.as<bool>() ? "true" : "false");
        break;
      case sol::type::number:
        std::cout << value.as<double>();
        break;
      case sol::type::string:
        std::cout << "\"" << value.as<std::string>() << "\"";
        break;
      case sol::type::table:
        std::cout << "table";
        break;
      default:
        std::cout << "other";
        break;
    }
    std::cout << "\n";
  }
}

// Example validation function using Lua for dynamic rules
bool ConfigValidator::ValidateAgainstLua(const ConfigData &config,
                                         const YAML::Node &schema,
                                         sol::state &lua) {
  LoadConfigToLua(lua, schema);

  // You can define or load Lua conditions here based on the schema
  std::string condition = R"(
        if physics.gravity > 9.8 and simulation.duration > 1000 then
            return false
        else
            return true
        end
    )";

  sol::protected_function_result result =
      lua.safe_script(condition, sol::script_pass_on_error);
  if (!result.valid()) {
    sol::error err = result;
    std::cerr << "Lua validation error: " << err.what() << std::endl;
    return false;
  }

  return result;
}

