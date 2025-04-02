#ifndef MATUREGAMETOCYTEEVENT_H
#define MATUREGAMETOCYTEEVENT_H

// #include "Core/ObjectPool.h"
// #include "Core/PropertyMacro.h"
#include "Event.h"

class ClonalParasitePopulation;

class Scheduler;

class Person;

class MatureGametocyteEvent : public PersonEvent {
public:
  //disallow copy, assign and move
  MatureGametocyteEvent(const MatureGametocyteEvent&) = delete;
  void operator=(const MatureGametocyteEvent&) = delete;
  MatureGametocyteEvent(MatureGametocyteEvent&&) = delete;
  void operator=(MatureGametocyteEvent&&) = delete;

  // OBJECTPOOL(MatureGametocyteEvent)
private:
  ClonalParasitePopulation* blood_parasite_;
public:
  ClonalParasitePopulation* blood_parasite() { return blood_parasite_; }
  void set_blood_parasite(ClonalParasitePopulation* value) { blood_parasite_ = value; }

public:
  MatureGametocyteEvent(Person* person) : PersonEvent(person) {}

  //    MatureGametocyteEvent(const MatureGametocyteEvent& orig);
  virtual ~MatureGametocyteEvent() = default;

  const std::string name() const override { return "MatureGametocyteEvent"; }

private:
  void do_execute() override;
};

#endif /* MATUREGAMETOCYTEEVENT_H */
