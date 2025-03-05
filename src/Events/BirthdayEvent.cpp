#include "BirthdayEvent.h"
#include <Core/Scheduler/Scheduler.h>
#include <spdlog/spdlog.h>
#include "Population/Person/Person.h"
#include "Utils/Helpers/TimeHelpers.h"

// OBJECTPOOL_IMPL(BirthdayEvent)

BirthdayEvent::BirthdayEvent() = default;

BirthdayEvent::~BirthdayEvent() = default;

void BirthdayEvent::execute() {
  assert(dispatcher!=nullptr);
  auto* person = dynamic_cast<Person *>(dispatcher);
  person->increase_age_by_1_year();
  schedule_event(scheduler, person, scheduler->current_time()
    + TimeHelpers::number_of_days_to_next_year(scheduler->calendar_date));
}

void BirthdayEvent::schedule_event(Scheduler* scheduler, Person *person, const int &time) {
  if (scheduler!=nullptr) {
    auto* birthday_event = new BirthdayEvent();
    birthday_event->scheduler = scheduler;
    birthday_event->set_id(Model::get_random()->uuid());
    birthday_event->time = time;
    birthday_event->executable = true;
    person->add_dispatcher(birthday_event);
    birthday_event->dispatcher = person;
    // person->add_dispatcher(birthday_event);
  }
}

std::string BirthdayEvent::name() {
  return "Birthday Event " + get_id();
}
