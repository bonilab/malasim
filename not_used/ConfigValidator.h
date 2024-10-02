// ConfigValidator.h
#ifndef CONFIGVALIDATOR_H
#define CONFIGVALIDATOR_H

#include <yaml-cpp/yaml.h>

#include <sol/sol.hpp>

class ConfigData;

class ConfigValidator {
public:
  ConfigValidator() = default;
  ~ConfigValidator() = default;

  // Prevent copying and moving
  ConfigValidator(const ConfigValidator &) = delete;
  ConfigValidator(ConfigValidator &&) = delete;
  ConfigValidator &operator=(const ConfigValidator &) = delete;
  ConfigValidator &operator=(ConfigValidator &&) = delete;

  // Validate the config data
  bool Validate(const ConfigData &config);

  bool ValidateAgainstLua(const ConfigData &config, const YAML::Node &schema,
                          sol::state &lua);

  // Optionally, validate against YAML schema rules
  bool ValidateAgainstSchema(const ConfigData &config,
                             const YAML::Node &schema);

  // Helper method to load entire config into Lua
  void LoadConfigToLua(sol::state &lua, const YAML::Node &config);

  // Helper method to convert YAML to Lua tables
  sol::table PushYamlToLua(sol::state &lua, const YAML::Node &node);

private:
  // Helper functions for different validation rules
  // void ValidateTimestep(double timestep);
  // void ValidateGravity(double gravity);
  // Other validation logic for fields
};

#endif  // CONFIGVALIDATOR_H

