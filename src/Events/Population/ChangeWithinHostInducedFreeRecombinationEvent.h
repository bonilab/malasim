#ifndef POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
#define POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H

#include "Events/Event.h"

class ChangeWithinHostInducedFreeRecombinationEvent : public WorldEvent {
public:
    // Disallow copy
    ChangeWithinHostInducedFreeRecombinationEvent(const ChangeWithinHostInducedFreeRecombinationEvent&) = delete;
    ChangeWithinHostInducedFreeRecombinationEvent& operator=(const ChangeWithinHostInducedFreeRecombinationEvent&) = delete;

    // Disallow move
    ChangeWithinHostInducedFreeRecombinationEvent(ChangeWithinHostInducedFreeRecombinationEvent&&) = delete;
    ChangeWithinHostInducedFreeRecombinationEvent& operator=(ChangeWithinHostInducedFreeRecombinationEvent&&) = delete;

    explicit ChangeWithinHostInducedFreeRecombinationEvent(const bool &value = false, const int &at_time = -1);
    ~ChangeWithinHostInducedFreeRecombinationEvent() override = default;

    [[nodiscard]] const std::string name() const override {
        return "ChangeWithinHostInducedRecombinationEvent";
    }

    bool value{true};

private:
    void do_execute() override;
};

#endif  // POMS_CHANGEWITHINHOSTINDUCEDFREERECOMBINATIONEVENT_H
