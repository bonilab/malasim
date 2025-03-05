#include "MoveParasiteToBloodEvent.h"


#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/ClonalParasitePopulation.h"
#include "Population/DrugsInBlood.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Population/Person/Person.h"
#include "Population/SingleHostClonalParasitePopulations.h"

//OBJECTPOOL_IMPL(MoveParasiteToBloodEvent)

MoveParasiteToBloodEvent::MoveParasiteToBloodEvent()
    : infection_genotype_(nullptr) {}

MoveParasiteToBloodEvent::~MoveParasiteToBloodEvent() {}

void MoveParasiteToBloodEvent::schedule_event(Scheduler* scheduler, Person* p,
                                              Genotype* infection_type,
                                              const int &time) {
  if (scheduler != nullptr) {
    auto* e = new MoveParasiteToBloodEvent();
    e->dispatcher = p;
    e->set_infection_genotype(infection_type);
    e->time = time;

    p->add_dispatcher(e);
    scheduler->schedule_individual_event(e);
  }
}

void MoveParasiteToBloodEvent::execute() {
  auto* person = dynamic_cast<Person*>(dispatcher);
  auto* parasite_type = person->liver_parasite_type();
  person->set_liver_parasite_type(nullptr);

  // add to blood
  if (person->get_host_state() == Person::EXPOSED) {
    person->set_host_state(Person::ASYMPTOMATIC);
  }

  person->get_immune_system()->set_increase(true);

  auto new_parasite = person->add_new_parasite_to_blood(parasite_type);

  new_parasite->set_last_update_log10_parasite_density(
      Model::get_random()->random_normal_truncated(
          Model::get_config()->get_parasite_parameters().get_parasite_density_levels()
              .get_log_parasite_density_asymptomatic(),
          0.5));

  if (person->has_effective_drug_in_blood()) {
    // person has drug in blood
    new_parasite->set_update_function(
        Model::get_instance().having_drug_update_function());
  } else {
    if (person->get_all_clonal_parasite_populations()->size() > 1) {
      if (Model::get_config()->get_epidemiological_parameters().get_allow_new_coinfection_to_cause_symptoms()) {
        person->determine_clinical_or_not(new_parasite);
      } else {
        new_parasite->set_update_function(
            Model::get_instance().immunity_clearance_update_function());
      }
    } else {
      person->determine_clinical_or_not(new_parasite);
    }
  }

  person->schedule_mature_gametocyte_event(new_parasite);
}
