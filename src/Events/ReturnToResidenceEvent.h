#ifndef RETURNTORESIDENCEEVENT_H
#define RETURNTORESIDENCEEVENT_H

//#include "Core/ObjectPool.h"
#include "Event.h"

class Person;
class Scheduler;

class ReturnToResidenceEvent : public PersonEvent {
public:
  //disallow copy and move
  ReturnToResidenceEvent(const ReturnToResidenceEvent&) = delete;
  ReturnToResidenceEvent(ReturnToResidenceEvent&&) = delete;

//  DELETE_COPY_AND_MOVE(ReturnToResidenceEvent)

//  OBJECTPOOL(ReturnToResidenceEvent)

public:
  ReturnToResidenceEvent(Person* person) : PersonEvent(person) {}
  virtual ~ReturnToResidenceEvent() = default;

  const std::string name() const override { return "ReturnToResidenceEvent"; }

private:
  void do_execute() override;
};

#endif
