#ifndef CHANGETREATMENTCOVERAGEEVENT_H
#define CHANGETREATMENTCOVERAGEEVENT_H

#include <memory>

#include "Events/Event.h"
#include "Treatment/ITreatmentCoverageModel.h"

class ChangeTreatmentCoverageEvent : public WorldEvent {
public:
  std::unique_ptr<ITreatmentCoverageModel> treatment_coverage_model;

  ChangeTreatmentCoverageEvent(const ChangeTreatmentCoverageEvent &) = delete;
  ChangeTreatmentCoverageEvent(ChangeTreatmentCoverageEvent &&) = delete;
  ChangeTreatmentCoverageEvent &operator=(const ChangeTreatmentCoverageEvent &) = delete;
  ChangeTreatmentCoverageEvent &operator=(ChangeTreatmentCoverageEvent &&) = delete;
  explicit ChangeTreatmentCoverageEvent(std::unique_ptr<ITreatmentCoverageModel> tcm);

  ~ChangeTreatmentCoverageEvent() override;

  [[nodiscard]] const std::string name() const override { return "ChangeTreatmentCoverageEvent"; }

private:
  void do_execute() override;
};

#endif  // CHANGETREATMENTCOVERAGEEVENT_H
