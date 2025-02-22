#include "ClinicalUpdateFunction.h"
#include "Simulation/Model.h"
#include "Configuration/Config.h"

ClinicalUpdateFunction::ClinicalUpdateFunction(Model *model) : model_(model) {
}

ClinicalUpdateFunction::~ClinicalUpdateFunction() = default;

double ClinicalUpdateFunction::get_current_parasite_density(ClonalParasitePopulation *parasite, int duration) {
  return model_->get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_asymptomatic();
}