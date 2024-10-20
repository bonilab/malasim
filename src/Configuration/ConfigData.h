#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "DrugParameters.h"
#include "EpidemiologicalParameters.h"
#include "GenotypeParameters.h"
#include "ImmuneSystemParameters.h"
#include "ModelSettings.h"
#include "MosquitoParameters.h"
#include "MovementSettings.h"
#include "ParasiteParameters.h"
#include "PopulationDemographic.h"
#include "PopulationEvents.h"
#include "SeasonalitySettings.h"
#include "TransmissionSettings.h"
#include "SimulationTimeframe.h"
#include "SpatialSettings.h"
#include "StrategyParameters.h"
#include "TherapyParameters.h"

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
  GenotypeParameters genotype_parameters{};
  DrugParameters drug_parameters{};
  TherapyParameters therapy_parameters{};
  StrategyParameters strategy_parameters{};
  EpidemiologicalParameters epidemiological_parameters{};
  MosquitoParameters mosquito_parameters{};
  PopulationEvents population_events{};
};

#endif  // CONFIG_DATA_H

