#ifndef POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
#define POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
#include "Events/Event.h"

class ChangeWithinHostInducedFreeRecombinationEvent : public WorldEvent {
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

  const std::string name() const override {
    return "ChangeWithinHostInducedRecombinationEvent";
  }

private:
  void do_execute() override;
};

#endif  // POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
