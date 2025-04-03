#include "BirthdayEvent.h"

#include <cassert>

#include "Core/Scheduler/Scheduler.h"
#include "Population/Person/Person.h"
#include "Simulation/Model.h"

// OBJECTPOOL_IMPL(BirthdayEvent)

void BirthdayEvent::do_execute() {
  auto* person = get_person();
  assert(person != nullptr);
  person->increase_age_by_1_year();

  const auto days_to_next_birthday =
      Model::get_scheduler()->get_days_to_next_year();

  person->schedule_birthday_event(days_to_next_birthday);
}
