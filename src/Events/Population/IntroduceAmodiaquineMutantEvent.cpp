#include "IntroduceAmodiaquineMutantEvent.h"

#include <spdlog/spdlog.h>

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Parasites/Genotype.h"
#include "Utils/Random.h"
#include "Simulation/Model.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Utils/Helpers/StringHelpers.h"

IntroduceAmodiaquineMutantEvent::IntroduceAmodiaquineMutantEvent(const int &location,
                                               const int &execute_at,
                                               const double &fraction,
                                               const std::vector<std::tuple<int,int,char>> &alleles) :
location_(location),fraction_(fraction), alleles_(alleles){
  time = execute_at;
}

IntroduceAmodiaquineMutantEvent::~IntroduceAmodiaquineMutantEvent() = default;

void IntroduceAmodiaquineMutantEvent::execute() {
  auto* pi =
      Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();

  for (std::size_t j = 0; j < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ++j) {
    const auto number_infected_individual_in_ac =
        pi->vPerson()[0][Person::ASYMPTOMATIC][j].size()
        + pi->vPerson()[0][Person::CLINICAL][j].size();
    const auto number_of_importation_cases = Model::get_instance().get_random()->random_poisson(
        number_infected_individual_in_ac * fraction_);
    if (number_of_importation_cases == 0) continue;
    for (auto i = 0; i < number_of_importation_cases; i++) {
      const auto index =
          Model::get_instance().get_random()->random_uniform(number_infected_individual_in_ac);

      Person* p = nullptr;
      if (index < pi->vPerson()[0][Person::ASYMPTOMATIC][j].size()) {
        p = pi->vPerson()[0][Person::ASYMPTOMATIC][j][index];
      } else {
        p = pi->vPerson()[0][Person::CLINICAL][j]
                         [index
                          - pi->vPerson()[0][Person::ASYMPTOMATIC][j].size()];
      }

      for (auto* pp : *(p->get_all_clonal_parasite_populations()->parasites())) {
        auto* old_genotype = pp->genotype();
        auto* new_genotype = old_genotype->modify_genotype_allele(alleles_,Model::get_instance().get_config());
        pp->set_genotype(new_genotype);
      }
    }
  }

  spdlog::info("{}: Introduce Amodiaquine mutant event",
              StringHelpers::date_as_string(
                  date::year_month_day{Model::get_instance().get_scheduler()->calendar_date}));
}
