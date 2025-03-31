#include "DistrictImportationDailyEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Events/Event.h"
#include "Parasites/Genotype.h"
#include "Simulation/Model.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"

DistrictImportationDailyEvent::DistrictImportationDailyEvent(
    int district, double dailyRate, int startDay, const std::vector<std::tuple<int,int,char>> &alleles)
    : district_(district),
      daily_rate_(dailyRate), alleles_(alleles){
  time = startDay;
}

void DistrictImportationDailyEvent::schedule_event(Scheduler* scheduler,
                                                   int district,
                                                   double dailyRate,
                                                   int startDay,
                                                   const std::vector<std::tuple<int,int,char>> &alleles) {
  if (scheduler != nullptr) {
    auto* event = new DistrictImportationDailyEvent(
        district, dailyRate, startDay,alleles);
    event->dispatcher = nullptr;
    event->time = startDay;
    scheduler->schedule_population_event(event);
  }
}

void DistrictImportationDailyEvent::execute() {
  // std::cout << date::year_month_day{ Model::get_scheduler()->calendar_date } <<
  // ":import periodically event" << std::endl;
  // schedule importation for the next day
  schedule_event(Model::get_scheduler(), district_,
                 daily_rate_, Model::get_scheduler()->current_time() + 1,alleles_);

  auto number_of_importation_cases = Model::get_random()->random_poisson(daily_rate_);

  if (number_of_importation_cases == 0) { return; }

  const auto &locations = SpatialData::get_instance().get_locations_in_unit("district", district_);

  auto* pi =
      Model::get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();

  std::vector<double> infected_cases_by_location(locations.size(), 0);

  for (auto i = 0; i < locations.size(); i++) {
    auto location = locations[i];

    for (auto ac = 0; ac < Model::get_config()->number_of_age_classes(); ac++) {
      // only select state clinical or asymptomatic
      infected_cases_by_location[i] +=
          pi->vPerson()[location][Person::ASYMPTOMATIC][ac].size()
          + pi->vPerson()[location][Person::CLINICAL][ac].size();
    }
  }
  // use multinomial distribution to distribute the number of importation cases
  // to the locations
  std::vector<uint> importation_cases_by_location(locations.size(), 0);
  Model::get_random()->random_multinomial(
      locations.size(), number_of_importation_cases,
      infected_cases_by_location, importation_cases_by_location);
  for (auto i = 0; i < locations.size(); i++) {
    if (infected_cases_by_location[i] == 0) { continue; }
    if (importation_cases_by_location[i] == 0) { continue; }
    auto location = locations[i];
    auto number_of_importation_cases = importation_cases_by_location[i];

    for (auto i = 0; i < number_of_importation_cases; i++) {
      auto ac =
          Model::get_random()->random_uniform(Model::get_config()->number_of_age_classes());
      auto hs = Model::get_random()->random_uniform(2) + Person::ASYMPTOMATIC;

      auto max_retry = 10;
      while (pi->vPerson()[location][hs][ac].empty() && max_retry > 0) {
        // redraw if the selected state and age class is empty
        ac = Model::get_random()->random_uniform(
            Model::get_config()->number_of_age_classes());

        hs = Model::get_random()->random_uniform(2) + Person::ASYMPTOMATIC;
        max_retry--;
      }
      if (max_retry == 0) { continue; }

      auto index = Model::get_random()->random_uniform(
          static_cast<unsigned long>(pi->vPerson()[location][hs][ac].size()));

      auto* person = pi->vPerson()[location][hs][ac][index];

      // Mutate all the clonal populations the individual is carrying
      for (auto& pp : *person->get_all_clonal_parasite_populations()) {
        auto* old_genotype = pp->genotype();
        auto* new_genotype =
            old_genotype->modify_genotype_allele(alleles_, Model::get_config());
        pp->set_genotype(new_genotype);
      }
    }
  }
}

