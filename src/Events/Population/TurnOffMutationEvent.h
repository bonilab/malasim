#ifndef TURNOFFMUTATIONEVENT_H
#define TURNOFFMUTATIONEVENT_H

#include "Events/Event.h"
#include <string>

class TurnOffMutationEvent : public WorldEvent {
public:
    // Disallow copy
    TurnOffMutationEvent(const TurnOffMutationEvent&) = delete;
    TurnOffMutationEvent& operator=(const TurnOffMutationEvent&) = delete;

    // Disallow move
    TurnOffMutationEvent(TurnOffMutationEvent&&) = delete;
    TurnOffMutationEvent& operator=(TurnOffMutationEvent&&) = delete;

    explicit TurnOffMutationEvent(const int& at_time = -1);
    ~TurnOffMutationEvent() override = default;

    [[nodiscard]] const std::string name() const override {
        return "TurnOffMutationEvent";
    }

private:
    void do_execute() override;
};

#endif // TURNOFFMUTATIONEVENT_H
