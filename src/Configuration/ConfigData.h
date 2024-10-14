#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "EpidemiologicalParameters.h"
#include "ImmuneSystemParameters.h"
#include "ModelSettings.h"
#include "MovementSettings.h"
#include "ParasiteParameters.h"
#include "PopulationDemographic.h"
#include "SeasonalitySettings.h"
#include "TransmissionSettings.h"
#include "SimulationTimeframe.h"
#include "SpatialSettings.h"

struct ConfigData {
  ModelSettings model_settings{};
  TransmissionSettings transmission_settings{};
  PopulationDemographic population_demographic{};
  SimulationTimeframe simulation_timeframe{};
  SpatialSettings spatial_settings{};
  SeasonalitySettings seasonality_settings{};
  MovementSettings movement_settings{};
  ParasiteParameters parasite_parameters{};
  ImmuneSystemParameters immune_system_parameters{};
  EpidemiologicalParameters epidemiological_parameters{};
};

#endif  // CONFIG_DATA_H

