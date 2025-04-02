#include "ReceiveTherapyEvent.h"
#include "Population/Person/Person.h"
#include "Core/Scheduler/Scheduler.h"
#include "Treatment/Therapies/Therapy.h"
#include "Population/ClonalParasitePopulation.h"


void ReceiveTherapyEvent::do_execute() {
  auto *person = get_person();
  if (person == nullptr) {
    throw std::runtime_error("Person is nullptr");
  }
  
  person->receive_therapy(received_therapy_, clinical_caused_parasite_, is_part_of_MAC_therapy);

  person->schedule_update_by_drug_event(clinical_caused_parasite_);
}
