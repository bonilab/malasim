#ifndef POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
#define POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
#include "Events/Event.h"

class ChangeWithinHostInducedFreeRecombinationEvent : public Event {
public:
  //disallow copy, assign and move
    ChangeWithinHostInducedFreeRecombinationEvent(const ChangeWithinHostInducedFreeRecombinationEvent&) = delete;
    void operator=(const ChangeWithinHostInducedFreeRecombinationEvent&) = delete;
    ChangeWithinHostInducedFreeRecombinationEvent(ChangeWithinHostInducedFreeRecombinationEvent&&) = delete;
    void operator=(ChangeWithinHostInducedFreeRecombinationEvent&&) = delete;
public:
  bool value {true};
public:
  explicit ChangeWithinHostInducedFreeRecombinationEvent(const bool &value = false, const int &at_time = -1);

  ~ChangeWithinHostInducedFreeRecombinationEvent() override = default;

  std::string name() override {
    return "ChangeWithinHostInducedRecombinationEvent";
  }

private:
  void execute() override;
};

#endif  // POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
