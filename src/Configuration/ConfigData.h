#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "ModelSettings.h"
#include "PopulationDemographic.h"
#include "TransmissionSettings.h"

struct ConfigData {
  ModelSettings model_settings;
  TransmissionSettings transmission_settings;
  PopulationDemographic population_demographic;
};

#endif  // CONFIG_DATA_H

