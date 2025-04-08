#include "ReceiveTherapyEvent.h"

#include "Population/Person/Person.h"

void ReceiveTherapyEvent::do_execute() {
  auto* person = get_person();
  if (person == nullptr) { throw std::runtime_error("Person is nullptr"); }

  person->receive_therapy(received_therapy_, clinical_caused_parasite_, is_part_of_mac_therapy_);

  person->schedule_update_by_drug_event(clinical_caused_parasite_);
}
