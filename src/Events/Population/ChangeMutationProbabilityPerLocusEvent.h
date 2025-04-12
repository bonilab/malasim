#ifndef POMS_ChangeMutationProbabilityPerLocusEVENT_H
#define POMS_ChangeMutationProbabilityPerLocusEVENT_H

#include "Events/Event.h"
#include "Core/Scheduler/Scheduler.h"
#include <string>
#include <vector>
#include "Simulation/Model.h"

class ChangeMutationProbabilityPerLocusEvent : public WorldEvent {
public:
    // Disallow copy
    ChangeMutationProbabilityPerLocusEvent(const ChangeMutationProbabilityPerLocusEvent&) = delete;
    ChangeMutationProbabilityPerLocusEvent& operator=(const ChangeMutationProbabilityPerLocusEvent&) = delete;

    // Disallow move
    ChangeMutationProbabilityPerLocusEvent(ChangeMutationProbabilityPerLocusEvent&&) = delete;
    ChangeMutationProbabilityPerLocusEvent& operator=(ChangeMutationProbabilityPerLocusEvent&&) = delete;

    explicit ChangeMutationProbabilityPerLocusEvent(const double &value = 0.0, const int &at_time = -1);
    ~ChangeMutationProbabilityPerLocusEvent() override = default;

    [[nodiscard]] const std::string name() const override {
        return "ChangeMutationProbabilityPerLocusEvent";
    }

    double value{0.001};

private:
    void do_execute() override;
};

#endif //POMS_ChangeMutationProbabilityPerLocusEVENT_H
