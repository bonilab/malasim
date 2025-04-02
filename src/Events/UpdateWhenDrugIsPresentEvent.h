#ifndef UPDATEWHENDRUGISPRESENTEVENT_H
#define    UPDATEWHENDRUGISPRESENTEVENT_H

#include "Event.h"
// #include "Core/ObjectPool.h"
// #include "Core/PropertyMacro.h"
#include <string>

class ClonalParasitePopulation;

class Scheduler;

class Person;

class UpdateWhenDrugIsPresentEvent : public PersonEvent {
public:
  //disallow copy and assign
  UpdateWhenDrugIsPresentEvent(const UpdateWhenDrugIsPresentEvent &) = delete;
  void operator=(const UpdateWhenDrugIsPresentEvent &) = delete;
  UpdateWhenDrugIsPresentEvent(UpdateWhenDrugIsPresentEvent &&) = delete;
  void operator=(UpdateWhenDrugIsPresentEvent &&) = delete;

// OBJECTPOOL(UpdateWhenDrugIsPresentEvent)

private:
    ClonalParasitePopulation *clinical_caused_parasite_ = nullptr;
public:
    ClonalParasitePopulation *clinical_caused_parasite() { return clinical_caused_parasite_; }
    void set_clinical_caused_parasite(ClonalParasitePopulation *value) { clinical_caused_parasite_ = value; }

 public:
  UpdateWhenDrugIsPresentEvent(Person* person) : PersonEvent(person), clinical_caused_parasite_(nullptr) {}

  //    UpdateByHavingDrugEvent(const UpdateByHavingDrugEvent& orig);
  virtual ~UpdateWhenDrugIsPresentEvent() = default;

  const std::string name() const override {
    return "UpdateByHavingDrugEvent";
  }

 private:
  void do_execute() override;
};

#endif    /* UPDATEWHENDRUGISPRESENTEVENT_H */
