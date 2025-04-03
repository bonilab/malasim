/*
 * ReturnToResidenceEvent.cpp
 *
 * Implement the event to return the individual to their original location.
 */
#include "ReturnToResidenceEvent.h"

#include "Core/Scheduler/Scheduler.h"
#include "Population/Person/Person.h"
#include "Population/Population.h"
#include "Simulation/Model.h"

// OBJECTPOOL_IMPL(ReturnToResidenceEvent)

void ReturnToResidenceEvent::do_execute() {
  auto* person = get_person();
  if (person == nullptr) { throw std::runtime_error("Person is nullptr"); }
  auto source_location = person->get_location();
  person->set_location(person->get_residence_location());
  Model::get_population()->notify_movement(source_location,
                                           person->get_residence_location());
}
