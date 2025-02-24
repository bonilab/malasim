/*
 * CirculateToTargetLocationNextDayEvent.cpp
 *
 * Implement the event to move the individual to the next location.
 */
#include "CirculateToTargetLocationNextDayEvent.h"

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"
#include "ReturnToResidenceEvent.h"
#include "Population/Population.h"

//OBJECTPOOL_IMPL(CirculateToTargetLocationNextDayEvent)

void CirculateToTargetLocationNextDayEvent::schedule_event(
    Scheduler* scheduler, Person* p, const int &target_location,
    const int &time) {
  if (scheduler != nullptr) {
    auto* e = new CirculateToTargetLocationNextDayEvent();
    e->dispatcher = p;
    e->set_target_location(target_location);
    e->time = time;

    p->add_dispatcher(e);
    scheduler->schedule_individual_event(e);
  }
}

std::string CirculateToTargetLocationNextDayEvent::name() {
  return "CirculateToTargetLocationNextDayEvent";
}

void CirculateToTargetLocationNextDayEvent::execute() {
  // Get the person and perform the movement
  auto* person = dynamic_cast<Person*>(dispatcher);
  auto source_location = person->get_location();
  person->set_location(target_location_);

  // Notify the population of the change
  Model::get_instance().get_population()->notify_movement(source_location, target_location_);

  // Did we randomly arrive back at the residence location?
  if (target_location_ == person->get_residence_location()) {
    // Cancel any outstanding return trips and return
    person->cancel_all_return_to_residence_events();
    return;
  }

  // If the person already ahs a return trip scheduled then we can return
  if (person->has_return_to_residence_event()) { return; }

  // Since a return is not scheduled, we need to create a return event based
  // upon a gamma distribution
  auto length_of_trip = 0;
  while (length_of_trip < 1) {
    length_of_trip = static_cast<int>(std::round(Model::get_instance().get_random()->random_gamma(
        Model::get_instance().get_config()->get_movement_settings().get_length_of_stay_theta(),
        Model::get_instance().get_config()->get_movement_settings().get_length_of_stay_k())));
  }
  ReturnToResidenceEvent::schedule_event(
      Model::get_instance().get_scheduler(), person,
      Model::get_instance().get_scheduler()->current_time() + length_of_trip);
}
