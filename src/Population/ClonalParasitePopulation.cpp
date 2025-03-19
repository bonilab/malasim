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

const double ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY = -1000;

ClonalParasitePopulation::ClonalParasitePopulation(Genotype *genotype)
    : last_update_log10_parasite_density_(LOG_ZERO_PARASITE_DENSITY),
      gametocyte_level_(0.0),
      first_date_in_blood_(-1),
      parasite_population_(nullptr),
      genotype_(genotype),
      update_function_(nullptr) {}

ClonalParasitePopulation::~ClonalParasitePopulation() = default;

double ClonalParasitePopulation::get_current_parasite_density(const int &current_time) {
  const auto duration = current_time - parasite_population()->latest_update_time();
  if (duration == 0) {
    return last_update_log10_parasite_density_;
  }

  if (update_function_ == nullptr) {
    //        std::cout << "hello" << std::endl;
    return last_update_log10_parasite_density_;
  }

  return update_function_->get_current_parasite_density(this, duration);
}

double ClonalParasitePopulation::get_log10_infectious_density() const {
  if (NumberHelpers::is_equal(last_update_log10_parasite_density_, LOG_ZERO_PARASITE_DENSITY)
      || NumberHelpers::is_equal(gametocyte_level_, 0.0))
    return LOG_ZERO_PARASITE_DENSITY;

  return last_update_log10_parasite_density_ + log10(gametocyte_level_);
}

double ClonalParasitePopulation::last_update_log10_parasite_density() const {
  return last_update_log10_parasite_density_;
}

void ClonalParasitePopulation::set_last_update_log10_parasite_density(const double &value) {
  last_update_log10_parasite_density_ = value;
}

double ClonalParasitePopulation::gametocyte_level() const {
  return gametocyte_level_;
}

void ClonalParasitePopulation::set_gametocyte_level(const double &value) {
  if (NumberHelpers::is_enot_qual(gametocyte_level_, value)) {
    gametocyte_level_ = value;
  }
}

Genotype *ClonalParasitePopulation::genotype() const {
  return genotype_;
}

void ClonalParasitePopulation::set_genotype(Genotype *value) {
  if (genotype_ != value) {
    genotype_ = value;
  }
}

bool ClonalParasitePopulation::resist_to(const int &drug_id) const {
  return genotype_->resist_to(Model::get_config()->get_drug_parameters().drug_db->at(drug_id));
}

void ClonalParasitePopulation::update() {
  set_last_update_log10_parasite_density(get_current_parasite_density(Model::get_scheduler()->current_time()));
}

void ClonalParasitePopulation::perform_drug_action(const double &percent_parasite_remove) {
  double newSize = last_update_log10_parasite_density_;
  if (percent_parasite_remove > 1) {
    newSize = Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_cured();
  } else {
//    std::cout << "Day: " << Model::get_scheduler()->current_time() << "\tDrug: old density: "
//    << newSize << "\tremoved by drug: " << log10(1 - percent_parasite_remove);
    newSize += log10(1 - percent_parasite_remove);
//    std::cout << "\tnew density: " << newSize << std::endl;
  }

  if (newSize < Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_cured()) {
    newSize = Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_cured();
  }

//      std::cout << Model::get_scheduler()->current_time() << "\t" <<parasite_population()->person()->last_therapy_id() << "\t"  <<
//      percent_parasite_remove << "\t"<<last_update_log10_parasite_density_ << "\t" <<newSize << std::endl;
  set_last_update_log10_parasite_density(newSize);
}
