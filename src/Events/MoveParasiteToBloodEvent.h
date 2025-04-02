#ifndef MOVEPARASITETOBLOODEVENT_H
#define MOVEPARASITETOBLOODEVENT_H

#include <string>

//#include "Core/ObjectPool.h"
//#include "Core/PropertyMacro.h"
#include "Event.h"

class ClonalParasitePopulation;

class Scheduler;

class Person;

class Genotype;

class MoveParasiteToBloodEvent : public Event {
public:
  //disallow copy and assign and move
  MoveParasiteToBloodEvent(const MoveParasiteToBloodEvent &orig) = delete;
  void operator=(const MoveParasiteToBloodEvent &orig) = delete;
  MoveParasiteToBloodEvent(MoveParasiteToBloodEvent &&orig) = delete;
  void operator=(MoveParasiteToBloodEvent &&orig) = delete;

//  OBJECTPOOL(MoveParasiteToBloodEvent)
private:
  Genotype* infection_genotype_;
public:
  Genotype* infection_genotype(){return infection_genotype_;}
  void set_infection_genotype(Genotype* infection_genotype) {
    infection_genotype_ = infection_genotype;
  }

public:
  MoveParasiteToBloodEvent();

  //    MoveParasiteToBloodEvent(const MoveParasiteToBloodEvent& orig);
  virtual ~MoveParasiteToBloodEvent();

  static void schedule_event(Scheduler* scheduler, Person* p,
                             Genotype* infection_type, const int &time);

  const std::string name() const override { return "MoveParasiteToBloodEvent"; }

private:
  void do_execute() override;
};

#endif /* MOVEPARASITETOBLOODEVENT_H */
