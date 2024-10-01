#include "Config.h"

#include <iostream>

bool Config::ValidateNode(const YAML::Node &node, const YAML::Node &schema) {
  for (auto it = schema.begin(); it != schema.end(); ++it) {
    std::string key = it->first.as<std::string>();
    const YAML::Node &schema_field = it->second;

    // Check if the field is required and present
    if (schema_field["required"] && schema_field["required"].as<bool>()
        && !node[key]) {
      std::cerr << "Missing required field: " << key << std::endl;
      return false;
    }

    // If the field exists, check the type
    if (node[key]) {
      std::string expected_type = schema_field["type"].as<std::string>();
      if (expected_type == "double" && !node[key].IsScalar()) {
        std::cerr << "Invalid type for field: " << key << " (expected double)"
                  << std::endl;
        return false;
      }
      if (expected_type == "string" && !node[key].IsScalar()) {
        std::cerr << "Invalid type for field: " << key << " (expected string)"
                  << std::endl;
        return false;
      }

      // Additional checks like min, max can be added
      if (expected_type == "double" && schema_field["min"]) {
        double value = node[key].as<double>();
        if (value < schema_field["min"].as<double>()) {
          std::cerr << "Value for " << key
                    << " is less than the minimum allowed: "
                    << schema_field["min"].as<double>() << std::endl;
          return false;
        }
      }
    }
  }
  return true;
}
