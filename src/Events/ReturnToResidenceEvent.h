#ifndef RETURNTORESIDENCEEVENT_H
#define RETURNTORESIDENCEEVENT_H

// #include "Core/ObjectPool.h"
#include "Event.h"

class Person;
class Scheduler;

class ReturnToResidenceEvent : public PersonEvent {
  //  OBJECTPOOL(ReturnToResidenceEvent)
public:
  ReturnToResidenceEvent &operator=(const ReturnToResidenceEvent &) = delete;
  ReturnToResidenceEvent &operator=(ReturnToResidenceEvent &&) = delete;
  // disallow copy and move
  ReturnToResidenceEvent(const ReturnToResidenceEvent &) = delete;
  ReturnToResidenceEvent(ReturnToResidenceEvent &&) = delete;

  explicit ReturnToResidenceEvent(Person* person) : PersonEvent(person) {}
  ~ReturnToResidenceEvent() override = default;

  [[nodiscard]] const std::string name() const override { return "ReturnToResidenceEvent"; }

private:
  void do_execute() override;
};

#endif
