#ifndef BIRTHDAYEVENT_H
#define BIRTHDAYEVENT_H

#include <string>

// #include "Core/ObjectPool.h"
#include "Event.h"

class Person;

class BirthdayEvent : public PersonEvent {
public:
  //disallow copy and assignment
  BirthdayEvent(const BirthdayEvent& other) = delete;
  void operator=(const BirthdayEvent& other) = delete;
  BirthdayEvent(BirthdayEvent&& other) = delete;
  BirthdayEvent& operator=(BirthdayEvent&& other) = delete;

  // OBJECTPOOL(BirthdayEvent)

  // DELETE_COPY_AND_MOVE(BirthdayEvent)

public:
  BirthdayEvent(Person* person) : PersonEvent(person) {}

  //    BirthdayEvent(const BirthdayEvent& orig);
  virtual ~BirthdayEvent() = default;

  const std::string name() const override { return "Birthday Event"; }

private:
  void do_execute() override;
};

#endif /* BIRTHDAYEVENT_H */
