/*
 * File:   EndClinicalEvent.cpp
 * Author: Merlin
 *
 * Created on July 31, 2013, 12:27 PM
 */

#include "EndClinicalEvent.h"

#include "Core/Scheduler/Scheduler.h"
#include "Population/ClonalParasitePopulation.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Population/Person/Person.h"

//OBJECTPOOL_IMPL(EndClinicalEvent)

EndClinicalEvent::EndClinicalEvent() : clinical_caused_parasite_(nullptr) {}

EndClinicalEvent::~EndClinicalEvent() = default;

void EndClinicalEvent::do_execute() {
  auto person = dynamic_cast<Person*>(event_manager);

  if (person->get_all_clonal_parasite_populations()->size() == 0) {
    person->change_state_when_no_parasite_in_blood();

  } else {
    // still have parasite in blood
    person->get_immune_system()->set_increase(true);
    person->set_host_state(Person::ASYMPTOMATIC);

    if (person->get_all_clonal_parasite_populations()->contain(
            clinical_caused_parasite_)) {
      auto log_parasite_density =
          clinical_caused_parasite_->last_update_log10_parasite_density();

      // becase the current model does not have within host dynamics, so we
      // assume that the threshold for the parasite density to re-appear in
      // blood is 100 per uL
      const bool isHigherThanRecrudescenceThreshold = log_parasite_density > 2;
      if (isHigherThanRecrudescenceThreshold) {
        person->determine_symptomatic_recrudescence(clinical_caused_parasite_);
      }
    }
  }
}
