#ifndef TURNONMUTATIONEVENT_H
#define TURNONMUTATIONEVENT_H

#include "Events/Event.h"
#include <string>

class TurnOnMutationEvent : public Event {
public:
  // Disable copy and assignment and move
  TurnOnMutationEvent(const TurnOnMutationEvent&) = delete;
  void operator=(const TurnOnMutationEvent&) = delete;
  TurnOnMutationEvent(TurnOnMutationEvent&&) = delete;
  void operator=(TurnOnMutationEvent&&) = delete;

  double mutation_probability = 0.0;
  int drug_id = -1;

public:
  explicit TurnOnMutationEvent(const int &at_time, const double &mutation_probability);

  ~TurnOnMutationEvent() override = default;

  const std::string name() const override {
    return "TurnOnMutationEvent";
  }

private:
  void do_execute() override;
};

#endif // TURNONMUTATIONEVENT_H
