/*
 * ReturnToResidenceEvent.cpp
 *
 * Implement the event to return the individual to their original location.
 */
#include "ReturnToResidenceEvent.h"
#include "Population/Person/Person.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/Population.h"

//OBJECTPOOL_IMPL(ReturnToResidenceEvent)

void ReturnToResidenceEvent::schedule_event(Scheduler* scheduler, Person* p,
                                            const int &time) {
  if (scheduler != nullptr) {
    auto* e = new ReturnToResidenceEvent();
    e->dispatcher = p;
    e->time = time;
    p->add_dispatcher(e);
    scheduler->schedule_individual_event(e);
  }
}

void ReturnToResidenceEvent::execute() {
  auto* person = dynamic_cast<Person*>(dispatcher);
  auto source_location = person->get_location();
  person->set_location(person->get_residence_location());
  Model::get_instance().get_population()->notify_movement(source_location,
                                     person->get_residence_location());
}
