#ifndef SWITCH_IMMUNE_COMPONENT_EVENT_H
#define SWITCH_IMMUNE_COMPONENT_EVENT_H

#include "Event.h"
// #include "Core/ObjectPool.h"

class Scheduler;

class Person;

class SwitchImmuneComponentEvent : public PersonEvent {
  // OBJECTPOOL(SwitchImmuneComponentEvent)
public:
  SwitchImmuneComponentEvent(const SwitchImmuneComponentEvent &) = delete;
  SwitchImmuneComponentEvent(SwitchImmuneComponentEvent &&) = delete;
  SwitchImmuneComponentEvent &operator=(const SwitchImmuneComponentEvent &) = delete;
  SwitchImmuneComponentEvent &operator=(SwitchImmuneComponentEvent &&) = delete;

  explicit SwitchImmuneComponentEvent(Person* person);

  ~SwitchImmuneComponentEvent() override;

  [[nodiscard]] const std::string name() const override { return "SwitchImmuneComponentEvent"; }

protected:
  void do_execute() override;
};

#endif /* SWITCH_IMMUNE_COMPONENT_EVENT_H */

