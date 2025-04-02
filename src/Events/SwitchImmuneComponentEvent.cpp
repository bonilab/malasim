#include "SwitchImmuneComponentEvent.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Population/Person/Person.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/ImmuneSystem/NonInfantImmuneComponent.h"
#include <cassert>

//OBJECTPOOL_IMPL(SwitchImmuneComponentEvent)

SwitchImmuneComponentEvent::SwitchImmuneComponentEvent(Person* person) 
    : PersonEvent(person) {
    assert(person != nullptr);
}

SwitchImmuneComponentEvent::~SwitchImmuneComponentEvent() = default;

void SwitchImmuneComponentEvent::do_execute() {
    assert(person_ != nullptr);
    person_->get_immune_system()->set_immune_component(new NonInfantImmuneComponent());
}


