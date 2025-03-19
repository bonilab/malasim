#ifndef CHANGEMUTATIONMASKEVENT_H
#define CHANGEMUTATIONMASKEVENT_H


#include "Events/Event.h"
#include "Core/Scheduler/Scheduler.h"
#include <string>
#include <vector>
#include "Simulation/Model.h"

class ChangeMutationMaskEvent  : public Event {
public:
  //disallow copy, assign and move
  ChangeMutationMaskEvent(const ChangeMutationMaskEvent&) = delete;
  void operator=(const ChangeMutationMaskEvent&) = delete;
  ChangeMutationMaskEvent(ChangeMutationMaskEvent&&) = delete;
  void operator=(ChangeMutationMaskEvent&&) = delete;

public:
  std::string mask_;
public:
  explicit ChangeMutationMaskEvent(const std::string &mask, const int &at_time = -1);

  ~ChangeMutationMaskEvent() override = default;

  std::string name() override {
    return "ChangeMutationMaskEvent";
  }

private:
  void execute() override;
};

#endif //CHANGEMUTATIONMASKEVENT_H
