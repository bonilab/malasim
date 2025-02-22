#ifndef BIRTHDAYEVENT_H
#define    BIRTHDAYEVENT_H

#include "Event.h"
#include <string>

class Person;

class BirthdayEvent : public Event {
 // OBJECTPOOL(BirthdayEvent);
 public:
  // Disable copy and assignment
  BirthdayEvent(const BirthdayEvent&) = delete;
  void operator=(const BirthdayEvent&) = delete;

  BirthdayEvent();

  //    BirthdayEvent(const BirthdayEvent& orig);
  virtual ~BirthdayEvent();

  std::string name() override;

 static void schedule_event(Scheduler* scheduler, Person* person, const int &time);
 private:
  void execute() override;
};

#endif    /* BIRTHDAYEVENT_H */
