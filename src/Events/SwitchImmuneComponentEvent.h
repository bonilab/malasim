#ifndef SWITCH_IMMUNE_COMPONENT_EVENT_H
#define SWITCH_IMMUNE_COMPONENT_EVENT_H

#include "Event.h"
//#include "Core/ObjectPool.h"

class Scheduler;

class Person;

class SwitchImmuneComponentEvent : public PersonEvent {
// OBJECTPOOL(SwitchImmuneComponentEvent)

 public:
  SwitchImmuneComponentEvent(Person* person);

  ~SwitchImmuneComponentEvent() override;

  const std::string name() const override {
    return "SwitchImmuneComponentEvent";
  }

 protected:
  void do_execute() override;

 private:
  Person* person_;  // Store direct reference to person
};

#endif    /* SWITCH_IMMUNE_COMPONENT_EVENT_H */

