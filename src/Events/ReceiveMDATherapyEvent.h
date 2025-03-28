#ifndef RECEIVEMDADRUGEVENT_H
#define RECEIVEMDADRUGEVENT_H

#include <string>

//#include "Core/PropertyMacro.h"
#include "Event.h"

class Scheduler;

class Person;

class Therapy;

class ReceiveMDATherapyEvent : public Event {
public:
  //disable copy and move
  ReceiveMDATherapyEvent(const ReceiveMDATherapyEvent&) = delete;
  ReceiveMDATherapyEvent(ReceiveMDATherapyEvent&&) = delete;
//  DELETE_COPY_AND_MOVE(ReceiveMDATherapyEvent)

//  POINTER_PROPERTY(Therapy, received_therapy)
private:
  Therapy* received_therapy_;
public:
  Therapy* received_therapy() { return received_therapy_; }
  void set_received_therapy(Therapy* value) { received_therapy_ = value; }

public:
  ReceiveMDATherapyEvent();

  //    ReceiveMDADrugEvent(const ReceiveMDADrugEvent& orig);
  virtual ~ReceiveMDATherapyEvent();

  static void schedule_event(Scheduler* scheduler, Person* p, Therapy* therapy,
                             const int &time);

  std::string name() override { return "ReceiveMDADrugEvent"; }

private:
  void execute() override;
};

#endif /* RECEIVEMDADRUGEVENT_H */
