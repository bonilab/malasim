#ifndef MOVEPARASITETOBLOODEVENT_H
#define MOVEPARASITETOBLOODEVENT_H

#include <string>

// #include "Core/ObjectPool.h"
// #include "Core/PropertyMacro.h"
#include "Event.h"

class ClonalParasitePopulation;

class Scheduler;

class Person;

class Genotype;

class MoveParasiteToBloodEvent : public PersonEvent {
  //  OBJECTPOOL(MoveParasiteToBloodEvent)
public:
  // Disallow copy
  MoveParasiteToBloodEvent(const MoveParasiteToBloodEvent&) = delete;
  MoveParasiteToBloodEvent& operator=(const MoveParasiteToBloodEvent&) = delete;

  // Disallow move
  MoveParasiteToBloodEvent(MoveParasiteToBloodEvent&&) = delete;
  MoveParasiteToBloodEvent& operator=(MoveParasiteToBloodEvent&&) = delete;

  explicit MoveParasiteToBloodEvent(Person* person) : PersonEvent(person) {}
  ~MoveParasiteToBloodEvent() override = default;

  [[nodiscard]] const std::string name() const override { return "MoveParasiteToBloodEvent"; }
  Genotype* infection_genotype() { return infection_genotype_; }
  void set_infection_genotype(Genotype* infection_genotype) {
    infection_genotype_ = infection_genotype;
  }

private:
  Genotype* infection_genotype_{nullptr};
  void do_execute() override;
};

#endif /* MOVEPARASITETOBLOODEVENT_H */
