#ifndef TURNONMUTATIONEVENT_H
#define TURNONMUTATIONEVENT_H

#include "Events/Event.h"
#include <string>

class TurnOnMutationEvent : public WorldEvent {
public:
  // Disallow copy
  TurnOnMutationEvent(const TurnOnMutationEvent&) = delete;
  TurnOnMutationEvent& operator=(const TurnOnMutationEvent&) = delete;

  // Disallow move
  TurnOnMutationEvent(TurnOnMutationEvent&&) = delete;
  TurnOnMutationEvent& operator=(TurnOnMutationEvent&&) = delete;

  explicit TurnOnMutationEvent(const int& at_time, const double& mutation_probability);
  ~TurnOnMutationEvent() override = default;

  [[nodiscard]] const std::string name() const override {
    return "TurnOnMutationEvent";
  }

  double mutation_probability{0.0};
  int drug_id{-1};

private:
  void do_execute() override;
};

#endif // TURNONMUTATIONEVENT_H
