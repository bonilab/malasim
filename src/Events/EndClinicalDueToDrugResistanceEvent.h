#ifndef ENDCLINICALDUETODRUGRESISTANCEEVENT_H
#define ENDCLINICALDUETODRUGRESISTANCEEVENT_H

//#include "Core/ObjectPool.h"
#include "Event.h"

class ClonalParasitePopulation;

class Scheduler;

class Person;

class EndClinicalDueToDrugResistanceEvent : public Event {
public:
  // Disable copy and move
  EndClinicalDueToDrugResistanceEvent(const EndClinicalDueToDrugResistanceEvent&) = delete;
  EndClinicalDueToDrugResistanceEvent(EndClinicalDueToDrugResistanceEvent&&) = delete;

//  OBJECTPOOL(EndClinicalDueToDrugResistanceEvent)

private:
  ClonalParasitePopulation* clinical_caused_parasite_;
public:
  ClonalParasitePopulation* clinical_caused_parasite() const {
    return clinical_caused_parasite_;
  }
  void set_clinical_caused_parasite(ClonalParasitePopulation* clinical_caused_parasite) {
      clinical_caused_parasite_ = clinical_caused_parasite;
  }

public:
  EndClinicalDueToDrugResistanceEvent();

  //    EndClinicalDueToDrugResistanceEvent(const
  //    EndClinicalDueToDrugResistanceEvent& orig);
  virtual ~EndClinicalDueToDrugResistanceEvent();

  static void schedule_event(Scheduler* scheduler, Person* p,
                             ClonalParasitePopulation* clinical_caused_parasite,
                             const int &time);

  std::string name() override { return "EndClinicalDueToDrugResistanceEvent"; }

private:
  void execute() override;
};

#endif /* ENDCLINICALDUETODRUGRESISTANCEEVENT_H */
