/*
 * ReportTreatmentFailureDeathEvent.cpp
 *
 * Implement the event to report that an individual died of malaria following
 * treatment.
 */
#include "ReportTreatmentFailureDeathEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "MDC/ModelDataCollector.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"

ReportTreatmentFailureDeathEvent::ReportTreatmentFailureDeathEvent()
    : age_class_(0), location_id_(0), therapy_id_(0) {}

ReportTreatmentFailureDeathEvent::~ReportTreatmentFailureDeathEvent() = default;

void ReportTreatmentFailureDeathEvent::do_execute() {
  Model::get_mdc()->record_1_treatment_failure_by_therapy(
      location_id(), age_class(), therapy_id());
}
