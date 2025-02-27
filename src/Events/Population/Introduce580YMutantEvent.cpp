
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Configuration//Config.h"
#include "Utils/Random.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Introduce580YMutantEvent.h"

#include "Parasites/Genotype.h"
#include "Utils/Helpers/StringHelpers.h"

Introduce580YMutantEvent::Introduce580YMutantEvent(const int &location, const int &execute_at,
                                                   const double &fraction,
                                                   const std::vector<std::tuple<int,int,char>> &alleles) :
location_(location),fraction_(fraction), alleles_(alleles)
                                                                             {
  time = execute_at;
}

Introduce580YMutantEvent::~Introduce580YMutantEvent() = default;

void Introduce580YMutantEvent::execute() {
  // TODO: rework on this


  auto* pi = Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();

  // get the approximate current frequency of 580Y in the population
  // and only fill up the different between input fraction and the current frequency

  double current_580Y_fraction = 0.0;
  double total_population_count = 0;
  for (int j = 0; j < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ++j) {
    for (Person* p :  pi->vPerson()[0][Person::ASYMPTOMATIC][j]) {
      total_population_count += p->get_all_clonal_parasite_populations()->size();
      for (ClonalParasitePopulation* pp : *p->get_all_clonal_parasite_populations()->parasites()) {
//        if (pp->genotype()->aa_structure()[2] == 1) {
//          current_580Y_fraction++;
//        }
      }
    }
  }

  current_580Y_fraction = total_population_count == 0 ? 0 : current_580Y_fraction / total_population_count;
  double target_fraction = fraction_ - current_580Y_fraction;
  if (target_fraction <= 0) {
    spdlog::info("{}: Introduce 580Y Copy event with 0 cases",
                StringHelpers::date_as_string(
                    date::year_month_day{Model::get_instance().get_scheduler()->calendar_date}));
    return;
  }
//  std::cout << target_fraction << std::endl;

  for (int j = 0; j < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ++j) {
    const auto number_infected_individual_in_ac =
      pi->vPerson()[0][Person::ASYMPTOMATIC][j].size() + pi->vPerson()[0][Person::CLINICAL][j].size();
    const auto number_of_importation_cases = Model::get_instance().get_random()->random_poisson(
      number_infected_individual_in_ac * target_fraction);
    if (number_of_importation_cases == 0)
      continue;
    for (auto i = 0; i < number_of_importation_cases; i++) {

      const size_t index = Model::get_instance().get_random()->random_uniform(number_infected_individual_in_ac);

      Person* p = nullptr;
      if (index < pi->vPerson()[0][Person::ASYMPTOMATIC][j].size()) {
        p = pi->vPerson()[0][Person::ASYMPTOMATIC][j][index];
      } else {
        p = pi->vPerson()[0][Person::CLINICAL][j][index - pi->vPerson()[0][Person::ASYMPTOMATIC][j].size()];
      }

      //mutate all clonal populations
      for (auto* pp : *(p->get_all_clonal_parasite_populations()->parasites())) {
        auto* old_genotype = pp->genotype();
        auto* new_genotype = old_genotype->modify_genotype_allele(alleles_,Model::get_instance().get_config());
        pp->set_genotype(new_genotype);
      }
    }
  }

  spdlog::info("{}: Introduce 580Y mutant event with fraction {}",
              StringHelpers::date_as_string(
                  date::year_month_day{Model::get_instance().get_scheduler()->calendar_date}),
                  target_fraction);
}
