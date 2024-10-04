// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <date/date.h>
#include <yaml-cpp/yaml.h>

#include <string>

#include "ConfigData.h"

#define GETTER(Type, StructName, FieldName, Validator)   \
  [[nodiscard]] Type get_##FieldName() const {           \
    return get_field(config_data_.StructName.FieldName); \
  }

#define SETTER(Type, StructName, FieldName, Validator)       \
  void set_##FieldName(Type FieldName) {                     \
    Validator(FieldName);                                    \
    set_field(config_data_.StructName.FieldName, FieldName); \
  }

class Config {
public:
  // Constructor and Destructor
  Config() = default;
  ~Config() = default;

  // delete copy and move constructors and assign operators
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;
  Config(Config &&) = delete;
  Config &operator=(Config &&) = delete;

  // Load configuration from a YAML file
  void load(const std::string &filename);

  // Reload configuration (useful for dynamic updates)
  void reload();

  // Getters for entire configuration structures
  [[nodiscard]] const ModelSettings &get_model_settings() const {
    return config_data_.model_settings;
  }
  [[nodiscard]] const TransmissionSettings &get_transmission_settings() const {
    return config_data_.transmission_settings;
  }
  [[nodiscard]] const PopulationDemographic &get_population_demographic()
      const {
    return config_data_.population_demographic;
  }

  // Validate all cross-field validations
  void validate_all_cross_field_validations();

private:
  // Template method for getting a field
  template <typename T>
  [[nodiscard]] const T &get_field(const T &field) const {
    return field;
  }

  // Template method for setting a field
  template <typename T>
  void set_field(T &field, const T &value) {
    field = value;
  }
  // Configuration Data
  ConfigData config_data_;

  // Configuration File Path
  std::string config_file_path_;
};

#endif  // CONFIG_H
