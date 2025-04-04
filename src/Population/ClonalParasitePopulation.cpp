#include "ClonalParasitePopulation.h"
#include <cmath>
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Helpers/NumberHelpers.hxx"
#include "Simulation/Model.h"
#include "Person/Person.h"
#include "SingleHostClonalParasitePopulations.h"
#include "Parasites/Genotype.h"
#include "Treatment/Therapies/Therapy.h"


ClonalParasitePopulation::ClonalParasitePopulation(Genotype *genotype)
    : genotype_(genotype) {}

ClonalParasitePopulation::~ClonalParasitePopulation() = default;

double ClonalParasitePopulation::get_current_parasite_density(const int &current_time) {
  if (update_function_ == nullptr) {
    return last_update_log10_parasite_density_;
  }

  const auto duration = current_time - parasite_population()->latest_update_time();
  if (duration == 0) {
    return last_update_log10_parasite_density_;
  }

  if (duration < 0) {
    // duration is negative which is some error in the system
    // we do not allow thing happens in future
    spdlog::error("Duration is negative: {}", duration);
    throw std::invalid_argument("Duration is negative");
  }

  return update_function_->get_current_parasite_density(this, duration);
}

double ClonalParasitePopulation::get_log10_infectious_density() const {
  if (NumberHelpers::is_equal(last_update_log10_parasite_density_, LOG_ZERO_PARASITE_DENSITY)
      || NumberHelpers::is_equal(gametocyte_level_, 0.0))
    return LOG_ZERO_PARASITE_DENSITY;

  return last_update_log10_parasite_density_ + log10(gametocyte_level_);
}

bool ClonalParasitePopulation::resist_to(const int &drug_id) const {
  return genotype_->resist_to(Model::get_config()->get_drug_parameters().drug_db->at(drug_id));
}

void ClonalParasitePopulation::update() {
  set_last_update_log10_parasite_density(get_current_parasite_density(Model::get_scheduler()->current_time()));
}

void ClonalParasitePopulation::perform_drug_action(double percent_parasite_remove, double log10_parasite_density_cured) {

  if (percent_parasite_remove < 0) {
    throw std::invalid_argument("Percent parasite remove is less than 0");
  }

  double newSize = last_update_log10_parasite_density_;
  if (percent_parasite_remove >= 1) {
    newSize = log10_parasite_density_cured;
  } else {
    newSize += log10(1 - percent_parasite_remove);
  }

  if (newSize < log10_parasite_density_cured) {
    newSize = log10_parasite_density_cured;
  }

  set_last_update_log10_parasite_density(newSize);
}
