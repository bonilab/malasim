# Configuration

The Configuration module is responsible for loading, validating, and managing the configuration settings for the simulation. It reads configuration data from a YAML file and provides access to various configuration parameters through getter methods.

## Files

- `Config.h` and `Config.cpp`: Defines the `Config` class, which handles loading and validating the configuration file.
- `ConfigData.h`: Defines the `ConfigData` struct, which holds all the configuration parameters.
- `ConfigData.cpp`: Implements the methods for the `ConfigData` struct.

## Classes

### Config

The `Config` class is responsible for:

- Loading configuration data from a YAML file.
- Validating the configuration data.
- Providing access to the configuration parameters.

#### Methods

- `bool load(const std::string &filename)`: Loads the configuration from the specified YAML file.
- `void reload()`: Reloads the configuration file.
- `void validate_all_cross_field_validations()`: Validates all cross-field validations.
- Various getter methods to access specific configuration parameters.

### ConfigData

The `ConfigData` struct holds all the configuration parameters. It includes the following fields:

- `ModelSettings model_settings`: Configuration for the model settings.
- `TransmissionSettings transmission_settings`: Configuration for transmission settings.
- `PopulationDemographic population_demographic`: Configuration for population demographics.
- `SimulationTimeframe simulation_timeframe`: Configuration for the simulation timeframe.
- `SpatialSettings spatial_settings`: Configuration for spatial settings.
- `SeasonalitySettings seasonality_settings`: Configuration for seasonality settings.
- `MovementSettings movement_settings`: Configuration for movement settings.
- `ParasiteParameters parasite_parameters`: Configuration for parasite parameters.
- `ImmuneSystemParameters immune_system_parameters`: Configuration for immune system parameters.
- `GenotypeParameters genotype_parameters`: Configuration for genotype parameters.
- `DrugParameters drug_parameters`: Configuration for drug parameters.
- `TherapyParameters therapy_parameters`: Configuration for therapy parameters.
- `StrategyParameters strategy_parameters`: Configuration for strategy parameters.
- `EpidemiologicalParameters epidemiological_parameters`: Configuration for epidemiological parameters.
- `MosquitoParameters mosquito_parameters`: Configuration for mosquito parameters.
- `PopulationEvents population_events`: Configuration for population events.
