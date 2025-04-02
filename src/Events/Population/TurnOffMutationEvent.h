#ifndef TURNOFFMUTATIONEVENT_H
#define TURNOFFMUTATIONEVENT_H

#include "Events/Event.h"
#include <string>

class TurnOffMutationEvent : public Event {
public:
  // Disable copy and assignment and move
  TurnOffMutationEvent(const TurnOffMutationEvent&) = delete;
  void operator=(const TurnOffMutationEvent&) = delete;
  TurnOffMutationEvent(TurnOffMutationEvent&&) = delete;
  void operator=(TurnOffMutationEvent&&) = delete;

 public:
  explicit TurnOffMutationEvent(const int &at_time);

  virtual ~TurnOffMutationEvent() = default;

  const std::string name() const override {
    return "TurnOffMutationEvent";
  }

 private:
  void do_execute() override;
};

#endif // TURNOFFMUTATIONEVENT_H
