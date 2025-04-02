#include "ChangeTreatmentCoverageEvent.h"

#include "Core/Scheduler/Scheduler.h"
#include "Utils/Helpers/ObjectHelpers.h"
#include "Simulation/Model.h"
#include "Treatment/LinearTCM.h"
#include "Utils/Helpers/StringHelpers.h"

ChangeTreatmentCoverageEvent::ChangeTreatmentCoverageEvent(
    ITreatmentCoverageModel* tcm) {
  set_time(tcm->starting_time);
  treatment_coverage_model = tcm;
}

ChangeTreatmentCoverageEvent::~ChangeTreatmentCoverageEvent() {
  ObjectHelpers::delete_pointer(treatment_coverage_model);
}

void ChangeTreatmentCoverageEvent::do_execute() {
spdlog::info("{}: change treatment coverage model to {}",
             StringHelpers::date_as_string(date::year_month_day{Model::get_scheduler()->calendar_date}),
             treatment_coverage_model->type);
  Model::get_instance().set_treatment_coverage(treatment_coverage_model);

  if (auto* linear_tcm = dynamic_cast<LinearTCM*>(treatment_coverage_model)) {
    linear_tcm->update_rate_of_change();
  }
  treatment_coverage_model = nullptr;
  set_executable(false);
}
