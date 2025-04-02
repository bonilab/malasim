/*
 * RotateStrategyEvent.h
 *
 * Define the event class to rotate treatment strategies on a regular basis.
 */
#ifndef ROTATE_STRATEGY_EVENT_H
#define ROTATE_STRATEGY_EVENT_H

// #include "Core/ObjectPool.h"
// #include "Core/PropertyMacro.h"
#include "Events/Event.h"

class RotateStrategyEvent : public Event {
private:
  int new_strategy_id_;
  int next_strategy_id_;
    int years_;

private:
  void do_execute() override;

public:
  inline static const std::string EventName = "rotate_treatment_strategy_event";

  RotateStrategyEvent(int at_time, int years, int current_strategy_id,
                      int next_strategy_id);
  ~RotateStrategyEvent() override = default;

  const std::string name() const override { return EventName; }
};

#endif
