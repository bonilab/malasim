#include "EndClinicalByNoTreatmentEvent.h"

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/ClonalParasitePopulation.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Population/Person/Person.h"
#include "Population/SingleHostClonalParasitePopulations.h"

//OBJECTPOOL_IMPL(EndClinicalByNoTreatmentEvent)

EndClinicalByNoTreatmentEvent::EndClinicalByNoTreatmentEvent()
    : clinical_caused_parasite_(nullptr) {}

EndClinicalByNoTreatmentEvent::~EndClinicalByNoTreatmentEvent() = default;

void EndClinicalByNoTreatmentEvent::schedule_event(
    Scheduler* scheduler, Person* p,
    ClonalParasitePopulation* clinical_caused_parasite, const int &time) {
  if (scheduler != nullptr) {
    auto* e = new EndClinicalByNoTreatmentEvent();
    e->dispatcher = p;
    e->set_clinical_caused_parasite(clinical_caused_parasite);
    e->time = time;

    p->add_event(e);
    scheduler->schedule_individual_event(e);
  }
}

void EndClinicalByNoTreatmentEvent::execute() {
  auto* person = dynamic_cast<Person*>(dispatcher);

  if (person->get_all_clonal_parasite_populations()->size() == 0) {
    //        assert(false);
    person->change_state_when_no_parasite_in_blood();

  } else {
    // still have parasite in blood
    person->get_immune_system()->set_increase(true);
    person->set_host_state(Person::ASYMPTOMATIC);
    if (person->get_all_clonal_parasite_populations()->contain(
            clinical_caused_parasite_)) {
      clinical_caused_parasite_->set_last_update_log10_parasite_density(
          Model::get_instance().get_config()->get_parasite_parameters()
          .get_parasite_density_levels().get_log_parasite_density_asymptomatic());

      person->determine_relapse_or_not(clinical_caused_parasite_);
    }
    //        std::cout <<
    //        clinical_caused_parasite_->last_update_log10_parasite_density()<<
    //        std::endl; std::cout <<
    //        person->immune_system()->get_latest_immune_value()<< std::endl;
  }
}
