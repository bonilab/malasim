
#include "Config.h"

#include <yaml-cpp/yaml.h>

#include <mutex>

#include "YAMLConverters.h"

void Config::load(const std::string &filename) {
  config_file_path_ = filename;
  YAML::Node config = YAML::LoadFile(filename);

  config_data_.model_settings = config["model_settings"].as<ModelSettings>();

  config_data_.transmission_settings =
      config["transmission_settings"].as<TransmissionSettings>();

  config_data_.population_demographic =
      config["population_demographic"].as<PopulationDemographic>();

  // Validate all cross field validations
  validate_all_cross_field_validations();
}

void Config::reload() { load(config_file_path_); }

