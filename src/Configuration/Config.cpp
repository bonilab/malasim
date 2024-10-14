
#include "Config.h"

#include <yaml-cpp/yaml.h>

void Config::load(const std::string &filename) {
  config_file_path_ = filename;
  YAML::Node config = YAML::LoadFile(filename);

  config_data_.model_settings = config["model_settings"].as<ModelSettings>();
  config_data_.simulation_timeframe  =
    config["simulation_timeframe"].as<SimulationTimeframe>();

  config_data_.transmission_settings =
      config["transmission_settings"].as<TransmissionSettings>();

  config_data_.population_demographic =
    config["population_demographic"].as<PopulationDemographic>();

  config_data_.spatial_settings =
     config["spatial_settings"].as<SpatialSettings>();

  config_data_.seasonality_settings =
    config["seasonality_settings"].as<SeasonalitySettings>();

  config_data_.movement_settings =
    config["movement_settings"].as<MovementSettings>();

  config_data_.parasite_parameters =
    config["parasite_parameters"].as<ParasiteParameters>();

  config_data_.immune_system_parameters =
    config["immune_system_parameters"].as<ImmuneSystemParameters>();

  config_data_.epidemiological_parameters =
    config["epidemiological_parameters"].as<EpidemiologicalParameters>();


  // Validate all cross field validations
  validate_all_cross_field_validations();
}

void Config::reload() { load(config_file_path_); }

