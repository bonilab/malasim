/*
 * ReportTreatmentFailureDeathEvent.h
 *
 * Defines the event that reports that an individual died of malaria following
 * treatment.
 */
#ifndef REPORTTREATMENTFAILUREDEATHEVENT_H
#define REPORTTREATMENTFAILUREDEATHEVENT_H

// #include "Core/ObjectPool.h"
#include "Event.h"

class Person;
class Scheduler;

class ReportTreatmentFailureDeathEvent : public PersonEvent {
public:
  //disallow copy and move
  ReportTreatmentFailureDeathEvent(const ReportTreatmentFailureDeathEvent&) = delete;
  ReportTreatmentFailureDeathEvent(ReportTreatmentFailureDeathEvent&&) = delete;

  // OBJECTPOOL(ReportTreatmentFailureDeathEvent)

private:
  int age_class_{-1};
  int location_id_{-1};
  int therapy_id_{-1};
public:
  int age_class() const { return age_class_; }
  void set_age_class(int value) { age_class_ = value; }
  int location_id() const { return location_id_; }
  void set_location_id(int value) { location_id_ = value; }
  int therapy_id() const { return therapy_id_; }
  void set_therapy_id(int value) { therapy_id_ = value; }

public:
  ReportTreatmentFailureDeathEvent(Person* person) : PersonEvent(person), age_class_(0), location_id_(0), therapy_id_(0) {}
  ~ReportTreatmentFailureDeathEvent() override = default;

  const std::string name() const override { return "ReportTreatmentFailureDeathEvent"; }

private:
  void do_execute() override;
};

#endif
