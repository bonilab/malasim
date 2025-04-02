#ifndef UPDATEWHENDRUGISPRESENTEVENT_H
#define    UPDATEWHENDRUGISPRESENTEVENT_H

#include "Event.h"
// #include "Core/ObjectPool.h"
// #include "Core/PropertyMacro.h"
#include <string>

class ClonalParasitePopulation;

class Scheduler;

class Person;

class UpdateWhenDrugIsPresentEvent : public Event {
public:
  //disallow copy and assign
  UpdateWhenDrugIsPresentEvent(const UpdateWhenDrugIsPresentEvent &) = delete;
  void operator=(const UpdateWhenDrugIsPresentEvent &) = delete;

// OBJECTPOOL(UpdateWhenDrugIsPresentEvent)

private:
    ClonalParasitePopulation *clinical_caused_parasite_ = nullptr;
public:
    ClonalParasitePopulation *clinical_caused_parasite() { return clinical_caused_parasite_; }
    void set_clinical_caused_parasite(ClonalParasitePopulation *value) { clinical_caused_parasite_ = value; }

 public:
  UpdateWhenDrugIsPresentEvent();

  //    UpdateByHavingDrugEvent(const UpdateByHavingDrugEvent& orig);
  virtual ~UpdateWhenDrugIsPresentEvent();

  static void schedule_event(Scheduler *scheduler, Person *p, ClonalParasitePopulation *clinical_caused_parasite,
                             const int &time);

  const std::string name() const override {
    return "UpdateByHavingDrugEvent";
  }

 private:
  void do_execute() override;
};

#endif    /* UPDATEWHENDRUGISPRESENTEVENT_H */
