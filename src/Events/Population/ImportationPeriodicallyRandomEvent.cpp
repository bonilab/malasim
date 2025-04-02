/*
 * ImportationPeriodicallyRandomEvent.cpp
 *
 * Implement the event.
 */
#include "ImportationPeriodicallyRandomEvent.h"

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "Simulation/Model.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Population/Population.h"
#include "Utils/Helpers/StringHelpers.h"
#include "Utils/Index/PersonIndexAll.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"

void ImportationPeriodicallyRandomEvent::do_execute() {
  // Start by finding the number of infections to inflict
  auto date =
      static_cast<date::year_month_day>(Model::get_scheduler()->calendar_date);
  auto days = TimeHelpers::days_in_month(
      static_cast<int>(date.year()), static_cast<unsigned int>(date.month()));
  auto infections = Model::get_random()->random_poisson((double)count_ / days);

  // Inflict the infections
  for (auto ndx = 0; ndx < infections; ndx++) {
    // Get the location and person index
    auto location = get_location();
    auto* pi = Model::get_population()
                   ->get_person_index<PersonIndexByLocationStateAgeClass>();

    // Get the age classes for the susceptible individuals
    unsigned long age_class;
    do {
      age_class = Model::get_random()->random_uniform(static_cast<unsigned long>(
          pi->vPerson()[location][Person::HostStates::SUSCEPTIBLE].size()));
    } while (pi->vPerson()[location][0][age_class].empty());

    // Get the individual
    unsigned long index = Model::get_random()->random_uniform(
        pi->vPerson()[location][Person::HostStates::SUSCEPTIBLE][age_class]
            .size());
    auto* person = pi->vPerson()[location][Person::HostStates::SUSCEPTIBLE]
                                [age_class][index];

    // Inflict the infection
    infect(person, genotypeId_);

    // Log on demand
    spdlog::debug("{} - Introduced infection at {}",
                  StringHelpers::date_as_string(date::year_month_day{Model::get_scheduler()->calendar_date}),
                  location);
  }

  // Minimum schedule update is one day
  auto time = Model::get_scheduler()->current_time() + 1;
  if (static_cast<unsigned int>(date.day()) == days) {
    // If it is the last day of the month, schedule for the first of the month,
    // next year
    auto nextRun = date::year_month_day(date.year() + date::years{1},
                                        date.month(), date::day{1});
    time = (date::sys_days{nextRun}
            - date::sys_days{Model::get_config()->get_simulation_timeframe().get_starting_date()})
               .count();
  }

  // Generate and schedule the event
  auto* event = new ImportationPeriodicallyRandomEvent(
      genotypeId_, time, count_, log_parasite_density_);
  Model::get_scheduler()->schedule_population_event(event);
}

// The following is based upon a fitness proportionate selection (roulette wheel
// selection)
std::size_t ImportationPeriodicallyRandomEvent::get_location() {
  // Note the common values
  auto locations = Model::get_config()->number_of_locations();
  auto* pi =
      Model::get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();
  auto age_classes = pi->vPerson()[0][Person::HostStates::SUSCEPTIBLE].size();

  // Start by finding the total susceptible population
  auto population = 0ul;
  for (std::size_t location = 0; location < locations; location++) {
    for (std::size_t age_class = 0; age_class < age_classes; age_class++) {
      population +=
          pi->vPerson()[location][Person::HostStates::SUSCEPTIBLE][age_class]
              .size();
    }
  }

  // Get a random pull [0, population - 1]
  auto target = Model::get_random()->random_uniform<int>(0, population - 1);

  // Find the index of location which has a cumulative sum less than the target
  // value
  for (std::size_t location = 0; location < locations; location++) {
    // Note we have to check for each age class since we don't know exactly how
    // many individuals are in a given location otherwise
    for (std::size_t age_class = 0; age_class < age_classes; age_class++) {
      if (target
          < pi->vPerson()[location][Person::HostStates::SUSCEPTIBLE][age_class]
                .size()) {
        return location;
      }
      target -=
          pi->vPerson()[location][Person::HostStates::SUSCEPTIBLE][age_class]
              .size();
    }
  }

  // If all goes well we shouldn't reach this point
  spdlog::error(
      "Failed to find a random location for genotype import, target: {}",target);
  throw std::runtime_error(
      "Failed to find a random location for genotype import");
}

void ImportationPeriodicallyRandomEvent::infect(Person* person,
                                                int genotypeId) const {
  // Prepare the immune system
  person->get_immune_system()->set_increase(true);
  person->set_host_state(Person::ASYMPTOMATIC);

  // Prepare the genotype
  Genotype* genotype = Model::get_config()->get_genotype_parameters().genotype_db->at(genotypeId);

  // Inflict the infection
  auto* blood_parasite = person->add_new_parasite_to_blood(genotype);
  blood_parasite->set_gametocyte_level(Model::get_config()->get_epidemiological_parameters().get_gametocyte_level_full());
  blood_parasite->set_last_update_log10_parasite_density(log_parasite_density_);
  blood_parasite->set_update_function(Model::get_instance().immunity_clearance_update_function());

  // Check if the configured log density is equal to or greater than the
  // standard for clinical
  if (log_parasite_density_ >= Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_clinical()) {
    blood_parasite->set_update_function(Model::get_instance().progress_to_clinical_update_function());
    person->schedule_progress_to_clinical_event(blood_parasite);
  }
}
