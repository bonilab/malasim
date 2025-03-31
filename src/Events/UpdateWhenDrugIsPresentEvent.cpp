#include "UpdateWhenDrugIsPresentEvent.h"

#include "Population/Person/Person.h"
#include "Population/ClonalParasitePopulation.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/DrugsInBlood.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Configuration/Config.h"
#include "Simulation/Model.h"

//OBJECTPOOL_IMPL(UpdateWhenDrugIsPresentEvent)

UpdateWhenDrugIsPresentEvent::UpdateWhenDrugIsPresentEvent() : clinical_caused_parasite_(nullptr) {}

UpdateWhenDrugIsPresentEvent::~UpdateWhenDrugIsPresentEvent() = default;

void UpdateWhenDrugIsPresentEvent::schedule_event(Scheduler *scheduler, Person *person,
                                                  ClonalParasitePopulation *clinical_caused_parasite, const int &time) {
  if (scheduler!=nullptr) {

    auto *event = new UpdateWhenDrugIsPresentEvent();
    event->dispatcher = person;
    event->set_clinical_caused_parasite(clinical_caused_parasite);
    event->time = time;
    person->add_event(event);
  }
}

void UpdateWhenDrugIsPresentEvent::execute() {
  auto *person = dynamic_cast<Person *>(dispatcher);
  if (person->drugs_in_blood()->size() > 0) {
    if (person->get_all_clonal_parasite_populations()->contain(clinical_caused_parasite_) && person->get_host_state()==
        Person::CLINICAL) {
      if (clinical_caused_parasite_->last_update_log10_parasite_density() <= Model::get_config()->get_parasite_parameters().
          get_parasite_density_levels().
          get_log_parasite_density_asymptomatic()) {
        person->set_host_state(Person::ASYMPTOMATIC);
      }
    }
    person->schedule_update_by_drug_event(clinical_caused_parasite_);
  } else {
    for (auto i = 0; i < person->get_all_clonal_parasite_populations()->size(); i++) {
      auto* blood_parasite = person->get_all_clonal_parasite_populations()->at(i);
      if (blood_parasite->update_function()==Model::get_instance().having_drug_update_function()) {
        blood_parasite->set_update_function(Model::get_instance().immunity_clearance_update_function());
      }
    }
  }
}
