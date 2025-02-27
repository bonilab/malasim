#define NOMINMAX

#include "SingleRoundMDAEvent.h"
#include "Simulation/Model.h"
#include "date/date.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Utils/Random.h"
#include "Events/ReceiveMDATherapyEvent.h"
#include "Utils/Helpers/StringHelpers.h"

SingleRoundMDAEvent::SingleRoundMDAEvent(const int &execute_at) {
  time = execute_at;
}

void SingleRoundMDAEvent::execute() {
  spdlog::info("{}: executing Single Round MDA",
    StringHelpers::date_as_string(date::year_month_day{scheduler->calendar_date}));

  // for all location
  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    // step 1: get number of individuals for MDA
    auto pi_lsa = Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();
    std::vector<Person *> all_persons_in_location;
    for (auto hs = 0; hs < Person::DEAD; hs++) {
      for (auto ac = 0; ac < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
        for (auto p : pi_lsa->vPerson()[loc][hs][ac]) {
          all_persons_in_location.push_back(p);
        }
      }
    }

    const auto number_indidividuals_in_location = all_persons_in_location.size();
    auto number_of_individuals_will_receive_mda = Model::get_instance().get_random()->random_poisson(
        fraction_population_targeted[loc]*number_indidividuals_in_location);

    number_of_individuals_will_receive_mda =
        number_of_individuals_will_receive_mda > number_indidividuals_in_location
        ? number_indidividuals_in_location
        : number_of_individuals_will_receive_mda;
    //shuffle app_persons_in_location index for sampling without replacement
    Model::get_instance().get_random()->shuffle(all_persons_in_location);

    for (auto p_i = 0; p_i < number_of_individuals_will_receive_mda; p_i++) {
      auto p = all_persons_in_location[p_i];
      //step 2: determine whether person will receive treatment
      const auto prob = Model::get_instance().get_random()->random_flat(0.0, 1.0);
      if (prob < p->prob_present_at_mda()) {
        // receive MDA
        auto *therapy = Model::get_instance().get_config()->get_therapy_parameters()
        .therapy_db[Model::get_instance().get_config()->get_strategy_parameters().get_mda().get_mda_therapy_id()];
        // schedule received therapy in within days_to_complete_all_treatments
        int days_to_receive_mda_therapy = Model::get_instance().get_random()->random_uniform(days_to_complete_all_treatments) + 1;
        ReceiveMDATherapyEvent::schedule_event(Model::get_instance().get_scheduler(), p, therapy,
                                               Model::get_instance().get_scheduler()->current_time() +
                                                   days_to_receive_mda_therapy);

      }
    }
  }

}
