#ifndef CHANGESTRATEGYEVENT_H
#define CHANGESTRATEGYEVENT_H

#include <string>

#include "Events/Event.h"

class ChangeTreatmentStrategyEvent : public WorldEvent {
public:
  //disallow copy and move
  ChangeTreatmentStrategyEvent(const ChangeTreatmentStrategyEvent&) = delete;
  void operator=(const ChangeTreatmentStrategyEvent&) = delete;
  ChangeTreatmentStrategyEvent(ChangeTreatmentStrategyEvent&&) = delete;
  void operator=(ChangeTreatmentStrategyEvent&&) = delete;

public:
  int strategy_id{-1};

  ChangeTreatmentStrategyEvent(const int &at_time, const int &strategy_id);

  virtual ~ChangeTreatmentStrategyEvent() = default;

  const std::string name() const override { return "ChangeStrategyEvent"; }

private:
  void do_execute() override;
};

#endif  // CHANGESTRATEGYEVENT_H
