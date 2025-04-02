#ifndef POMS_CHANGEINTERRUPTEDFEEDINGRATEEVENT_H
#define POMS_CHANGEINTERRUPTEDFEEDINGRATEEVENT_H

#include "Events/Event.h"
#include "Core/Scheduler/Scheduler.h"
#include <string>
#include <vector>
#include "Simulation/Model.h"

class ChangeInterruptedFeedingRateEvent : public Event {
public:
  //disallow copy, assign and move
  ChangeInterruptedFeedingRateEvent(const ChangeInterruptedFeedingRateEvent&) = delete;
  void operator=(const ChangeInterruptedFeedingRateEvent&) = delete;
  ChangeInterruptedFeedingRateEvent(ChangeInterruptedFeedingRateEvent&&) = delete;
  void operator=(ChangeInterruptedFeedingRateEvent&&) = delete;

public:
  int location {-1};
  double ifr {0.0};
public:
  explicit ChangeInterruptedFeedingRateEvent(const int &location = -1, const double &ifr = 0.0, const int &at_time = -1);

  ~ChangeInterruptedFeedingRateEvent() override = default;

  const std::string name() const override {
    return "ChangeInterruptedFeedingRateEvent";
  }

private:
  void do_execute() override;
};

#endif  // POMS_CHANGEINTERRUPTEDFEEDINGRATEEVENT_H
