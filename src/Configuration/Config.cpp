#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "Config.h"
#include "Utils/AscFile.h"

int inline get_pipe_count(const std::string &str) {
  int pipe_count = 0;
  for(const char c : str) {
    if(c == '|') {
      pipe_count++;
    }
  }
  return pipe_count;
}

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
  /*----------------------------
  Validate model settings
  ----------------------------*/
  ModelSettings model_settings = config_data_.model_settings;
  //Check if days between stdout output is a positive number
  if(model_settings.get_days_between_stdout_output() < 0) {
    throw std::invalid_argument("Days between stdout output should be a positive number");
  }

  /*----------------------------
  Validate simulation timeframe
  ----------------------------*/
  SimulationTimeframe simulation_timeframe = config_data_.simulation_timeframe;
  //Check if ending date is greater than starting date
  if(simulation_timeframe.get_starting_date() > simulation_timeframe.get_ending_date()) {
      throw std::invalid_argument("Simulation ending date should be greater than starting date");
  }
  //Comparison period should be in between starting and ending date
  if(simulation_timeframe.get_start_of_comparison_period() < simulation_timeframe.get_starting_date() ||
       simulation_timeframe.get_start_of_comparison_period() > simulation_timeframe.get_ending_date()) {
      throw std::invalid_argument("Comparison period should be in between starting and ending date");
  }
  //start_collect_data_day should be >= 0
  if(simulation_timeframe.get_start_collect_data_day() < 0) {
      throw std::invalid_argument("Start collect data day should be greater than or equal to 0");
  }

  /*----------------------------
  Validate transmission settings
  ----------------------------*/
  TransmissionSettings transmission_settings = config_data_.transmission_settings;
  //Check transmission_parameter in range [0,1]
  if(transmission_settings.get_transmission_parameter() < 0 || transmission_settings.get_transmission_parameter() > 1) {
    throw std::invalid_argument("Transmission parameter should be in range [0,1]");
  }
  //Check if p_infection_from_an_infectious_bite is in range [0,1]
  if(transmission_settings.get_p_infection_from_an_infectious_bite() < 0 || transmission_settings.get_p_infection_from_an_infectious_bite() > 1) {
    throw std::invalid_argument("P infection from an infectious bite should be in range [0,1]");
  }

  /*----------------------------
  Validate population demographic
  ----------------------------*/
  PopulationDemographic population_demographic = config_data_.population_demographic;
  //Check if number_of_age_classes is a positive number
  if(population_demographic.get_number_of_age_classes() <= 0) {
      throw std::invalid_argument("Number of age classes should be a positive number");
  }
  //Check if age_structure size is equal to number_of_age_classes
  if(population_demographic.get_age_structure().size() != population_demographic.get_number_of_age_classes()) {
      throw std::invalid_argument("Age structure size should be equal to number of age classes");
  }
  //Check if age_structure is positive array
  for(auto age_structure : population_demographic.get_age_structure()) {
      if(age_structure < 0) {
      throw std::invalid_argument("Age structure should be a positive array");
      }
  }
  //Check if initial_age_structure is positive array
  for(auto initial_age_structure : population_demographic.get_initial_age_structure()) {
      if(initial_age_structure < 0) {
      throw std::invalid_argument("Initial age structure should be a positive array");
      }
  }
  //Check if birth_rate is a positive number
  if(population_demographic.get_birth_rate() <= 0) {
      throw std::invalid_argument("Birth rate should be a positive number");
  }
  //Check if death_rate_by_age_class is a positive array
  for(auto death_rate_by_age_class : population_demographic.get_death_rate_by_age_class()) {
      if(death_rate_by_age_class < 0) {
      throw std::invalid_argument("Death rate by age class should be a positive array");
      }
  }
  //Check if mortality_when_treatment_fail_by_age_class is a positive array
  for(auto mortality_when_treatment_fail_by_age_class : population_demographic.get_mortality_when_treatment_fail_by_age_class()) {
      if(mortality_when_treatment_fail_by_age_class < 0) {
      throw std::invalid_argument("Mortality when treatment fail by age class should be a positive array");
      }
  }
  //Check if artificial_rescaling_of_population_size is a positive number
  if(population_demographic.get_artificial_rescaling_of_population_size() <= 0) {
      throw std::invalid_argument("Artificial rescaling of population size should be a positive number");
  }

  /*----------------------------
  Validate spatial settings
  ----------------------------*/
  SpatialSettings spatial_settings = config_data_.spatial_settings;
  //Check if mode is either grid_based or location_based
  if(spatial_settings.get_mode() != "grid_based" && spatial_settings.get_mode() != "location_based") {
      throw std::invalid_argument("Spatial mode should be either grid_based or location_based");
  }
  //If mode is grid_based, check if all raster file paths are provided
  if(spatial_settings.get_mode() == "grid_based") {
    SpatialSettings::GridBased grid_based = spatial_settings.get_grid_based();
    if(grid_based.get_population_raster().empty() || grid_based.get_district_raster().empty() ||
       grid_based.get_beta_raster().empty() || grid_based.get_p_treatment_over_5_raster().empty() ||
       grid_based.get_p_treatment_under_5_raster().empty()) {
      throw std::invalid_argument("All raster file paths should be provided for grid based spatial mode");
    }
    // Validate all raster file paths
    utils::AscFile *asc_file = new utils::AscFile();
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_population_raster(),
                          utils::AscFile::Type::Population)) {
      std::cout << "Population raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_district_raster(),
                           utils::AscFile::Type::District)) {
      std::cout << "District raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_beta_raster(),
                           utils::AscFile::Type::Beta)) {
      std::cout << "Beta raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_p_treatment_over_5_raster(),
                            utils::AscFile::Type::Treatment)) {
      std::cout << "P treatment over 5 raster file validated successfully" << std::endl;
    }
    if(asc_file->load_and_validate(config_data_.spatial_settings.get_grid_based().get_p_treatment_under_5_raster(),
                            utils::AscFile::Type::Treatment)) {
      std::cout << "P treatment under 5 raster file validated successfully" << std::endl;
    }
    //Check if age_distribution_by_location size is different from 1
    if(grid_based.get_age_distribution_by_location().size() != 1) {
      throw std::invalid_argument("Age distribution using raster must be 1 location (to distribute equally)");
    }
    //Check if age_distribution_by_location size matched initial_age_structure size
    if(grid_based.get_age_distribution_by_location()[0].size() != population_demographic.get_initial_age_structure().size()) {
      throw std::invalid_argument("Age distribution by raster must have size match initial age structure size");
    }
  }
  //Location based
  if(spatial_settings.get_mode() == "location_based") {
    SpatialSettings::LocationBased location_based = spatial_settings.get_location_based();
    if(location_based.get_population_size_by_location().empty() || location_based.get_location_info().empty() ||
       location_based.get_age_distribution_by_location().empty() || location_based.get_p_treatment_under_5_by_location().empty() ||
       location_based.get_p_treatment_over_5_by_location().empty() || location_based.get_beta_by_location().empty()) {
      throw std::invalid_argument("All locations should be provided for location based spatial mode");
    }
    //Check if all location sizes are equal
    if(location_based.get_population_size_by_location().size() != location_based.get_location_info().size() ||
       location_based.get_population_size_by_location().size() != location_based.get_age_distribution_by_location().size() ||
       location_based.get_population_size_by_location().size() != location_based.get_p_treatment_under_5_by_location().size() ||
       location_based.get_population_size_by_location().size() != location_based.get_p_treatment_over_5_by_location().size() ||
       location_based.get_population_size_by_location().size() != location_based.get_beta_by_location().size()) {
      throw std::invalid_argument("All location sizes should be equal");
    }
    //Check if age_distribution_by_location size matched initial_age_structure size
    if(location_based.get_age_distribution_by_location()[0].size() != population_demographic.get_initial_age_structure().size()) {
      throw std::invalid_argument("Age distribution by location size should match initial age structure size");
    }
  }

  /*----------------------------
  Validate seasonality settings
  ----------------------------*/
  SeasonalitySettings seasonality_settings = config_data_.seasonality_settings;
  //Check if rainfall file name is provided
  if(seasonality_settings.get_rainfall().get_filename().empty()) {
    throw std::invalid_argument("Rainfall file name should be provided");
  }
  //Check if rainfall file exists
  if(!std::filesystem::exists(seasonality_settings.get_rainfall().get_filename())) {
    throw std::invalid_argument("Rainfall file does not exist");
  }

  /*----------------------------
  Validate movement settings
  ----------------------------*/
  MovementSettings movement_settings = config_data_.movement_settings;
  //Check if Barabasi parameters are valid
  if(movement_settings.get_spatial_model().get_name() == "Barabasi") {
      MovementSettings::BarabasiSettings barabasi = movement_settings.get_spatial_model().get_barabasi();
      if(barabasi.get_r_g_0() <= 0 || barabasi.get_beta_r() <= 0 || barabasi.get_kappa() <= 0) {
      throw std::invalid_argument("Barabasi parameters should be positive numbers");
      }
  }
  //Check if Wesolowski parameters are valid
  if(movement_settings.get_spatial_model().get_name() == "Wesolowski") {
      MovementSettings::WesolowskiSettings wesolowski = movement_settings.get_spatial_model().get_wesolowski();
      if(wesolowski.get_alpha() <= 0 || wesolowski.get_beta() <= 0 || wesolowski.get_gamma() <= 0 || wesolowski.get_kappa() <= 0) {
      throw std::invalid_argument("Wesolowski parameters should be positive numbers");
      }
  }
  //Check if circular parameters are valid
  MovementSettings::CirculationInfo circulation_info = movement_settings.get_circulation_info();
  if(circulation_info.get_max_relative_moving_value() < 0 || circulation_info.get_number_of_moving_levels() <= 0) {
    throw std::invalid_argument("Max relative moving value should be >= 0 and number of moving levels should be > 0");
  }
  //Check if circulation_percent is a positive number
  if(circulation_info.get_circulation_percent() < 0) {
      throw std::invalid_argument("Circulation percent cannot be negative");
  }

  /*----------------------------
  Validate parasite parameters
  ----------------------------*/
  ParasiteParameters parasite_parameters = config_data_.parasite_parameters;
  //All log numbers should be smaller than 6
  if(parasite_parameters.get_parasite_density_levels().get_log_parasite_density_cured() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_from_liver() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_asymptomatic() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_clinical() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_clinical_from() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_clinical_to() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_detectable() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_detectable_pfpr() >= 6 ||
     parasite_parameters.get_parasite_density_levels().get_log_parasite_density_pyrogenic() >= 6) {
    throw std::invalid_argument("All log parasite density should be smaller than 6");
  }
  //Check if recombination_parameters is in range[0,1]
  if(parasite_parameters.get_recombination_parameters().get_within_chromosome_recombination_rate() < 0 ||
       parasite_parameters.get_recombination_parameters().get_within_chromosome_recombination_rate() > 1) {
      throw std::invalid_argument("Within_chromosome_recombination_rate should be in range [0,1]");
  }

  /*----------------------------
  Validate immune system parameters
  ----------------------------*/
    ImmuneSystemParameters immune_system_parameters = config_data_.immune_system_parameters;
  //Check if all parameters are positive numbers
    if(immune_system_parameters.get_b1() < 0 || immune_system_parameters.get_b2() < 0
      || immune_system_parameters.get_duration_for_naive() < 0 || immune_system_parameters.get_duration_for_naive() < 0
      || immune_system_parameters.get_mean_initial_condition() < 0 || immune_system_parameters.get_sd_initial_condition() < 0
      || immune_system_parameters.get_immune_inflation_rate() < 0 || immune_system_parameters.get_min_clinical_probability() < 0
      || immune_system_parameters.get_max_clinical_probability() < 0 || immune_system_parameters.get_immune_effect_on_progression_to_clinical() < 0
      || immune_system_parameters.get_age_mature_immunity() < 0 || immune_system_parameters.get_factor_effect_age_mature_immunity() < 0
      || immune_system_parameters.get_midpoint() < 0){
      throw std::invalid_argument("All immune system parameters should be positive numbers");
  }

  /*----------------------------
  Validate genotype parameters
  ----------------------------*/
    GenotypeParameters genotype_parameters = config_data_.genotype_parameters;
  //Check if mask contains 13 "|" characters
  if(get_pipe_count(genotype_parameters.get_mutation_mask()) != 13) {
    throw std::invalid_argument("Override EC50 pattern should contain 13 '|' characters");
  }
    //Check if mutation rate is in mutation_probability_per_locus [0,1]
  if(genotype_parameters.get_mutation_probability_per_locus() < 0 || genotype_parameters.get_mutation_probability_per_locus() > 1) {
    throw std::invalid_argument("Mutation rate should be in range [0,1]");
  }
  //Check override_ec50_patterns, each pattern size should match mutation mask size
  for(auto override_ec50_pattern : genotype_parameters.get_override_ec50_patterns()) {
    if(override_ec50_pattern.get_pattern().size() != genotype_parameters.get_mutation_mask().size()) {
      throw std::invalid_argument("Override EC50 pattern size should match mutation mask size");
    }
    //Pattern should contains 13 "|" characters
    if(get_pipe_count(override_ec50_pattern.get_pattern()) != 13) {
      throw std::invalid_argument("Override EC50 pattern should contain 13 '|' characters");
    }
  }
  //Check if aa_sequence in genotype_info of initial_genotype_info has correct string size and format
  for(auto initial_genotype_info : genotype_parameters.get_initial_genotype_info()) {
    for(auto genotype_info : initial_genotype_info.get_genotype_info()) {
      if(get_pipe_count(genotype_info.get_aa_sequence()) != 13) {
        throw std::invalid_argument("Initial genotype aa sequence should contain 13 '|' characters");
      }
      if(genotype_info.get_aa_sequence().size() != genotype_parameters.get_mutation_mask().size()) {
        throw std::invalid_argument("Initial genotype aa sequence size should match mutation mask size");
      }
    }
  }

  /*----------------------------
  Validate drug parameters
  ----------------------------*/
  //Loop through all drug parameters
  DrugParameters drug_parameters = config_data_.drug_parameters;
  for(auto drug: drug_parameters.get_drug_db()) {
    if(drug.second.get_half_life() < 0 || drug.second.get_maximum_parasite_killing_rate() < 0
      || drug.second.get_n() < 0 || drug.second.get_k() < 0 || drug.second.get_base_EC50() < 0) {
        throw std::invalid_argument("All drug parameters should be positive numbers");
    }
    //maximum_parasite_killing_rate should be in range [0,1]
    if(drug.second.get_maximum_parasite_killing_rate() < 0 || drug.second.get_maximum_parasite_killing_rate() > 1) {
      throw std::invalid_argument("Maximum parasite killing rate should be in range [0,1]");
    }
    if(drug.second.get_age_specific_drug_concentration_sd().size() != population_demographic.get_number_of_age_classes()) {
      throw std::invalid_argument("Age specific drug concentration size should match number of age classes");
    }
    if(drug.second.get_age_specific_drug_absorption().size() > 0 &&
       drug.second.get_age_specific_drug_absorption().size() != population_demographic.get_number_of_age_classes()) {
      throw std::invalid_argument("Age specific drug absorption size should match number of age classes");
    }
  }

  /*----------------------------
  Validate therapy parameters
  ----------------------------*/
  TherapyParameters therapy_parameters = config_data_.therapy_parameters;
  //Check if tf_testing_day is positive number
  if(therapy_parameters.get_tf_testing_day() < 0) {
      throw std::invalid_argument("TF testing day should be a positive number");
  }
  //Check if tf_rate is in range [0,1]
  if(therapy_parameters.get_tf_rate() < 0 || therapy_parameters.get_tf_rate() > 1) {
      throw std::invalid_argument("TF rate should be in range [0,1]");
  }
  int therapy_max_dosing_days = 0;
  //Loop through therapy_db
  for(auto therapy : therapy_parameters.get_therapy_db()) {
    for(auto drug_id : therapy.second.get_drug_ids()) {
      //Check if drug id is in drug_db keys
      if(drug_parameters.get_drug_db().find(drug_id) == drug_parameters.get_drug_db().end()) {
        throw std::invalid_argument("Drug id should be in drug db keys");
      }
      // Check if dosing days are positive numbers
      for(auto dosing_day : therapy.second.get_dosing_days()) {
          if(dosing_day < 0) {
          throw std::invalid_argument("Dosing days should be positive numbers");
          }
          if(dosing_day >= therapy_max_dosing_days) {
              therapy_max_dosing_days = dosing_day;
          }
      }
    }
  }

  /*----------------------------
  Validate strategy parameters
  ----------------------------*/
  StrategyParameters strategy_parameters = config_data_.strategy_parameters;
  //Check if initial_strategy_id is in strategy_db keys
  if(strategy_parameters.get_strategy_db().find(strategy_parameters.get_initial_strategy_id()) == strategy_parameters.get_strategy_db().end()) {
      throw std::invalid_argument("Initial strategy id should be in strategy db keys");
  }

  /*----------------------------
  Validate epidemiological parameters
  ----------------------------*/
  EpidemiologicalParameters epidemiological_parameters = config_data_.epidemiological_parameters;
  //Check if number_of_tracking_days is a positive number
  if(epidemiological_parameters.get_number_of_tracking_days() <= 0) {
      throw std::invalid_argument("Number of tracking days should be a positive number");
  }
  //Check if days_to_clinical_under_five and days_to_clinical_over_five are positive numbers
  if(epidemiological_parameters.get_days_to_clinical_under_five() < 0 || epidemiological_parameters.get_days_to_clinical_over_five() < 0) {
      throw std::invalid_argument("Days to clinical under five and over five should be positive numbers");
  }
  //Check if days_mature_gametocyte_under_five and days_mature_gametocyte_over_five are positive numbers
  if(epidemiological_parameters.get_days_mature_gametocyte_under_five() < 0 || epidemiological_parameters.get_days_mature_gametocyte_over_five() < 0) {
      throw std::invalid_argument("Days mature gametocyte under five and over five should be positive numbers");
  }
  //Check if p_compliance is in range [0,1]
  if(epidemiological_parameters.get_p_compliance() < 0 || epidemiological_parameters.get_p_compliance() > 1) {
      throw std::invalid_argument("P compliance should be in range [0,1]");
  }
  //Check if min_dosing_days is positive number
  if(epidemiological_parameters.get_min_dosing_days() < 0) {
      throw std::invalid_argument("Min dosing days should be a positive number");
  }
  //Check if min_dosing_days is less than therapy_max_dosing_days
  if(epidemiological_parameters.get_min_dosing_days() >= therapy_max_dosing_days) {
      throw std::invalid_argument("Min dosing days should be less than therapy max dosing days");
  }
  //Check if gametocyte_level_under_artemisinin_action is in range [0,1]
  if(epidemiological_parameters.get_gametocyte_level_under_artemisinin_action() < 0 ||
       epidemiological_parameters.get_gametocyte_level_under_artemisinin_action() > 1) {
      throw std::invalid_argument("Gametocyte level under artemisinin action should be in range [0,1]");
  }
  //Check if gametocyte_level_full is in range [0,1]
  if(epidemiological_parameters.get_gametocyte_level_full() < 0 ||
       epidemiological_parameters.get_gametocyte_level_full() > 1) {
      throw std::invalid_argument("Gametocyte level full transmission should be in range [0,1]");
  }
  //Check if p_relapse is in range [0,1]
  if(epidemiological_parameters.get_p_relapse() < 0 || epidemiological_parameters.get_p_relapse() > 1) {
      throw std::invalid_argument("P relapse should be in range [0,1]");
  }
  //Check if relapse_duration is a positive number
  if(epidemiological_parameters.get_relapse_duration() < 0) {
      throw std::invalid_argument("Relapse duration should be a positive number");
  }
  //Check if relapse_rate is positive number
  if(epidemiological_parameters.get_relapse_rate() < 0) {
      throw std::invalid_argument("Relapse rate should be a positive number");
  }
  //Check if update_frequency is a positive number
  if(epidemiological_parameters.get_update_frequency() < 0) {
      throw std::invalid_argument("Update frequency should be a positive number");
  }
  //Check if tf_window_size is a positive number
  if(epidemiological_parameters.get_tf_window_size() < 0) {
      throw std::invalid_argument("TF window size should be a positive number");
  }
  //Check if fraction_mosquitoes_interrupted_feeding is in range [0,1]
  if(epidemiological_parameters.get_fraction_mosquitoes_interrupted_feeding() < 0 ||
     epidemiological_parameters.get_fraction_mosquitoes_interrupted_feeding() > 1) {
      throw std::invalid_argument("Fraction mosquitoes interrupted feeding should be in range [0,1]");
  }
  //Check if inflation_factor is positive number
  if(epidemiological_parameters.get_inflation_factor() < 0) {
      throw std::invalid_argument("Inflation factor should be a positive number");
  }
  EpidemiologicalParameters::RelativeBitingInfo relative_biting_info = epidemiological_parameters.get_relative_biting_info();
  //Check if relative_biting_info is positive number
  if(relative_biting_info.get_max_relative_biting_value() < 0 || relative_biting_info.get_min_relative_biting_value() <= 0
    || relative_biting_info.get_number_of_biting_levels() <= 0) {
    throw std::invalid_argument("Relative biting info should be positive numbers");
  }
  //Check if min_relative_biting_value <= max_relative_biting_value
  if(relative_biting_info.get_min_relative_biting_value() >= relative_biting_info.get_max_relative_biting_value()) {
      throw std::invalid_argument("Min relative biting value should be less than max relative biting value");
  }
  EpidemiologicalParameters::RelativeInfectivity relative_infectivity = epidemiological_parameters.get_relative_infectivity();
  //Check if relative_infectivity is positive number
  if(relative_infectivity.get_sigma() < 0 || relative_infectivity.get_ro() <= 0
      || relative_infectivity.get_blood_meal_volume() <= 0) {
      throw std::invalid_argument("Relative infectivity should be positive numbers");
  }

  /*----------------------------
  Validate mosquito parameters
  ----------------------------*/
  MosquitoParameters mosquito_parameters = config_data_.mosquito_parameters;
  //Check if mosquito_mode is either grid_based or location_based
  if(mosquito_parameters.get_mosquito_config().get_mode() != "grid_based" && mosquito_parameters.get_mosquito_config().get_mode() != "location_based") {
      throw std::invalid_argument("Mosquito mode should be either grid_based or location_based");
  }
  //If mode is grid_based, check if all raster file paths are provided
  if(mosquito_parameters.get_mosquito_config().get_mode() == "grid_based") {
    MosquitoParameters::GridBased grid_based = mosquito_parameters.get_mosquito_config().get_grid_based();
    utils::AscFile *asc_file = new utils::AscFile();
    if(asc_file->load_and_validate(grid_based.get_interrupted_feeding_rate_raster(),
                                    utils::AscFile::Type::Mosquito_IFR)) {
      std::cout << "Interrupted feeding rate raster file validated successfully" << std::endl;
                                    }
    if(asc_file->load_and_validate(grid_based.get_prmc_size_raster(),
                           utils::AscFile::Type::Mosquito_Size)) {
      std::cout << "PRMC size raster file validated successfully" << std::endl;
    }
  }
  //If location_based, check if all location sizes are equal
  if(mosquito_parameters.get_mosquito_config().get_mode() == "location_based") {
    MosquitoParameters::LocationBased location_based = mosquito_parameters.get_mosquito_config().get_location_based();
    if(location_based.get_interrupted_feeding_rate().empty() || location_based.get_prmc_size().empty()) {
      throw std::invalid_argument("All locations should be provided for location based mosquito mode");
    }
    //Check if all location sizes are equal
    SpatialSettings::LocationBased spatial_location_based = spatial_settings.get_location_based();
    if(location_based.get_interrupted_feeding_rate().size() != location_based.get_prmc_size().size()
      && location_based.get_interrupted_feeding_rate().size() != spatial_location_based.get_population_size_by_location().size()) {
      throw std::invalid_argument("All location sizes should be equal");
    }
  }

  /*----------------------------
  Validate population events
  ----------------------------*/
  PopulationEvents population_events = config_data_.population_events;
  //Loop through all events
  for(auto population_event : population_events.get_events()) {
    //Check if name is provided
    if(population_event.get_name().empty()) {
      throw std::invalid_argument("Name should be provided for all population events");
    }
    for(auto event_info : population_event.get_info()) {
      //Check if event date is valid
      if(event_info.get_date() <= simulation_timeframe.get_starting_date() ||
           event_info.get_date() >= simulation_timeframe.get_ending_date()) {
          throw std::invalid_argument("Event date should be in between simulation starting and ending date");
      }
    }
  }
}

void Config::reload() { load(config_file_path_); }

