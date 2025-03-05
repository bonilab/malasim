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

void UpdateWhenDrugIsPresentEvent::schedule_event(Scheduler *scheduler, Person *p,
                                                  ClonalParasitePopulation *clinical_caused_parasite, const int &time) {
  if (scheduler!=nullptr) {

    auto *e = new UpdateWhenDrugIsPresentEvent();
    e->dispatcher = p;
    e->set_clinical_caused_parasite(clinical_caused_parasite);
    e->time = time;

    p->add_dispatcher(e);
    scheduler->schedule_individual_event(e);
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
    //        no drug in blood, reset the status of drug Update parasite
    // the drug update parasite is inserted into blood when  there is still drug in blood and also the clinical cause parasite

    for (auto i = 0; i < person->get_all_clonal_parasite_populations()->size(); i++) {
      const auto blood_parasite = person->get_all_clonal_parasite_populations()->parasites()->at(i);
      if (blood_parasite->update_function()==Model::get_instance().having_drug_update_function()) {
        //Prevent parasite density fall back to asymptomatic level early when day for parasite cleared by mono drug less than day of clinical ended.
        //This makes parasite density fall back to asymptomatic level after clinical ended.
//        person->determine_relapse_or_not(blood_parasite);
        blood_parasite->set_update_function(Model::get_instance().immunity_clearance_update_function());
      }
    }
  }
}
