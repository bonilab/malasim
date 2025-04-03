#ifndef CHANGETREATMENTCOVERAGEEVENT_H
#define CHANGETREATMENTCOVERAGEEVENT_H

#include "Events/Event.h"
#include "Treatment/ITreatmentCoverageModel.h"

class ChangeTreatmentCoverageEvent : public WorldEvent {
public:
  ITreatmentCoverageModel* treatment_coverage_model;

  explicit ChangeTreatmentCoverageEvent(ITreatmentCoverageModel* tcm);

  virtual ~ChangeTreatmentCoverageEvent();

  const std::string name() const override { return "ChangeTreatmentCoverageEvent"; }

private:
  void do_execute() override;
};

#endif  // CHANGETREATMENTCOVERAGEEVENT_H
