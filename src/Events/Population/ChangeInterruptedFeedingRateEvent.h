#ifndef POMS_CHANGEINTERRUPTEDFEEDINGRATEEVENT_H
#define POMS_CHANGEINTERRUPTEDFEEDINGRATEEVENT_H

#include "Events/Event.h"
#include "Core/Scheduler/Scheduler.h"
#include <string>
#include <vector>
#include "Simulation/Model.h"

class ChangeInterruptedFeedingRateEvent : public WorldEvent {
public:
    // Disallow copy
    ChangeInterruptedFeedingRateEvent(const ChangeInterruptedFeedingRateEvent&) = delete;
    ChangeInterruptedFeedingRateEvent& operator=(const ChangeInterruptedFeedingRateEvent&) = delete;

    // Disallow move
    ChangeInterruptedFeedingRateEvent(ChangeInterruptedFeedingRateEvent&&) = delete;
    ChangeInterruptedFeedingRateEvent& operator=(ChangeInterruptedFeedingRateEvent&&) = delete;

    explicit ChangeInterruptedFeedingRateEvent(const int &location = -1, const double &ifr = 0.0, const int &at_time = -1);
    ~ChangeInterruptedFeedingRateEvent() override = default;

    [[nodiscard]] const std::string name() const override {
        return "ChangeInterruptedFeedingRateEvent";
    }

    int location{-1};
    double ifr{0.0};

private:
    void do_execute() override;
};

#endif  // POMS_CHANGEINTERRUPTEDFEEDINGRATEEVENT_H
