// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <date/date.h>
#include <string>
#include "ConfigData.h"

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
  bool load(const std::string &filename);

  // Reload configuration (useful for dynamic updates)
  void reload();

  // Validate all cross-field validations
  void validate_all_cross_field_validations();

  // Getters for entire configuration structures
  [[nodiscard]] const ModelSettings &get_model_settings() const {
    return config_data_.model_settings;
  }
  [[nodiscard]] const SimulationTimeframe &get_simulation_timeframe() const {
    return config_data_.simulation_timeframe;
  }
  [[nodiscard]] const TransmissionSettings &get_transmission_settings() const {
    return config_data_.transmission_settings;
  }
  [[nodiscard]] const PopulationDemographic &get_population_demographic()
      const {
    return config_data_.population_demographic;
  }
  [[nodiscard]] const SpatialSettings &get_spatial_settings() const {
    return config_data_.spatial_settings;
  }
  [[nodiscard]] const SeasonalitySettings &get_seasonality_settings() const {
    return config_data_.seasonality_settings;
  }
  [[nodiscard]] const MovementSettings &get_movement_settings() const {
    return config_data_.movement_settings;
  }
  [[nodiscard]] const ParasiteParameters &get_parasite_parameters() const {
    return config_data_.parasite_parameters;
  }
  [[nodiscard]] const ImmuneSystemParameters &get_immune_system_parameters()
      const {
    return config_data_.immune_system_parameters;
  }
  [[nodiscard]] const GenotypeParameters &get_genotype_parameters()
      const {
    return config_data_.genotype_parameters;
  }
  [[nodiscard]] const DrugParameters &get_drug_parameters()
      const {
    return config_data_.drug_parameters;
  }
  [[nodiscard]] const TherapyParameters &get_therapy_parameters()
      const {
    return config_data_.therapy_parameters;
  }
  [[nodiscard]] const StrategyParameters &get_strategy_parameters()
      const {
    return config_data_.strategy_parameters;
  }
  [[nodiscard]] const EpidemiologicalParameters &get_epidemiological_parameters()
      const {
    return config_data_.epidemiological_parameters;
  }
  [[nodiscard]] const MosquitoParameters &get_mosquito_parameters()
      const {
    return config_data_.mosquito_parameters;
  }
  [[nodiscard]] const PopulationEvents &get_population_events()
      const {
    return config_data_.population_events;
  }

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
