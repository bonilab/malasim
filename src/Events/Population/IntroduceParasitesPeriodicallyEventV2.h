#ifndef POMS_SRC_EVENTS_POPULATION_INTRODUCEPARASITESPERIODICALLYEVENTV2_H
#define POMS_SRC_EVENTS_POPULATION_INTRODUCEPARASITESPERIODICALLYEVENTV2_H


// #include "Core/ObjectPool.h"
#include "Events/Event.h"

class IntroduceParasitesPeriodicallyEventV2  : public Event {
public:
  //disallow copy and assign
  IntroduceParasitesPeriodicallyEventV2(const IntroduceParasitesPeriodicallyEventV2&) = delete;
  void operator=(const IntroduceParasitesPeriodicallyEventV2&) = delete;
  IntroduceParasitesPeriodicallyEventV2(IntroduceParasitesPeriodicallyEventV2&&) = delete;
  void operator=(IntroduceParasitesPeriodicallyEventV2&&) = delete;

  // OBJECTPOOL(IntroduceParasitesPeriodicallyEventV2)
private:
  int location_;
  int duration_;
  int number_of_cases_;
public:
  int location(){return location_;}
  void set_location(int location){location_ = location;}
  int duration(){return duration_;}
  void set_duration(int duration){duration_ = duration;}
  int number_of_cases(){return number_of_cases_;}
  void set_number_of_cases(int number_of_cases){number_of_cases_ = number_of_cases;}

public:
  std::vector<std::vector<double>> allele_distributions;
  int start_day;
  int end_day;

public:
  IntroduceParasitesPeriodicallyEventV2( const std::vector<std::vector<double>> & allele_distributions_in = std::vector<std::vector<double>>(),
                                         const int &location = -1, const int &duration = -1,
                                        const int &number_of_cases = -1, const int &start_day_in = -1,
                                         const int & end_day_in = -1);

  //    ImportationEvent(const ImportationEvent& orig);
  virtual ~IntroduceParasitesPeriodicallyEventV2();

  static void schedule_event(Scheduler *scheduler, IntroduceParasitesPeriodicallyEventV2* old_event);

  const std::string name() const override {
    return "IntroduceParasitesPeriodicallyEventV2";
  }

private:
  void do_execute() override;

};


#endif //POMS_SRC_EVENTS_POPULATION_INTRODUCEPARASITESPERIODICALLYEVENTV2_H
