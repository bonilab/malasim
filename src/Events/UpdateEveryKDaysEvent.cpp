#include "UpdateEveryKDaysEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"

//OBJECTPOOL_IMPL(UpdateEveryKDaysEvent)

UpdateEveryKDaysEvent::UpdateEveryKDaysEvent() = default;

UpdateEveryKDaysEvent::~UpdateEveryKDaysEvent() = default;

void UpdateEveryKDaysEvent::schedule_event(Scheduler* scheduler, Person* p,
                                           const int &time) {
  if (scheduler != nullptr) {
    auto* e = new UpdateEveryKDaysEvent();
    e->dispatcher = p;
    e->time = time;

    p->add_dispatcher(e);
    scheduler->schedule_individual_event(e);
  }
}

void UpdateEveryKDaysEvent::execute() {
  auto* person = static_cast<Person*>(dispatcher);
  person->schedule_update_every_K_days_event(Model::get_instance().get_config()->get_epidemiological_parameters().get_update_frequency());
}
