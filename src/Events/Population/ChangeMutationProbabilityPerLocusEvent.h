#ifndef POMS_ChangeMutationProbabilityPerLocusEVENT_H
#define POMS_ChangeMutationProbabilityPerLocusEVENT_H

#include "Events/Event.h"
#include "Core/Scheduler/Scheduler.h"
#include <string>
#include <vector>
#include "Simulation/Model.h"

class ChangeMutationProbabilityPerLocusEvent : public WorldEvent {
public:
    //disallow copy, assign and move
    ChangeMutationProbabilityPerLocusEvent(const ChangeMutationProbabilityPerLocusEvent&) = delete;
    void operator=(const ChangeMutationProbabilityPerLocusEvent&) = delete;
    ChangeMutationProbabilityPerLocusEvent(ChangeMutationProbabilityPerLocusEvent&&) = delete;
    void operator=(ChangeMutationProbabilityPerLocusEvent&&) = delete;

public:
    double value {0.001};
public:
    explicit ChangeMutationProbabilityPerLocusEvent(const double &value = 0.0, const int &at_time = -1);

    ~ChangeMutationProbabilityPerLocusEvent() override = default;

    const std::string name() const override {
        return "ChangeMutationProbabilityPerLocusEvent";
    }

private:
    void do_execute() override;
};


#endif //POMS_ChangeMutationProbabilityPerLocusEVENT_H
