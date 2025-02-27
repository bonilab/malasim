#ifndef IMPORTATIONPERIODICALLYEVENT_H
#define    IMPORTATIONPERIODICALLYEVENT_H

//#include "Core/ObjectPool.h"
#include "Events/Event.h"

class ImportationPeriodicallyEvent : public Event {
public:
  //disallow copy and assign
  ImportationPeriodicallyEvent(const ImportationPeriodicallyEvent&) = delete;
  void operator=(const ImportationPeriodicallyEvent&) = delete;
  ImportationPeriodicallyEvent(ImportationPeriodicallyEvent&&) = delete;
  void operator=(ImportationPeriodicallyEvent&&) = delete;

// OBJECTPOOL(ImportationPeriodicallyEvent)
private:
  int location_;
  int duration_;
  int genotype_id_;
  int number_of_cases_;
public:
  int get_location() const {
    return location_;
  }
  void set_location(int location) {
    location_ = location;
  }
  int get_duration() const {
      return duration_;
  }
  void set_duration(int duration) {
      duration_ = duration;
  }
  int get_genotype_id() const {
      return genotype_id_;
  }
  void set_genotype_id(int genotype_id) {
      genotype_id_ = genotype_id;
  }
  int get_number_of_cases() const {
      return number_of_cases_;
  }
  void set_number_of_cases(int number_of_cases) {
      number_of_cases_ = number_of_cases;
  }

 public:
  ImportationPeriodicallyEvent(const int &location = -1, const int &duration = -1, int genotype_id = -1,
                               const int &number_of_cases = -1, const int &start_day = -1);

  //    ImportationEvent(const ImportationEvent& orig);
  virtual ~ImportationPeriodicallyEvent();

  static void schedule_event(Scheduler *scheduler, const int &location, const int &duration, unsigned int genotype_id,
                             const int &number_of_cases, const int &start_day);

  std::string name() override {
    return "ImportationPeriodicallyEvent";
  }

 private:
  void execute() override;

};

#endif    /* IMPORTATIONPERIODICALLYEVENT_H */
