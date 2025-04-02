/*
 * CirculateToTargetLocationNextDayEvent.h
 *
 * Define the event to move the individual to the next location.
 */
#ifndef CIRCULATETOTARGETLOCATIONNEXTDAYEVENT_H
#define CIRCULATETOTARGETLOCATIONNEXTDAYEVENT_H

//#include "Core/ObjectPool.h"
#include "Event.h"

class Person;
class Scheduler;

class CirculateToTargetLocationNextDayEvent : public Event {
public:
   //disallow copy and move
  CirculateToTargetLocationNextDayEvent(const CirculateToTargetLocationNextDayEvent&) = delete;
  CirculateToTargetLocationNextDayEvent& operator=(const CirculateToTargetLocationNextDayEvent&) = delete;
  CirculateToTargetLocationNextDayEvent(CirculateToTargetLocationNextDayEvent&&) = delete;
  CirculateToTargetLocationNextDayEvent& operator=(CirculateToTargetLocationNextDayEvent&&) = delete;

//  OBJECTPOOL(CirculateToTargetLocationNextDayEvent)
private:
  int target_location_;
public:
  int target_location() const { return target_location_; }
  void set_target_location(int value) { target_location_ = value; }

public:
  CirculateToTargetLocationNextDayEvent() : target_location_(0) {}
  ~CirculateToTargetLocationNextDayEvent() override = default;

  static void schedule_event(Scheduler* scheduler, Person* p,
                             const int &target_location, const int &time);
  const std::string name() const override { return "CirculateToTargetLocationNextDayEvent"; }

private:
  void do_execute() override;
};

#endif
