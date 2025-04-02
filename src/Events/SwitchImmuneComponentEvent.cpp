#include "SwitchImmuneComponentEvent.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Population/Person/Person.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/ImmuneSystem/NonInfantImmuneComponent.h"
#include <cassert>

//OBJECTPOOL_IMPL(SwitchImmuneComponentEvent)

SwitchImmuneComponentEvent::SwitchImmuneComponentEvent() = default;

SwitchImmuneComponentEvent::~SwitchImmuneComponentEvent() = default;

void SwitchImmuneComponentEvent::do_execute() {

  assert(dispatcher!=nullptr);
  auto *p = dynamic_cast<Person *>(dispatcher);
  p->get_immune_system()->set_immune_component(new NonInfantImmuneComponent());

}

void SwitchImmuneComponentEvent::schedule_for_switch_immune_component_event(Scheduler *scheduler, Person *p,
                                                                            const int &time) {
  if (scheduler!=nullptr) {
    auto *e = new SwitchImmuneComponentEvent();
    e->dispatcher = p;
    e->time = time;
    p->add_event(e);
    //scheduler->schedule_individual_event(e);
  }
}
