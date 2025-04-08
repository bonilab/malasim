#ifndef UPDATEWHENDRUGISPRESENTEVENT_H
#define UPDATEWHENDRUGISPRESENTEVENT_H

#include "Event.h"
// #include "Core/ObjectPool.h"
// #include "Core/PropertyMacro.h"
#include <string>

class ClonalParasitePopulation;

class Scheduler;

class Person;

class UpdateWhenDrugIsPresentEvent : public PersonEvent {
  // OBJECTPOOL(UpdateWhenDrugIsPresentEvent)
public:
  // disallow copy and assign
  UpdateWhenDrugIsPresentEvent(const UpdateWhenDrugIsPresentEvent &) = delete;
  void operator=(const UpdateWhenDrugIsPresentEvent &) = delete;
  UpdateWhenDrugIsPresentEvent(UpdateWhenDrugIsPresentEvent &&) = delete;
  void operator=(UpdateWhenDrugIsPresentEvent &&) = delete;

  explicit UpdateWhenDrugIsPresentEvent(Person* person) : PersonEvent(person) {}

  ~UpdateWhenDrugIsPresentEvent() override = default;

  [[nodiscard]] const std::string name() const override { return "UpdateByHavingDrugEvent"; }

  ClonalParasitePopulation* clinical_caused_parasite() { return clinical_caused_parasite_; }
  void set_clinical_caused_parasite(ClonalParasitePopulation* value) {
    clinical_caused_parasite_ = value;
  }

private:
  ClonalParasitePopulation* clinical_caused_parasite_{nullptr};

  void do_execute() override;
};

#endif /* UPDATEWHENDRUGISPRESENTEVENT_H */
