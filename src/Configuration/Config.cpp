#include <yaml-cpp/yaml.h>
#include "Config.h"
#include "Utils/AscFile.h"
#include "Utils/Logger.h"

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

  config_data_.genotype_parameters =
    config["genotype_parameters"].as<GenotypeParameters>();

  config_data_.drug_parameters =
    config["drug_parameters"].as<DrugParameters>();

  config_data_.therapy_parameters =
    config["therapy_parameters"].as<TherapyParameters>();

  config_data_.strategy_parameters =
    config["strategy_parameters"].as<StrategyParameters>();

  config_data_.epidemiological_parameters =
    config["epidemiological_parameters"].as<EpidemiologicalParameters>();

  config_data_.mosquito_parameters =
    config["mosquito_parameters"].as<MosquitoParameters>();

  config_data_.population_events =
    config["population_events"].as<PopulationEvents>();

  // Validate all cross field validations
  validate_all_cross_field_validations();
}

void Config::validate_all_cross_field_validations() {
  if(config_data_.spatial_settings.get_mode() == "grid_based") {
    // Validate all raster file paths
    AscFile *asc_file = new AscFile();
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_population_raster(),
                          AscFile::Type::Population)) {
      std::cout << "Population raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_district_raster(),
                           AscFile::Type::District)) {
      std::cout << "District raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_beta_raster(),
                           AscFile::Type::Beta)) {
      std::cout << "Beta raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_p_treatment_over_5_raster(),
                            AscFile::Type::Treatment)) {
      std::cout << "P treatment over 5 raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_p_treatment_under_5_raster(),
                            AscFile::Type::Treatment)) {
      std::cout << "P treatment under 5 raster file validated successfully" << std::endl;
    }
  }
  if(config_data_.mosquito_parameters.get_mosquito_config().get_mode() == "grid_based") {
    // Validate all raster file paths
    AscFile *asc_file = new AscFile();
    if(asc_file->load_and_validate(config_data_.mosquito_parameters.get_mosquito_config().get_grid_based().get_interrupted_feeding_rate_raster(),
                                    AscFile::Type::Mosquito_IFR)) {
      std::cout << "Interrupted feeding rate raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.mosquito_parameters.get_mosquito_config().get_grid_based().get_prmc_size_raster(),
                           AscFile::Type::Mosquito_Size)) {
      std::cout << "PRMC size raster file validated successfully" << std::endl;
    }
  }
  // Validate model settings


}


void Config::reload() { load(config_file_path_); }

