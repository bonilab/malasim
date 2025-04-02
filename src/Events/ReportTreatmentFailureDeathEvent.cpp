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

void ReportTreatmentFailureDeathEvent::schedule_event(Scheduler* scheduler,
                                                      Person* person,
                                                      const int &therapy_id,
                                                      const int &time) {
  // Ensure that the scheduler exists
  assert(scheduler != nullptr);

  // Create the event to be added to the queue
  auto* event = new ReportTreatmentFailureDeathEvent();
  event->time = time;
  event->set_age_class(person->get_age_class());
  event->set_location_id(person->get_location());
  event->set_therapy_id(therapy_id);
  //scheduler->schedule_individual_event(event);
}

void ReportTreatmentFailureDeathEvent::do_execute() {
  Model::get_mdc()->record_1_treatment_failure_by_therapy(
      location_id(), age_class(), therapy_id());
}
