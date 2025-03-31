#include "SingleHostClonalParasitePopulations.h"

#include <cmath>
#include <iostream>

#include "ClonalParasitePopulation.h"
#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "DrugsInBlood.h"
#include "Utils/Helpers/NumberHelpers.hxx"
#include "Utils/Helpers/ObjectHelpers.h"
#include "MDC/ModelDataCollector.h"
#include "Simulation/Model.h"
#include "Parasites/Genotype.h"
#include "Population/Person/Person.h"
#include "Treatment/Therapies/Drug.h"
#include "Mosquito/Mosquito.h"

SingleHostClonalParasitePopulations::SingleHostClonalParasitePopulations(Person* person)
    : person_(person),
      parasites_(std::vector<std::unique_ptr<ClonalParasitePopulation>>()),
      log10_total_infectious_denstiy(ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY) {}

void SingleHostClonalParasitePopulations::init() {
  parasites_.clear();
}

SingleHostClonalParasitePopulations::~SingleHostClonalParasitePopulations() {
  parasites_.clear();
  person_ = nullptr;
}

void SingleHostClonalParasitePopulations::clear() {
  parasites_.clear();
}

// transfer ownership of the parasite to the SingleHostClonalParasitePopulations
void SingleHostClonalParasitePopulations::add(ClonalParasitePopulation* blood_parasite) {
  blood_parasite->set_parasite_population(this);

  parasites_.push_back(std::unique_ptr<ClonalParasitePopulation>(blood_parasite));
  blood_parasite->set_index(parasites_.size() - 1);
  assert(parasites_.at(blood_parasite->get_index()) == blood_parasite);
}

void SingleHostClonalParasitePopulations::remove(ClonalParasitePopulation* blood_parasite) {
  remove(blood_parasite->get_index());
}

// TODO: test this
void SingleHostClonalParasitePopulations::remove(const int& index) {
  ClonalParasitePopulation* bp = parasites_[index].get();
  //    std::cout << parasites_.size() << std::endl;
  if (bp->get_index() != index) {

    spdlog::error("Incorrect index when remove parasite from SingleHostClonalParasitePopulations: {} - {} - {}",  
                 bp->get_index(), index, parasites_[index]->get_index());
    throw std::runtime_error("Incorrect index when remove parasite from SingleHostClonalParasitePopulations");
  }

  parasites_.back()->set_index(index);
  parasites_[index] = std::move(parasites_.back());
  parasites_.pop_back();
  // bp automatically deleted
}

int SingleHostClonalParasitePopulations::latest_update_time() const {
  return person_->get_latest_update_time();
}


bool SingleHostClonalParasitePopulations::contain(ClonalParasitePopulation* blood_parasite) {
  for (auto& parasite : parasites_) {
    if (blood_parasite == parasite.get()) {
      return true;
    }
  }

  return false;
}

void SingleHostClonalParasitePopulations::change_all_parasite_update_function(ParasiteDensityUpdateFunction* from,
                                                                              ParasiteDensityUpdateFunction* to) const {
  for (auto& parasite : parasites_) {
    if (parasite->update_function() == from) {
      parasite->set_update_function(to);
    }
  }
}

void SingleHostClonalParasitePopulations::update() {
  for (auto& bp : parasites_) {
    bp->update();
  }
}

void SingleHostClonalParasitePopulations::clear_cured_parasites() {
  log10_total_infectious_denstiy = ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY;
  //    std::vector<int> cured_parasites_index;
  for (int i = parasites_.size() - 1; i >= 0; i--) {
    if (parasites_[i]->last_update_log10_parasite_density()
        <= Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_cured() + 0.00001) {
      remove(i);
    } else {
      if (log10_total_infectious_denstiy == ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY) {
        log10_total_infectious_denstiy = parasites_[i]->get_log10_infectious_density();
      } else {
        log10_total_infectious_denstiy +=
            log10(pow(10, parasites_[i]->get_log10_infectious_density() - log10_total_infectious_denstiy) + 1);
      }
    }
  }
}

void SingleHostClonalParasitePopulations::update_by_drugs(DrugsInBlood* drugs_in_blood) const {
  for (auto& blood_parasite : parasites_) {
    auto* new_genotype = blood_parasite->genotype();

    double percent_parasite_remove = 0;
    for (auto& [drug_id, drug] : *drugs_in_blood->drugs()) {
      // select all locus
      // remember to use mask to turn on and off mutation location
      // for a specific time
      Genotype* candidate_genotype = new_genotype->perform_mutation_by_drug(
        Model::get_config(),
        Model::get_random(),
        drug->drug_type(),
        Model::get_config()->get_genotype_parameters().get_mutation_probability_per_locus());

        if (candidate_genotype->get_EC50_power_n(drug->drug_type())
            > new_genotype->get_EC50_power_n(drug->drug_type())) {
          // higher EC50^n means lower efficacy then allow mutation occur
          new_genotype = candidate_genotype;
        }
        if (new_genotype != blood_parasite->genotype()) {
          // if(blood_parasite->genotype()->get_aa_sequence()[35] == 'C'
          //    && new_genotype->get_aa_sequence()[35] == 'Y'){
          //   spdlog::info("580 C -> Y");
          // }
          // if(new_genotype->get_aa_sequence()[35] == 'Y'){
          //     spdlog::info("new 580Y {} -> {}",blood_parasite->genotype()->aa_sequence,new_genotype->aa_sequence);
          // }
          // mutation occurs
          Model::get_mdc()->record_1_mutation(person_->get_location(), blood_parasite->genotype(), new_genotype);
          Model::get_mdc()->record_1_mutation_by_drug(person_->get_location(), blood_parasite->genotype(), new_genotype, drug_id);

          //          LOG(TRACE) << Model::get_scheduler()->current_time() << "\t" << blood_parasite->genotype()->genotype_id()
          //          << "\t"
          //                     << new_genotype->genotype_id() << "\t"
          //                     << blood_parasite->genotype()->get_EC50_power_n(drug->drug_type()) << "\t"
          //                     << new_genotype->get_EC50_power_n(drug->drug_type());
          blood_parasite->set_genotype(new_genotype);
        }


      const auto p_temp = drug->get_parasite_killing_rate(blood_parasite->genotype()->genotype_id());
      percent_parasite_remove = percent_parasite_remove + p_temp - percent_parasite_remove * p_temp;
    }
    if (percent_parasite_remove > 0) {
      blood_parasite->perform_drug_action(percent_parasite_remove);
    }
  }
}

bool SingleHostClonalParasitePopulations::has_detectable_parasite() const {
  for (auto& parasite : parasites_) {
    if (parasite->last_update_log10_parasite_density()
        >= Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_detectable_pfpr()) {
      return true;
    }
  }
  return false;
}

bool SingleHostClonalParasitePopulations::is_gametocytaemic() const {
  for (auto& parasite : parasites_) {
    if (parasite->gametocyte_level() > 0) {
      return true;
    }
  }
  return false;
}
