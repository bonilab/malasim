#include "ReceiveTherapyEvent.h"
#include "Population/Person/Person.h"
#include "Core/Scheduler/Scheduler.h"
#include "Treatment/Therapies/Therapy.h"
#include "Population/ClonalParasitePopulation.h"

ReceiveTherapyEvent::ReceiveTherapyEvent() : received_therapy_(nullptr), clinical_caused_parasite_(nullptr) {}

ReceiveTherapyEvent::~ReceiveTherapyEvent() = default;

void ReceiveTherapyEvent::do_execute() {
  auto *person = dynamic_cast<Person *>(event_manager);
  //    if (person->is_in_external_population()) {
  //        return;
  //    }

  person->receive_therapy(received_therapy_, clinical_caused_parasite_, is_part_of_MAC_therapy);

  person->schedule_update_by_drug_event(clinical_caused_parasite_);
}
