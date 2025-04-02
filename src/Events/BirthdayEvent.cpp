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

  const auto days_to_next_year =
      TimeHelpers::number_of_days_to_next_year(Model::get_scheduler()->calendar_date);

  schedule_event(Model::get_scheduler(), person,
                 Model::get_scheduler()->current_time() + days_to_next_year);
}

void BirthdayEvent::schedule_event(Scheduler* scheduler, Person* p,
                                   const int &time) {
  if (scheduler != nullptr) {
    auto* birthday_event = new BirthdayEvent();
    birthday_event->dispatcher = p;
    birthday_event->time = time;
    p->add_event(birthday_event);
  }
}
