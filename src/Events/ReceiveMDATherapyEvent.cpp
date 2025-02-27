#include "ReceiveMDATherapyEvent.h"

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Population/ImmuneSystem/ImmunityClearanceUpdateFunction.h"
#include "Treatment/ITreatmentCoverageModel.h"

ReceiveMDATherapyEvent::ReceiveMDATherapyEvent() : received_therapy_(nullptr){};

ReceiveMDATherapyEvent::~ReceiveMDATherapyEvent() = default;

void ReceiveMDATherapyEvent::schedule_event(Scheduler* scheduler, Person* p,
                                            Therapy* therapy, const int &time) {
  if (scheduler != nullptr) {
    auto* e = new ReceiveMDATherapyEvent();
    e->dispatcher = p;
    e->set_received_therapy(therapy);
    e->time = time;

    p->add_dispatcher(e);
    scheduler->schedule_individual_event(e);
  }
}

void ReceiveMDATherapyEvent::execute() {
  auto* person = dynamic_cast<Person*>(dispatcher);
  //    if (person->is_in_external_population()) {
  //        return;
  //    }

  person->receive_therapy(received_therapy_, nullptr);

  // if this person has progress to clinical event then cancel it
  person->cancel_all_other_progress_to_clinical_events_except(nullptr);
  person->change_all_parasite_update_function(
      Model::get_instance().progress_to_clinical_update_function(),
      Model::get_instance().immunity_clearance_update_function());

  person->schedule_update_by_drug_event(nullptr);
}
