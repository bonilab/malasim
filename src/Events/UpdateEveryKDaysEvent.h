#ifndef UPDATEEVERYKDAYSEVENT_H
#define UPDATEEVERYKDAYSEVENT_H

//#include "Core/ObjectPool.h"
#include "Event.h"

class Person;

class UpdateEveryKDaysEvent : public Event {
public:
  // disallow copy and move
  UpdateEveryKDaysEvent(const UpdateEveryKDaysEvent&) = delete;
  UpdateEveryKDaysEvent(UpdateEveryKDaysEvent&&) = delete;

//  DELETE_COPY_AND_MOVE(UpdateEveryKDaysEvent)

//  OBJECTPOOL(UpdateEveryKDaysEvent)

public:
  UpdateEveryKDaysEvent();

  //    UpdateEveryKDaysEvent(const UpdateEveryKDaysEvent& orig);
  virtual ~UpdateEveryKDaysEvent();

  static void schedule_event(Scheduler* scheduler, Person* p, const int &time);

  const std::string name() const override { return "UpdateEveryKDaysEvent"; }

private:
  void do_execute() override;
};

#endif /* UPDATEEVERYKDAYSEVENT_H */
