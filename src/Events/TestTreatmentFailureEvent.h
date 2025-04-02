/*
 * TestTreatmentFailureEvent.cpp
 *
 * Defines the event that tests to see if the treatment given to a patient
 * failed or not.
 */
#ifndef TESTTREATMENTFAILUREEVENT_H
#define TESTTREATMENTFAILUREEVENT_H

//#include "Core/ObjectPool.h"
//#include "Core/PropertyMacro.h"
#include "Event.h"

class ClonalParasitePopulation;

class Scheduler;

class Person;

class TestTreatmentFailureEvent : public Event {
public:
  //disallow copy, assign and move
  TestTreatmentFailureEvent(const TestTreatmentFailureEvent&) = delete;
  void operator=(const TestTreatmentFailureEvent&) = delete;
  TestTreatmentFailureEvent(TestTreatmentFailureEvent&&) = delete;
  void operator=(TestTreatmentFailureEvent&&) = delete;

//  OBJECTPOOL(TestTreatmentFailureEvent)
private:
    int therapy_id_;
    ClonalParasitePopulation* clinical_caused_parasite_;
public:
    ClonalParasitePopulation* clinical_caused_parasite() { return clinical_caused_parasite_; }
    void set_clinical_caused_parasite(ClonalParasitePopulation* value) { clinical_caused_parasite_ = value; }
    int therapy_id() { return therapy_id_; }
    void set_therapy_id(int value) { therapy_id_ = value; }

public:
  TestTreatmentFailureEvent();
  ~TestTreatmentFailureEvent() override = default;

  const std::string name() const override { return "TestTreatmentFailureEvent"; }

private:
  void do_execute() override;
};

#endif
