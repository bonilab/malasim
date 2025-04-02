#include "BirthdayEvent.h"

#include <cassert>

#include "Core/Scheduler/Scheduler.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "Population/Person/Person.h"
#include "Simulation/Model.h"

// OBJECTPOOL_IMPL(BirthdayEvent)

BirthdayEvent::BirthdayEvent() = default;

BirthdayEvent::~BirthdayEvent() = default;

void BirthdayEvent::do_execute() {
  assert(dispatcher != nullptr);
  auto* person = dynamic_cast<Person*>(dispatcher);
  person->increase_age_by_1_year();

  const auto days_to_next_birthday =
      TimeHelpers::number_of_days_to_next_year(Model::get_scheduler()->calendar_date);

  person->schedule_birthday_event(days_to_next_birthday);
}
