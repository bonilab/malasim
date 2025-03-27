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
#include "Treatment/Strategies/NestedMFTStrategy.h"
#include "Events/ReportTreatmentFailureDeathEvent.h"

// OBJECTPOOL_IMPL(ProgressToClinicalEvent)

ProgressToClinicalEvent::ProgressToClinicalEvent() : clinical_caused_parasite_(nullptr) {}

ProgressToClinicalEvent::~ProgressToClinicalEvent() = default;

bool ProgressToClinicalEvent::should_receive_treatment(Person *person) {
  return Model::get_random()->random_flat(0.0, 1.0) <= Model::get_treatment_coverage()->get_probability_to_be_treated(
      person->get_location(), person->get_age());
}

void ProgressToClinicalEvent::handle_no_treatment(Person *person) {
  // did not receive treatment
  Model::get_mdc()->record_1_non_treated_case(person->get_location(), person->get_age(), person->get_age_class());

  if (person->will_progress_to_death_when_receive_no_treatment()) {
    person->cancel_all_events_except(nullptr);
    person->set_host_state(Person::DEAD);
    Model::get_mdc()->record_1_malaria_death(person->get_location(), person->get_age(), false);
    return;
  }
}

std::pair<Therapy *, bool> ProgressToClinicalEvent::determine_therapy(Person *person, bool is_recurrence) {
  auto* strategy = dynamic_cast<NestedMFTStrategy*>(Model::get_treatment_strategy());
  if (strategy != nullptr) {
    // if the strategy is NestedMFT and the therapy is the public sector
    const auto probability = Model::get_random()->random_flat(0.0, 1.0);

    double sum = 0;
    std::size_t s_id = -1;
    for (std::size_t i = 0; i < strategy->distribution.size(); i++) {
      sum += strategy->distribution[i];
      if (probability <= sum) {
        s_id = i;
        break;
      }
    }
    // this is public sector
    if (s_id == 0) {
      if (is_recurrence && Model::get_config()->get_therapy_parameters().get_recurrence_therapy_id() != -1) {
        return {
            Model::get_config()->get_therapy_parameters().therapy_db[Model::get_config()->get_therapy_parameters().get_recurrence_therapy_id()],
            false};
      }
      return {strategy->strategy_list[s_id]->get_therapy(person), true};
    }
    return {strategy->strategy_list[s_id]->get_therapy(person), false};
  }
  // If the strategy is not NestedMFT, then we need to handle the case when the recurrence therapy id is not -1
  if (Model::get_config()->get_therapy_parameters().get_recurrence_therapy_id() != -1) {
    return {
      Model::get_config()->get_therapy_parameters().therapy_db[Model::get_config()->get_therapy_parameters().get_recurrence_therapy_id()],
      false
    };
  }
  // If the strategy is not NestedMFT and the recurrence therapy id is -1, then we need to return the first therapy in the strategy
  return {Model::get_treatment_strategy()->get_therapy(person), true};
}

void ProgressToClinicalEvent::apply_therapy(Person *person, Therapy *therapy, bool is_public_sector) {
  person->receive_therapy(therapy, clinical_caused_parasite_, false, is_public_sector);

  clinical_caused_parasite_->set_update_function(Model::get_instance().having_drug_update_function());

  person->schedule_update_by_drug_event(clinical_caused_parasite_);
  // check if the person will progress to death despite of the treatment, this should be 
  // 90% lower than the no treatment case
  if (person->will_progress_to_death_when_recieve_treatment()) {
    person->cancel_all_events_except(nullptr);
    person->set_host_state(Person::DEAD);
    Model::get_mdc()->record_1_malaria_death(person->get_location(), person->get_age(), true);
    ReportTreatmentFailureDeathEvent::schedule_event(
        Model::get_scheduler(), person, therapy->get_id(),
        Model::get_scheduler()->current_time() + Model::get_config()->get_therapy_parameters().get_tf_testing_day());
    return;
  }
}

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

  transition_to_clinical_state(person);

}

void ProgressToClinicalEvent::transition_to_clinical_state(Person *person) {
  const auto density = Model::get_random()->random_uniform<double>(
      Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_clinical_from(),
      Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_clinical_to());

  clinical_caused_parasite_->set_last_update_log10_parasite_density(density);

  // Person change state to Clinical
  person->set_host_state(Person::CLINICAL);

  // TODO: what is the best option to apply here?
  // on one hand we don't what an individual have multiple clinical episodes
  // consecutively, on the other hand we don't want all the other clinical
  // episode to be cancled (i.e recrudescence epidsodes)
  person->cancel_all_other_progress_to_clinical_events_except(this);

  person->change_all_parasite_update_function(Model::get_instance().progress_to_clinical_update_function(),
                                              Model::get_instance().immunity_clearance_update_function());

  clinical_caused_parasite_->set_update_function(Model::get_instance().clinical_update_function());

  //Statistic collect cumulative clinical episodes
  Model::get_mdc()->collect_1_clinical_episode(person->get_location(), person->get_age(), person->get_age_class());


  if (should_receive_treatment(person)) {
    if ((Model::get_scheduler()->current_time() - person->get_latest_time_received_public_treatment()) < 30) {
      const auto [therapy, is_public_sector] = determine_therapy(person, true);
      // record 1 treatement for recrudescence
      Model::get_mdc()->record_1_recrudescence_treatment(person->get_location(), person->get_age(), person->get_age_class(), 0);

      apply_therapy(person, therapy, is_public_sector);
    } else {
      // this is normal routine for clinical cases
      const auto [therapy, is_public_sector] = determine_therapy(person, false);

      Model::get_mdc()->record_1_treatment(person->get_location(), person->get_age(), person->get_age_class(), therapy->get_id());

      person->schedule_test_treatment_failure_event(clinical_caused_parasite_, Model::get_config()->get_therapy_parameters().get_tf_testing_day(),
                                                    therapy->get_id());
      apply_therapy(person, therapy, is_public_sector);
    }
  } else{
    // not recieve treatment
    // Model::get_mdc()->record_1_non_treated_case(person->get_location(), person->get_age(), person->get_age_class());

    handle_no_treatment(person);
  }
  // schedule end clinical event for both treatment and non-treatment cases
  person->schedule_end_clinical_event(clinical_caused_parasite_);
}

  // TODO: remove this code
//   const auto p = Model::get_random()->random_flat(0.0, 1.0);

//   const auto p_treatment = Model::get_treatment_coverage()->get_probability_to_be_treated(
//       person->get_location(), person->get_age());

// //   std::cout << p_treatment << std::endl;
//   if (p <= p_treatment) {
//     auto *therapy = Model::get_treatment_strategy()->get_therapy(person);

//     person->receive_therapy(therapy, clinical_caused_parasite_);
//     //Statistic increase today treatments
//     Model::get_mdc()->record_1_treatment(person->get_location(), person->get_age(), person->get_age_class(), therapy->get_id());

//     clinical_caused_parasite_->set_update_function(Model::get_instance().having_drug_update_function());

//     // calculate EAMU
//     Model::get_mdc()->record_AMU_AFU(person, therapy, clinical_caused_parasite_);
//     //        calculateEAMU(therapy);
//     //

//     // death is 90% lower than no treatment
//     if (person->will_progress_to_death_when_recieve_treatment()) {

//       //for each test treatment failure event inside individual
//       // record treatment failure (not tf)
//       //            person->record_treatment_failure_for_test_treatment_failure_events();

//       //no treatment routine
//       receive_no_treatment_routine(person);

//       person->cancel_all_events_except(nullptr);
//       person->set_host_state(Person::DEAD);
//       Model::get_mdc()->record_1_malaria_death(person->get_location(), person->get_age(),true);
//       Model::get_mdc()->record_1_TF(person->get_location(), true);
//       Model::get_mdc()->record_1_treatment_failure_by_therapy(person->get_location(), person->get_age(),
//                                                                    therapy->get_id());
//       return;
//     }

//     person->schedule_update_by_drug_event(clinical_caused_parasite_);

//     person->schedule_end_clinical_event(clinical_caused_parasite_);
//     person->schedule_test_treatment_failure_event(clinical_caused_parasite_, Model::get_config()->get_therapy_parameters().get_tf_testing_day(),
//                                                   therapy->get_id());

//   } else {
//     //not recieve treatment
//     //Statistic store NTF
//     Model::get_mdc()->record_1_TF(person->get_location(), false);
//     Model::get_mdc()->record_1_non_treated_case(person->get_location(), person->get_age(), person->get_age_class());

//     receive_no_treatment_routine(person);
//     if (person->get_host_state()==Person::DEAD) {
//       Model::get_mdc()->record_1_malaria_death(person->get_location(), person->get_age(),false);
//       return;
//     }
//     //
//     //        //schedule for end of clinical event
//     //        std::cout << "progress clinical event" << std::endl;

//     person->schedule_end_clinical_by_no_treatment_event(clinical_caused_parasite_);
//   }



void ProgressToClinicalEvent::schedule_event(Scheduler *scheduler, Person *person,
                                             ClonalParasitePopulation *clinical_caused_parasite, const int &time) {
  if (scheduler!=nullptr) {
    auto *event = new ProgressToClinicalEvent();
    event->dispatcher = person;
    event->set_clinical_caused_parasite(clinical_caused_parasite);
    event->time = time;
    person->add_event(event);
  }
}

// void ProgressToClinicalEvent::receive_no_treatment_routine(Person *p) {
//   // if (p->will_progress_to_death_when_receive_no_treatment()) {
//   //   p->cancel_all_events_except(nullptr);
//   //   p->set_host_state(Person::DEAD);
//   // }
// }
