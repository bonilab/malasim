#ifndef POMS_SRC_EVENTS_POPULATION_INTRODUCETRIPLEMUTANTTODPMEVENT_H
#define POMS_SRC_EVENTS_POPULATION_INTRODUCETRIPLEMUTANTTODPMEVENT_H

//#include "Core/ObjectPool.h"
#include "Events/Event.h"
#include <string>

class IntroduceTrippleMutantToDPMEvent : public Event {
public:
  //disallow copy, assign and move
  IntroduceTrippleMutantToDPMEvent(const IntroduceTrippleMutantToDPMEvent &) = delete;
  void operator=(const IntroduceTrippleMutantToDPMEvent &) = delete;
  IntroduceTrippleMutantToDPMEvent(IntroduceTrippleMutantToDPMEvent &&) = delete;
  void operator=(IntroduceTrippleMutantToDPMEvent &&) = delete;

//OBJECTPOOL(IntroduceTrippleMutantToDPMEvent)

private:
  int location_;
    double fraction_;
    std::vector<std::tuple<int,int,char>> alleles_;

public:
  explicit IntroduceTrippleMutantToDPMEvent(
  const int& location = -1, const int& execute_at = -1,
                                        const double &fraction = 0,
                                        const std::vector<std::tuple<int,int,char>> &allele_map = {});

  //    ImportationEvent(const ImportationEvent& orig);
  ~IntroduceTrippleMutantToDPMEvent() override;

  const std::string name() const override {
    return "IntroduceTrippleMutantToDPMEvent";
  }

private:
  void do_execute() override;

};


#endif //POMS_SRC_EVENTS_POPULATION_INTRODUCETRIPLEMUTANTTODPMEVENT_H
