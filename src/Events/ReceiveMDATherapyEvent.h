#ifndef RECEIVEMDADRUGEVENT_H
#define RECEIVEMDADRUGEVENT_H

#include <string>

//#include "Core/PropertyMacro.h"
#include "Event.h"

class Scheduler;

class Person;

class Therapy;

class ReceiveMDATherapyEvent : public PersonEvent {
public:
  //disable copy and move
  ReceiveMDATherapyEvent(const ReceiveMDATherapyEvent&) = delete;
  ReceiveMDATherapyEvent(ReceiveMDATherapyEvent&&) = delete;
//  DELETE_COPY_AND_MOVE(ReceiveMDATherapyEvent)

//  POINTER_PROPERTY(Therapy, received_therapy)
private:
  Therapy* received_therapy_{nullptr};
public:
  Therapy* received_therapy() { return received_therapy_; }
  void set_received_therapy(Therapy* value) { received_therapy_ = value; }

public:
  ReceiveMDATherapyEvent(Person* person) : PersonEvent(person), received_therapy_(nullptr) {}

  //    ReceiveMDADrugEvent(const ReceiveMDADrugEvent& orig);
  virtual ~ReceiveMDATherapyEvent() = default;

  const std::string name() const override { return "ReceiveMDADrugEvent"; }

private:
  void do_execute() override;
};

#endif /* RECEIVEMDADRUGEVENT_H */
