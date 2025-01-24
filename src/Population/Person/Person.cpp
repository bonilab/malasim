#include "Person.h"

#include <Helpers/TimeHelpers.h>
#include <spdlog/spdlog.h>
#include <uuid.h>
#include <Configuration/Config.h>
#include <Simulation/Model.h>
#include <Utils/Random.h>

Person::Person() : age_(0),
                  age_class_(0),
                  birthday_(0),
                  location_(0),
                  residence_location_(0),
                  host_state_(Person::HostStates::SUSCEPTIBLE),
                  moving_level_(0),
                  number_of_times_bitten_(0),
                  number_of_trips_taken_(0),
                  last_therapy_id_(0),
                  population_(nullptr),
                  today_infections_(nullptr),
                  today_target_locations_(nullptr),
                  lastest_update_time_(-1){
  id_ = Model::get_instance().get_random()->uuid();
  // immune_system_ = nullptr;
  // all_clonal_parasite_populations_ = nullptr;
  // drugs_in_blood_ = nullptr;
}

Person::~Person() {
  event_queue = std::priority_queue<Event*, std::vector<Event*>, EventComparator>();
  Dispatcher::clear_dispatcher_events();
  delete today_infections_;
  delete today_target_locations_;
  // delete immune_system_;
  // delete all_clonal_parasite_populations_;
  // delete drug_in_blood_;
}

void Person::initialize() {
  Dispatcher::initialize();
  event_queue = std::priority_queue<Event*, std::vector<Event*>, EventComparator>();

  // immune_system_ = new ImmuneSystem(this);
  // all_clonal_parasite_populations_ = new SingleHostClonalParasitePopulations(this);
  // all_clonal_parasite_populations_->init();
  // drugs_in_blood_ = new DrugsInBlood(this);
  // drugs_in_blood_->init();
  today_infections_ = new std::vector<int>();
  today_target_locations_ = new std::vector<int>();
  starting_drug_values_for_MAC_ = std::map<int, double>();
  innate_relative_biting_rate_ = 0;
  current_relative_biting_rate_ = 0;
}

void Person::update(int time) {
  //Update all person attributes before execute events

  execute_events(time);
}

void Person::execute_events(int time) {
  if(event_queue.empty()) return;
  while (!event_queue.empty() && event_queue.top()->time <= time) {
    auto event = event_queue.top();
    event_queue.pop();
    if (get_id_str().substr(0,4) == "4fc8") {
      spdlog::info("Running event {} time {} of person {} {}", event->name(), time, get_id_str(), event->get_id_str());
    }
    event->perform_execute();
  }
}

void Person::add_event(Event* event) {
  event_queue.push(event);
}

void Person::remove_event(Event* event) {
  std::vector<Event*> temp_events;
  while (!event_queue.empty()) {
    auto top_event = event_queue.top();
    event_queue.pop();
    if (top_event != event) {
      temp_events.push_back(top_event);
    }
  }
  for (const auto& e : temp_events) {
    event_queue.push(e);
  }
}

void Person::increase_age_by_1_year() {
  age_++;
}

Population* Person::get_population() const { return population_; }
void Person::set_population(Population* population) { population_ = population; }

int Person::get_location() const { return location_; }
void Person::set_location(int location) { location_ = location; }

int Person::get_residence_location() const { return residence_location_; }
void Person::set_residence_location(int residence_location) { residence_location_ = residence_location; }

Person::HostStates Person::get_host_state() const { return host_state_; }
void Person::set_host_state(HostStates host_state) { host_state_ = host_state; }

int Person::get_age_class() const { return age_class_; }
void Person::set_age_class(int age_class) { age_class_ = age_class; }

int Person::get_birthday() const { return birthday_; }
void Person::set_birthday(int birthday) { birthday_ = birthday; }

int Person::get_lastest_update_time() const { return lastest_update_time_; }
void Person::set_lastest_update_time(int lastest_update_time) { lastest_update_time_ = lastest_update_time; }

int Person::get_moving_level() const { return moving_level_; }
void Person::set_moving_level(int moving_level) { moving_level_ = moving_level; }

std::vector<int>* Person::get_today_infections() const { return today_infections_; }
void Person::set_today_infections(std::vector<int>* today_infections) { today_infections_ = today_infections; }

std::vector<int>* Person::get_today_target_locations() const { return today_target_locations_; }
void Person::set_today_target_locations(std::vector<int>* today_target_locations) { today_target_locations_ = today_target_locations; }

std::vector<double> Person::get_prob_present_at_mda_by_age() const { return prob_present_at_mda_by_age_; }
void Person::set_prob_present_at_mda_by_age(const std::vector<double>& prob_present_at_mda_by_age) { prob_present_at_mda_by_age_ = prob_present_at_mda_by_age; }

int Person::get_number_of_times_bitten() const { return number_of_times_bitten_; }
void Person::set_number_of_times_bitten(int number_of_times_bitten) { number_of_times_bitten_ = number_of_times_bitten; }

int Person::get_number_of_trips_taken() const { return number_of_trips_taken_; }
void Person::set_number_of_trips_taken(int number_of_trips_taken) { number_of_trips_taken_ = number_of_trips_taken; }

int Person::get_last_therapy_id() const { return last_therapy_id_; }
void Person::set_last_therapy_id(int last_therapy_id) { last_therapy_id_ = last_therapy_id; }

std::map<int, double> Person::get_starting_drug_values_for_MAC() const { return starting_drug_values_for_MAC_; }
void Person::set_starting_drug_values_for_MAC(const std::map<int, double>& starting_drug_values_for_MAC) { starting_drug_values_for_MAC_ = starting_drug_values_for_MAC; }

double Person::get_innate_relative_biting_rate() const { return innate_relative_biting_rate_; }
void Person::set_innate_relative_biting_rate(double innate_relative_biting_rate) { innate_relative_biting_rate_ = innate_relative_biting_rate; }

double Person::get_current_relative_biting_rate() const { return current_relative_biting_rate_; }
void Person::set_current_relative_biting_rate(double current_relative_biting_rate) { current_relative_biting_rate_ = current_relative_biting_rate; }


// void Person::NotifyChange(const Property& property, const void* oldValue, const void* newValue) {
//   if (population_ != nullptr) {
//     population_->notify_change(this, property, oldValue, newValue);
//   }
// }

// void Person::set_location(const int& value) {
//   if (location_ != value) {
//     if (Model::DATA_COLLECTOR != nullptr) {
//       const auto day_diff = (Constants::DAYS_IN_YEAR() - Model::SCHEDULER->current_day_in_year());
//       if (location_ != -1) {
//         Model::DATA_COLLECTOR->update_person_days_by_years(location_, -day_diff);
//       }
//       Model::DATA_COLLECTOR->update_person_days_by_years(value, day_diff);
//     }
//
//     Model::DATA_COLLECTOR->record_1_migration(this, location_, value);
//
//     NotifyChange(LOCATION, &location_, &value);
//
//     location_ = value;
//   }
// }


// void Person::set_host_state(const HostStates& value) {
//   if (host_state_ != value) {
//     NotifyChange(HOST_STATE, &host_state_, &value);
//     if (value == DEAD) {
//       // clear also remove all infection forces
//       all_clonal_parasite_populations_->clear();
//       clear_events();
//
//       //
//       //            Model::STATISTIC->update_person_days_by_years(location_, -(Constants::DAYS_IN_YEAR() -
//       //            Model::SCHEDULER->current_day_in_year()));
//       Model::DATA_COLLECTOR->record_1_death(location_, birthday_, number_of_times_bitten_, age_class_, age_);
//     }
//
//     host_state_ = value;
//   }
// }

// void Person::set_age(const int& value) {
//   if (age_ != value) {
//     // TODO::if age access the limit of age structure i.e. 100, remove person???
//
//     NotifyChange(AGE, &age_, &value);
//
//     // update bitting rate level
//     age_ = value;
//
//     // update age class
//     if (Model::MODEL != nullptr) {
//       auto ac = age_class_ == -1 ? 0 : age_class_;
//
//       while (ac < (Model::CONFIG->number_of_age_classes() - 1) && age_ >= Model::CONFIG->age_structure()[ac]) {
//         ac++;
//       }
//
//       set_age_class(ac);
//     }
//   }
// }

// void Person::set_age_class(const int& value) {
//   if (age_class_ != value) {
//     NotifyChange(AGE_CLASS, &age_class_, &value);
//     age_class_ = value;
//   }
// }

// void Person::set_moving_level(const int& value) {
//   if (moving_level_ != value) {
//     NotifyChange(MOVING_LEVEL, &moving_level_, &value);
//     moving_level_ = value;
//   }
// }

// ImmuneSystem* Person::immune_system() const {
//   return immune_system_;
// }

// void Person::set_immune_system(ImmuneSystem* value) {
//   if (immune_system_ != value) {
//     delete immune_system_;
//     immune_system_ = value;
//   }
// }
//
// ClonalParasitePopulation* Person::add_new_parasite_to_blood(Genotype* parasite_type) const {
//   auto* blood_parasite = new ClonalParasitePopulation(parasite_type);
//
//   all_clonal_parasite_populations_->add(blood_parasite);
//
//   blood_parasite->set_last_update_log10_parasite_density(
//       Model::CONFIG->parasite_density_level().log_parasite_density_from_liver);
//
//   return blood_parasite;
// }
//
// double Person::relative_infectivity(const double& log10_parasite_density) {
//   if (log10_parasite_density == ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY) return 0.0;
//
//   // this sigma has already taken 'ln' and 'log10' into account
//   const auto d_n = log10_parasite_density * Model::CONFIG->relative_infectivity().sigma
//                    + Model::CONFIG->relative_infectivity().ro_star;
//   const auto p = Model::RANDOM->cdf_standard_normal_distribution(d_n);
//
//   return p * p + 0.01;
// }
//
// double Person::get_probability_progress_to_clinical() {
//   return immune_system_->get_clinical_progression_probability();
// }
//
// void Person::cancel_all_other_progress_to_clinical_events_except(Event* event) const {
//   for (auto* e : *events()) {
//     if (e != event && dynamic_cast<ProgressToClinicalEvent*>(e) != nullptr) {
//       //            std::cout << "Hello"<< std::endl;
//       e->executable = false;
//     }
//   }
// }
//
// void Person::cancel_all_events_except(Event* event) const {
//   for (auto* e : *events()) {
//     if (e != event) {
//       //            e->set_dispatcher(nullptr);
//       e->executable = false;
//     }
//   }
// }
//
// // void Person::record_treatment_failure_for_test_treatment_failure_events() {
// //
// //     for(Event* e :  *events()) {
// //         if (dynamic_cast<TestTreatmentFailureEvent*> (e) != nullptr && e->executable()) {
// //             //            e->set_dispatcher(nullptr);
// //             //record treatment failure
// //             Model::DATA_COLLECTOR->record_1_treatment_failure_by_therapy(location_, age_,
// //             ((TestTreatmentFailureEvent*) e)->therapyId());
// //
// //         }
// //     }
// // }
//
// void Person::change_all_parasite_update_function(ParasiteDensityUpdateFunction* from,
//                                                  ParasiteDensityUpdateFunction* to) const {
//   all_clonal_parasite_populations_->change_all_parasite_update_function(from, to);
// }
//
// bool Person::will_progress_to_death_when_receive_no_treatment() {
//   // yes == death
//   const auto p = Model::RANDOM->random_flat(0.0, 1.0);
//   return p <= Model::CONFIG->mortality_when_treatment_fail_by_age_class()[age_class_];
// }
//
// bool Person::will_progress_to_death_when_recieve_treatment() {
//   // yes == death
//   double P = Model::RANDOM->random_flat(0.0, 1.0);
//   // 90% lower than no treatment
//   return P <= Model::CONFIG->mortality_when_treatment_fail_by_age_class()[age_class_] * (1 - 0.9);
// }
//
// void Person::schedule_progress_to_clinical_event_by(ClonalParasitePopulation* blood_parasite) {
//   const auto time =
//       (age_ <= 5) ? Model::CONFIG->days_to_clinical_under_five() : Model::CONFIG->days_to_clinical_over_five();
//
//   ProgressToClinicalEvent::schedule_event(Model::SCHEDULER, this, blood_parasite,
//                                           Model::SCHEDULER->current_time() + time);
// }
//
// void Person::schedule_test_treatment_failure_event(ClonalParasitePopulation* blood_parasite, const int& testing_day,
//                                                    const int& t_id) {
//   TestTreatmentFailureEvent::schedule_event(Model::SCHEDULER, this, blood_parasite,
//                                             Model::SCHEDULER->current_time() + testing_day, t_id);
// }
//
// int Person::complied_dosing_days(const int& dosing_day) const {
//   if (Model::CONFIG->p_compliance() < 1) {
//     const auto p = Model::RANDOM->random_flat(0.0, 1.0);
//     if (p > Model::CONFIG->p_compliance()) {
//       // do not comply
//       const auto a = (Model::CONFIG->min_dosing_days() - dosing_day) / (1 - Model::CONFIG->p_compliance());
//       return static_cast<int>(std::ceil(a * p + Model::CONFIG->min_dosing_days() - a));
//     }
//   }
//   return dosing_day;
// }
//
// void Person::receive_therapy(Therapy* therapy, ClonalParasitePopulation* clinical_caused_parasite,
//                              bool is_part_of_MAC_therapy) {
//   // if therapy is SCTherapy
//   auto* sc_therapy = dynamic_cast<SCTherapy*>(therapy);
//   if (sc_therapy != nullptr) {
//     for (int j = 0; j < sc_therapy->drug_ids.size(); ++j) {
//       auto dosing_days = sc_therapy->drug_ids.size() == sc_therapy->dosing_day.size() ? sc_therapy->dosing_day[j]
//                                                                                       : sc_therapy->dosing_day[0];
//
//       dosing_days = complied_dosing_days(dosing_days);
//       int drug_id = sc_therapy->drug_ids[j];
//       add_drug_to_blood(Model::CONFIG->drug_db()->at(drug_id), dosing_days, is_part_of_MAC_therapy);
//     }
//   } else {
//     // else if therapy is MACTherapy
//     auto* mac_therapy = dynamic_cast<MACTherapy*>(therapy);
//     starting_drug_values_for_MAC.clear();
//     assert(mac_therapy != nullptr);
//     for (auto i = 0; i < mac_therapy->therapy_ids().size(); i++) {
//       const auto therapy_id = mac_therapy->therapy_ids()[i];
//       const auto start_day = mac_therapy->start_at_days()[i];
//
//       if (start_day == 1) {
//         receive_therapy(Model::CONFIG->therapy_db()[therapy_id], clinical_caused_parasite, true);
//       } else {
//         assert(start_day > 1);
//         ReceiveTherapyEvent::schedule_event(Model::SCHEDULER, this, Model::CONFIG->therapy_db()[therapy_id],
//                                             Model::SCHEDULER->current_time() + start_day - 1, clinical_caused_parasite,
//                                             true);
//       }
//     }
//   }
//
//   last_therapy_id_ = therapy->id();
// }
//
// void Person::add_drug_to_blood(DrugType* dt, const int& dosing_days, bool is_part_of_MAC_therapy) {
//   auto* drug = new Drug(dt);
//   drug->set_dosing_days(dosing_days);
//   drug->set_last_update_time(Model::SCHEDULER->current_time());
//
//   const auto sd = dt->age_group_specific_drug_concentration_sd()[age_class_];
//   const auto mean_drug_absorption = dt->age_specific_drug_absorption()[age_class_];
//   double drug_level = Model::RANDOM->random_normal_truncated(mean_drug_absorption, sd);
//
//   if (is_part_of_MAC_therapy) {
//     if (drugs_in_blood()->drugs()->find(dt->id()) != drugs_in_blood()->drugs()->end()) {
//       // long-half life drugs
//       drug_level = drugs_in_blood()->get_drug(dt->id())->starting_value();
//     } else if (starting_drug_values_for_MAC.find(dt->id()) != starting_drug_values_for_MAC.end()) {
//       // short half-life drugs
//       drug_level = starting_drug_values_for_MAC[dt->id()];
//     }
//     // store the override value or the default one
//     starting_drug_values_for_MAC[dt->id()] = drug_level;
//   }
//   drug->set_starting_value(drug_level);
//
//   if (drugs_in_blood_->is_drug_in_blood(dt)) {
//     drug->set_last_update_value(drugs_in_blood_->get_drug(dt->id())->last_update_value());
//   } else {
//     drug->set_last_update_value(0.0);
//   }
//
//   drug->set_start_time(Model::SCHEDULER->current_time());
//   drug->set_end_time(Model::SCHEDULER->current_time() + dt->get_total_duration_of_drug_activity(dosing_days));
//
//   drugs_in_blood_->add_drug(drug);
// }
//
// void Person::schedule_update_by_drug_event(ClonalParasitePopulation* clinical_caused_parasite) {
//   UpdateWhenDrugIsPresentEvent::schedule_event(Model::SCHEDULER, this, clinical_caused_parasite,
//                                                Model::SCHEDULER->current_time() + 1);
// }
//
// void Person::schedule_end_clinical_event(ClonalParasitePopulation* clinical_caused_parasite) {
//   int dClinical = Model::RANDOM->random_normal(7, 2);
//   dClinical = std::min<int>(std::max<int>(dClinical, 5), 14);
//
//   EndClinicalEvent::schedule_event(Model::SCHEDULER, this, clinical_caused_parasite,
//                                    Model::SCHEDULER->current_time() + dClinical);
// }
//
// void Person::schedule_end_clinical_by_no_treatment_event(ClonalParasitePopulation* clinical_caused_parasite) {
//   auto d_clinical = Model::RANDOM->random_normal(7, 2);
//   d_clinical = std::min<int>(std::max<int>(d_clinical, 5), 14);
//
//   EndClinicalByNoTreatmentEvent::schedule_event(Model::SCHEDULER, this, clinical_caused_parasite,
//                                                 Model::SCHEDULER->current_time() + d_clinical);
// }
//
// void Person::change_state_when_no_parasite_in_blood() {
//   if (all_clonal_parasite_populations_->size() == 0) {
//     if (liver_parasite_type_ == nullptr) {
//       //        std::cout << "S" << std::endl;
//       //        std::cout << host_state_<< std::endl;
//       set_host_state(SUSCEPTIBLE);
//       //        std::cout << "ES" << std::endl;
//
//     } else {
//       set_host_state(EXPOSED);
//     }
//     immune_system_->set_increase(false);
//   }
// }
//
// void Person::determine_relapse_or_not(ClonalParasitePopulation* clinical_caused_parasite) {
//   if (all_clonal_parasite_populations_->contain(clinical_caused_parasite)) {
//     const auto p = Model::RANDOM->random_flat(0.0, 1.0);
//
//     if (p <= Model::CONFIG->p_relapse()) {
//       //        if (P <= get_probability_progress_to_clinical()) {
//       // progress to clinical after several days
//       clinical_caused_parasite->set_update_function(Model::MODEL->progress_to_clinical_update_function());
//       clinical_caused_parasite->set_last_update_log10_parasite_density(
//           Model::CONFIG->parasite_density_level().log_parasite_density_asymptomatic);
//       schedule_relapse_event(clinical_caused_parasite, Model::CONFIG->relapse_duration());
//
//     } else {
//       // progress to clearance
//       if (clinical_caused_parasite->last_update_log10_parasite_density()
//           > Model::CONFIG->parasite_density_level().log_parasite_density_asymptomatic) {
//         clinical_caused_parasite->set_last_update_log10_parasite_density(
//             Model::CONFIG->parasite_density_level().log_parasite_density_asymptomatic);
//       }
//       clinical_caused_parasite->set_update_function(Model::MODEL->immunity_clearance_update_function());
//     }
//   }
// }
//
// void Person::determine_clinical_or_not(ClonalParasitePopulation* clinical_caused_parasite) {
//   if (all_clonal_parasite_populations_->contain(clinical_caused_parasite)) {
//     const auto p = Model::RANDOM->random_flat(0.0, 1.0);
//
//     //        if (P <= Model::CONFIG->p_relapse()) {
//     //    if (Model::SCHEDULER->current_time() >= 2000 && Model::SCHEDULER->current_time() <= 2010)
//     //      std::cout << this->age() << "\t" << this->immune_system()->get_current_value() << "\t"
//     //                << get_probability_progress_to_clinical()
//     //                << std::endl;
//     if (p <= get_probability_progress_to_clinical()) {
//       // progress to clinical after several days
//       clinical_caused_parasite->set_update_function(Model::MODEL->progress_to_clinical_update_function());
//       clinical_caused_parasite->set_last_update_log10_parasite_density(
//           Model::CONFIG->parasite_density_level().log_parasite_density_asymptomatic);
//       schedule_relapse_event(clinical_caused_parasite, Model::CONFIG->relapse_duration());
//
//     } else {
//       // progress to clearance
//
//       clinical_caused_parasite->set_update_function(Model::MODEL->immunity_clearance_update_function());
//     }
//   }
// }
//
// void Person::schedule_relapse_event(ClonalParasitePopulation* clinical_caused_parasite, const int& time_until_relapse) {
//   int duration = Model::RANDOM->random_normal(time_until_relapse, 15);
//   duration = std::min<int>(std::max<int>(duration, time_until_relapse - 15), time_until_relapse + 15);
//   ProgressToClinicalEvent::schedule_event(Model::SCHEDULER, this, clinical_caused_parasite,
//                                           Model::SCHEDULER->current_time() + duration);
// }
//
// void Person::update() {
//   //    std::cout << "Person Update"<< std::endl;
//   // already update
//   assert(host_state_ != DEAD);
//
//   if (latest_update_time_ == Model::SCHEDULER->current_time()) return;
//
//   //    std::cout << "ppu"<< std::endl;
//   // update the density of each blood parasite in parasite population
//   // parasite will be killed by immune system
//   all_clonal_parasite_populations_->update();
//
//   // update all drugs concentration
//   drugs_in_blood_->update();
//
//   // update drug activity on parasite
//   all_clonal_parasite_populations_->update_by_drugs(drugs_in_blood_);
//
//   immune_system_->update();
//
//   update_current_state();
//
//   // update bitting level only less than 1 to save performance
//   //  the other will be update in birthday event
//   update_relative_bitting_rate();
//
//   latest_update_time_ = Model::SCHEDULER->current_time();
//   //    std::cout << "End Person Update"<< std::endl;
// }
//
// void Person::update_relative_bitting_rate() {
//   if (Model::CONFIG->using_age_dependent_bitting_level()) {
//     current_relative_biting_rate = innate_relative_biting_rate * get_age_dependent_biting_factor();
//   } else {
//     current_relative_biting_rate = innate_relative_biting_rate;
//   }
// }
//
// void Person::update_current_state() {
//   //    std::cout << "ccod" << std::endl;
//   // clear drugs <=0.1
//   drugs_in_blood_->clear_cut_off_drugs_by_event(nullptr);
//   //    std::cout << "ccp" << std::endl;
//   // clear cured parasite
//   all_clonal_parasite_populations_->clear_cured_parasites();
//
//   //    std::cout << "change state" << std::endl;
//   if (all_clonal_parasite_populations_->size() == 0) {
//     change_state_when_no_parasite_in_blood();
//   } else {
//     immune_system_->set_increase(true);
//   }
// }
//
// void Person::randomly_choose_parasite() {
//   if (today_infections_->empty()) {
//     // already chose
//     return;
//   }
//   if (today_infections_->size() == 1) {
//     infected_by(today_infections_->at(0));
//   } else {
//     const std::size_t index_random_parasite = Model::RANDOM->random_uniform(today_infections_->size());
//     infected_by(today_infections_->at(index_random_parasite));
//   }
//
//   today_infections_->clear();
// }
//
// void Person::infected_by(const int& parasite_type_id) {
//   // only infect if liver is available :D
//   if (liver_parasite_type_ == nullptr) {
//     if (host_state_ == SUSCEPTIBLE) {
//       set_host_state(EXPOSED);
//     }
//
//     Genotype* genotype = Model::CONFIG->genotype_db.at(parasite_type_id);
//     liver_parasite_type_ = genotype;
//
//     // move parasite to blood in next 7 days
//     schedule_move_parasite_to_blood(genotype, 7);
//   }
// }
//
// void Person::schedule_move_parasite_to_blood(Genotype* genotype, const int& time) {
//   MoveParasiteToBloodEvent::schedule_event(Model::SCHEDULER, this, genotype, Model::SCHEDULER->current_time() + time);
// }
//
// void Person::schedule_mature_gametocyte_event(ClonalParasitePopulation* clinical_caused_parasite) {
//   const auto day_mature_gametocyte = (age_ <= 5) ? Model::CONFIG->days_mature_gametocyte_under_five()
//                                                  : Model::CONFIG->days_mature_gametocyte_over_five();
//   MatureGametocyteEvent::schedule_event(Model::SCHEDULER, this, clinical_caused_parasite,
//                                         Model::SCHEDULER->current_time() + day_mature_gametocyte);
// }
//
// void Person::randomly_choose_target_location() {
//   if (today_target_locations_->empty()) {
//     // already chose
//     return;
//   }
//
//   auto target_location { -1 };
//   if (today_target_locations_->size() == 1) {
//     target_location = today_target_locations_->at(0);
//   } else {
//     const int index_random_location = Model::RANDOM->random_uniform(today_target_locations_->size());
//     target_location = today_target_locations_->at(index_random_location);
//   }
//
//   schedule_move_to_target_location_next_day_event(target_location);
//
//   today_target_locations_->clear();
// }
//
// void Person::schedule_move_to_target_location_next_day_event(const int& location) {
//   this->number_of_trips_taken_ += 1;
//   CirculateToTargetLocationNextDayEvent::schedule_event(Model::SCHEDULER, this, location,
//                                                         Model::SCHEDULER->current_time() + 1);
// }
//
// bool Person::has_return_to_residence_event() const {
//   for (Event* e : *events()) {
//     if (dynamic_cast<ReturnToResidenceEvent*>(e) != nullptr) {
//       return true;
//     }
//   }
//   return false;
// }
//
// void Person::cancel_all_return_to_residence_events() const {
//   for (Event* e : *events()) {
//     if (dynamic_cast<ReturnToResidenceEvent*>(e) != nullptr) {
//       e->executable = false;
//     }
//   }
// }
//
// bool Person::has_detectable_parasite() const {
//   return all_clonal_parasite_populations_->has_detectable_parasite();
// }
//
// void Person::increase_number_of_times_bitten() {
//   if (Model::SCHEDULER->current_time() >= Model::CONFIG->start_collect_data_day()) {
//     number_of_times_bitten_++;
//   }
// }
//
// void Person::move_to_population(Population* target_population) {
//   assert(population_ != target_population);
//
//   population_->remove_person(this);
//   target_population->add_person(this);
// }
//
// bool Person::has_birthday_event() const {
//   for (Event* e : *events()) {
//     if (dynamic_cast<BirthdayEvent*>(e) != nullptr) {
//       return true;
//     }
//   }
//   return false;
// }
//
// bool Person::has_update_by_having_drug_event() const {
//   for (Event* e : *events()) {
//     if (dynamic_cast<UpdateWhenDrugIsPresentEvent*>(e) != nullptr) {
//       return true;
//     }
//   }
//   return false;
// }
//
// double Person::get_age_dependent_biting_factor() const {
//   //
//   // 0.00 - 0.25  -  6.5
//   // 0.25 - 0.50  -  8.0
//   // 0.50 - 0.75  -  9.0
//   // 0.75 - 1.00  -  9.5
//   // 1.00 - 2.00  -  11.0
//   // 2.00 - 3.00  -  13.5
//   // 3.00 - 4.00  -  15.5
//   // 4.00 - 5.00  -  17.5
//   // + 2.75kg until 20
//   // then divide by 61.5
//
//   if (age_ < 1) {
//     const auto age = ((Model::SCHEDULER->current_time() - birthday_) % Constants::DAYS_IN_YEAR())
//                      / static_cast<double>(Constants::DAYS_IN_YEAR());
//     if (age < 0.25) return 0.106;
//     if (age < 0.5) return 0.13;
//     if (age < 0.75) return 0.1463;
//     return 0.1545;
//   }
//   if (age_ < 2) return 0.1789;
//   if (age_ < 3) return 0.2195;
//   if (age_ < 4) return 0.2520;
//   if (age_ < 20) return (17.5 + (age_ - 4) * 2.75) / 61.5;
//   return 1.0;
// }
//
// double Person::p_infection_from_an_infectious_bite() const {
//   return (1 - immune_system_->get_current_value()) / 8.333 + 0.04;
// }
//
// bool Person::isGametocytaemic() const {
//   return all_clonal_parasite_populations_->is_gametocytaemic();
// }
//
// void Person::generate_prob_present_at_mda_by_age() {
//   if (prob_present_at_mda_by_age().empty()) {
//     for (auto i = 0; i < Model::CONFIG->mean_prob_individual_present_at_mda().size(); i++) {
//       auto value = Model::RANDOM->random_beta(Model::CONFIG->prob_individual_present_at_mda_distribution()[i].alpha,
//                                               Model::CONFIG->prob_individual_present_at_mda_distribution()[i].beta);
//       prob_present_at_mda_by_age_.push_back(value);
//     }
//   }
// }
//
// double Person::prob_present_at_mda() {
//   auto i = 0;
//   // std::cout << "hello " << i << std::endl;
//   while (age_ > Model::CONFIG->age_bracket_prob_individual_present_at_mda()[i]
//          && i < Model::CONFIG->age_bracket_prob_individual_present_at_mda().size()) {
//     i++;
//   }
//
//   return prob_present_at_mda_by_age_[i];
// }
//
// bool Person::has_effective_drug_in_blood() const {
//   for (const auto& kv_drug : *drugs_in_blood_->drugs()) {
//     if (kv_drug.second->last_update_value() > 0.5) return true;
//   }
//   return false;
// }

double Person::draw_random_relative_biting_rate(utils::Random* pRandom, Config* pConfig) {
  auto result =
      pRandom->random_gamma(pConfig->get_epidemiological_parameters().gamma_a,
        pConfig->get_epidemiological_parameters().gamma_b);

  while (result > (pConfig->get_epidemiological_parameters().get_relative_biting_info().get_max_relative_biting_value()
                   - pConfig->get_epidemiological_parameters().get_relative_biting_info().get_min_relative_biting_value())) {
    // re-draw
    result = pRandom->random_gamma(pConfig->get_epidemiological_parameters().gamma_a,
      pConfig->get_epidemiological_parameters().gamma_b);
  }

  return result + pConfig->get_epidemiological_parameters().get_relative_biting_info().get_min_relative_biting_value();
}

