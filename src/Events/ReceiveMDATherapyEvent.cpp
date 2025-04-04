#include "ReceiveMDATherapyEvent.h"

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Population/ImmuneSystem/ImmunityClearanceUpdateFunction.h"
#include "Treatment/ITreatmentCoverageModel.h"

void ReceiveMDATherapyEvent::do_execute() {
  auto* person = get_person();
  if (person == nullptr) {
    throw std::runtime_error("Person is nullptr");
  }
  //    if (person->is_in_external_population()) {
  //        return;
  //    }

  person->receive_therapy(received_therapy_, nullptr);

  // if this person has progress to clinical event then cancel it
  person->cancel_all_other_progress_to_clinical_events_except(nullptr);
  person->change_all_parasite_update_function(
      Model::get_instance()->progress_to_clinical_update_function(),
      Model::get_instance()->immunity_clearance_update_function());

  person->schedule_update_by_drug_event(nullptr);
}
