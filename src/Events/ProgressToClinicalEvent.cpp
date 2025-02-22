/* 
 * File:   ProgressToClinicalEvent.cpp
 * Author: Merlin
 * 
 * Created on July 30, 2013, 2:36 PM
 */

#include "ProgressToClinicalEvent.h"
#include "Population/Person/Person.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Population/Population.h"
#include "Simulation/Model.h"
#include "Configuration/Config.h"
#include "Treatment/Strategies/IStrategy.h"
#include "Treatment/Therapies/SCTherapy.h"
#include "Population/ClonalParasitePopulation.h"
#include "Utils/Random.h"
#include "MDC/ModelDataCollector.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Treatment/ITreatmentCoverageModel.h"

// OBJECTPOOL_IMPL(ProgressToClinicalEvent)

ProgressToClinicalEvent::ProgressToClinicalEvent() : clinical_caused_parasite_(nullptr) {}

ProgressToClinicalEvent::~ProgressToClinicalEvent() = default;

void ProgressToClinicalEvent::execute() {
  auto *person = dynamic_cast<Person *>(dispatcher);
  if (person->get_all_clonal_parasite_populations()->size()==0) {
    //parasites might be cleaned by immune system or other things else
    return;
  }

  //if the clinical_caused_parasite eventually removed then do nothing
  if (!person->get_all_clonal_parasite_populations()->contain(clinical_caused_parasite_)) {
    return;
  }

  if (person->get_host_state()==Person::CLINICAL) {
    clinical_caused_parasite_->set_update_function(Model::get_instance().immunity_clearance_update_function());
    return;
  }


  //    Model* model = person->population()->model();
  //    Random* random = model->random();
  //    Config* config = model->config();

  const auto density = Model::get_instance().get_random()->random_uniform_double(
      Model::get_instance().get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_clinical_from(),
      Model::get_instance().get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_clinical_to());

  clinical_caused_parasite_->set_last_update_log10_parasite_density(density);

  // Person change state to Clinical
  person->set_host_state(Person::CLINICAL);

  //this event affect other parasites in population
  //only the parasite that will go to clinical will be change to noneUpdate function,
  //P go to clearance will not be change
  //cancel all other progress to clinical events except current
  person->cancel_all_other_progress_to_clinical_events_except(this);

  person->change_all_parasite_update_function(Model::get_instance().progress_to_clinical_update_function(),
                                              Model::get_instance().immunity_clearance_update_function());
  clinical_caused_parasite_->set_update_function(Model::get_instance().clinical_update_function());

  //Statistic collect cumulative clinical episodes
  Model::get_instance().get_mdc()->collect_1_clinical_episode(person->get_location(), person->get_age(), person->get_age_class());

  const auto p = Model::get_instance().get_random()->random_flat(0.0, 1.0);

  const auto p_treatment = Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(
      person->get_location(), person->get_age());

//   std::cout << p_treatment << std::endl;
  if (p <= p_treatment) {
    auto *therapy = Model::get_instance().get_treatment_strategy()->get_therapy(person);

    person->receive_therapy(therapy, clinical_caused_parasite_);
    //Statistic increase today treatments
    Model::get_instance().get_mdc()->record_1_treatment(person->get_location(), person->get_age(), therapy->get_id());

    clinical_caused_parasite_->set_update_function(Model::get_instance().having_drug_update_function());

    // calculate EAMU
    Model::get_instance().get_mdc()->record_AMU_AFU(person, therapy, clinical_caused_parasite_);
    //        calculateEAMU(therapy);
    //

    // death is 90% lower than no treatment
    if (person->will_progress_to_death_when_recieve_treatment()) {

      //for each test treatment failure event inside individual
      // record treatment failure (not tf)
      //            person->record_treatment_failure_for_test_treatment_failure_events();

      //no treatment routine
      receive_no_treatment_routine(person);

      person->cancel_all_events_except(nullptr);
      person->set_host_state(Person::DEAD);
      Model::get_instance().get_mdc()->record_1_malaria_death(person->get_location(), person->get_age(),true);
      Model::get_instance().get_mdc()->record_1_TF(person->get_location(), true);
      Model::get_instance().get_mdc()->record_1_treatment_failure_by_therapy(person->get_location(), person->get_age(),
                                                                   therapy->get_id());
      return;
    }

    person->schedule_update_by_drug_event(clinical_caused_parasite_);

    person->schedule_end_clinical_event(clinical_caused_parasite_);
    person->schedule_test_treatment_failure_event(clinical_caused_parasite_, Model::get_instance().get_config()->get_therapy_parameters().get_tf_testing_day(),
                                                  therapy->get_id());

  } else {
    //not recieve treatment
    //Statistic store NTF
    Model::get_instance().get_mdc()->record_1_TF(person->get_location(), false);
    Model::get_instance().get_mdc()->record_1_non_treated_case(person->get_location(), person->get_age());

    receive_no_treatment_routine(person);
    if (person->get_host_state()==Person::DEAD) {
      Model::get_instance().get_mdc()->record_1_malaria_death(person->get_location(), person->get_age(),false);
      return;
    }
    //
    //        //schedule for end of clinical event
    //        std::cout << "progress clinical event" << std::endl;

    person->schedule_end_clinical_by_no_treatment_event(clinical_caused_parasite_);
  }
}

void ProgressToClinicalEvent::schedule_event(Scheduler *scheduler, Person *p,
                                             ClonalParasitePopulation *clinical_caused_parasite, const int &time) {
  if (scheduler!=nullptr) {
    auto *e = new ProgressToClinicalEvent();
    e->dispatcher = p;
    e->set_clinical_caused_parasite(clinical_caused_parasite);
    e->time = time;

    p->add_event(e);
    scheduler->schedule_individual_event(e);
  }
}

void ProgressToClinicalEvent::receive_no_treatment_routine(Person *p) {
  if (p->will_progress_to_death_when_receive_no_treatment()) {
    p->cancel_all_events_except(nullptr);
    p->set_host_state(Person::DEAD);
  }
}
