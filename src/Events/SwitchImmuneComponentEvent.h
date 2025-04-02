#ifndef SWITCHIMMUNECOMPONENTEVENT_H
#define    SWITCHIMMUNECOMPONENTEVENT_H

#include "Event.h"
//#include "Core/ObjectPool.h"

class Scheduler;

class Person;

class SwitchImmuneComponentEvent : public Event {
// OBJECTPOOL(SwitchImmuneComponentEvent)

 public:
  SwitchImmuneComponentEvent();

  SwitchImmuneComponentEvent(const SwitchImmuneComponentEvent &orig);

  virtual ~SwitchImmuneComponentEvent();

  static void schedule_for_switch_immune_component_event(Scheduler *scheduler, Person *p, const int &time);

  virtual const std::string name() const {
    return "SwitchImmuneComponentEvent";
  }

 private:
  virtual void do_execute();
};

#endif    /* SWITCHIMMUNECOMPONENTEVENT_H */

