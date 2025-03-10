#include "Person.h"

#include <Utils/Helpers/TimeHelpers.h>
#include <spdlog/spdlog.h>
#include <uuid.h>
#include <Configuration/Config.h>
#include <Simulation/Model.h>
#include <Utils/Random.h>

#include "Core/Scheduler/Scheduler.h"
#include "Events/BirthdayEvent.h"
#include "Events/CirculateToTargetLocationNextDayEvent.h"
#include "Events/EndClinicalByNoTreatmentEvent.h"
#include "Events/EndClinicalEvent.h"
#include "Events/MatureGametocyteEvent.h"
#include "Events/MoveParasiteToBloodEvent.h"
#include "Events/ProgressToClinicalEvent.h"
#include "Events/ReceiveTherapyEvent.h"
#include "Events/TestTreatmentFailureEvent.h"
#include "Events/UpdateEveryKDaysEvent.h"
#include "Events/UpdateWhenDrugIsPresentEvent.h"
#include "Events/ReturnToResidenceEvent.h"
#include "Utils/Helpers/ObjectHelpers.h"
#include "MDC/ModelDataCollector.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Population/DrugsInBlood.h"
#include "Population/Population.h"
#include "Population/ImmuneSystem/ImmuneSystem.h"
#include "Treatment/Therapies/Drug.h"
#include "Utils/Cli.hxx"
#include "Validation/MovementValidation.h"

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
                   liver_parasite_type_(nullptr),
                   latest_update_time_(-1){
  id_ = Model::get_random()->uuid();
  population_ = nullptr;
  immune_system_ = nullptr;
  all_clonal_parasite_populations_ = nullptr;
  drugs_in_blood_ = nullptr;

  today_infections_ = nullptr;
  today_target_locations_ = nullptr;
  latest_update_time_ = -1;
}

Person::~Person() {
  while (!event_queue.empty()) {
    delete event_queue.top();
    event_queue.pop();
  }
  Dispatcher::clear_dispatcher_events();
  ObjectHelpers::delete_pointer<ImmuneSystem>(immune_system_);
  ObjectHelpers::delete_pointer<SingleHostClonalParasitePopulations>(all_clonal_parasite_populations_);
  ObjectHelpers::delete_pointer<DrugsInBlood>(drugs_in_blood_);
  ObjectHelpers::delete_pointer<IntVector>(today_infections_);
  ObjectHelpers::delete_pointer<IntVector>(today_target_locations_);
}

void Person::initialize() {
  Dispatcher::initialize();
  event_queue = std::priority_queue<Event*, std::vector<Event*>, EventComparator>();

  immune_system_ = new ImmuneSystem(this);

  all_clonal_parasite_populations_ = new SingleHostClonalParasitePopulations(this);
  all_clonal_parasite_populations_->init();

  drugs_in_blood_ = new DrugsInBlood(this);
  drugs_in_blood_->init();

  today_infections_ = new std::vector<int>();
  today_target_locations_ = new std::vector<int>();

  starting_drug_values_for_MAC_ = std::map<int, double>();
  innate_relative_biting_rate_ = 0;
  current_relative_biting_rate_ = 0;
}

void Person::NotifyChange(const Property& property, const void* oldValue, const void* newValue) {
  if (population_ != nullptr) {
    population_->notify_change(this, property, oldValue, newValue);
  }
}

void Person::set_location(const int& value) {
  if (location_ != value) {
    if (Model::get_mdc() != nullptr) {
      const auto day_diff = (Constants::DAYS_IN_YEAR - Model::get_scheduler()->current_day_in_year());
      if (location_ != -1) {
        Model::get_mdc()->update_person_days_by_years(location_, -day_diff);
      }
      Model::get_mdc()->update_person_days_by_years(value, day_diff);
    }

    NotifyChange(LOCATION, &location_, &value);

    location_ = value;
  }
}

void Person::set_host_state(const HostStates& value) {
  if (host_state_ != value) {
    NotifyChange(HOST_STATE, &host_state_, &value);
    if (value == DEAD) {
      // clear also remove all infection forces
      all_clonal_parasite_populations_->clear();
      clear_dispatcher_events();
      Model::get_mdc()->record_1_death(location_, birthday_, number_of_times_bitten_, age_class_, age_);
    }

    host_state_ = value;
  }
}

void Person::set_age(const int& value) {
  if (age_ != value) {
    // TODO::if age access the limit of age structure i.e. 100, remove person???

    NotifyChange(AGE, &age_, &value);

    // update biting rate level
    age_ = value;

    // update age class
    if (Model::get_instance().get_model() != nullptr) {
      auto ac = age_class_ == -1 ? 0 : age_class_;

      while (ac < (Model::get_config()->get_population_demographic().get_number_of_age_classes() - 1)
        && age_ >= Model::get_config()->get_population_demographic().get_age_structure()[ac]) {
        ac++;
      }
      set_age_class(ac);
    }
  }
}

void Person::set_age_class(const int& value) {
  if (age_class_ != value) {
    NotifyChange(AGE_CLASS, &age_class_, &value);
    age_class_ = value;
  }
}

void Person::set_moving_level(const int& value) {
  if (moving_level_ != value) {
    NotifyChange(MOVING_LEVEL, &moving_level_, &value);
    moving_level_ = value;
  }
}

void Person::set_immune_system(ImmuneSystem* value) {
  if (immune_system_ != value) {
    delete immune_system_;
    immune_system_ = value;
  }
}

ClonalParasitePopulation* Person::add_new_parasite_to_blood(Genotype* parasite_type) const {
  auto* blood_parasite = new ClonalParasitePopulation(parasite_type);

  all_clonal_parasite_populations_->add(blood_parasite);

  blood_parasite->set_last_update_log10_parasite_density(
      Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_from_liver());

  return blood_parasite;
}

double Person::relative_infectivity(const double& log10_parasite_density) {
  if (log10_parasite_density == ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY) return 0.0;

  // this sigma has already taken 'ln' and 'log10' into account
  const auto d_n = log10_parasite_density * Model::get_config()->get_epidemiological_parameters().get_relative_infectivity().get_sigma()
                   + Model::get_config()->get_epidemiological_parameters().get_relative_infectivity().get_ro_star();
  const auto p = Model::get_random()->cdf_standard_normal_distribution(d_n);

  return p * p + 0.01;
}

double Person::get_probability_progress_to_clinical() {
  return immune_system_->get_clinical_progression_probability();
}

void Person::cancel_all_other_progress_to_clinical_events_except(Event* event) const {
  for (auto event_pair : *events()) {
    if (event_pair.second != event && dynamic_cast<ProgressToClinicalEvent*>(event_pair.second) != nullptr) {
      //            std::cout << "Hello"<< std::endl;
      event_pair.second->executable = false;
    }
  }
}

void Person::cancel_all_events_except(Event* event) const {
  for (auto event_pair : *events()) {
    if (event_pair.second != event) {
      //            e->set_dispatcher(nullptr);
      event_pair.second->executable = false;
    }
  }
}

void Person::change_all_parasite_update_function(ParasiteDensityUpdateFunction* from,
                                                 ParasiteDensityUpdateFunction* to) const {
  all_clonal_parasite_populations_->change_all_parasite_update_function(from, to);
}

bool Person::will_progress_to_death_when_receive_no_treatment() {
  // yes == death
  const auto p = Model::get_random()->random_flat(0.0, 1.0);
  return p <= Model::get_config()->get_population_demographic().get_mortality_when_treatment_fail_by_age_class()[age_class_];
}

bool Person::will_progress_to_death_when_recieve_treatment() {
  // yes == death
  double P = Model::get_random()->random_flat(0.0, 1.0);
  // 90% lower than no treatment
  return P <= Model::get_config()->get_population_demographic().get_mortality_when_treatment_fail_by_age_class()[age_class_] * (1 - 0.9);
}

void Person::schedule_progress_to_clinical_event_by(ClonalParasitePopulation* blood_parasite) {
  const auto time =
      (age_ <= 5) ? Model::Model::get_config()->get_epidemiological_parameters().get_days_to_clinical_under_five()
  : Model::get_config()->get_epidemiological_parameters().get_days_to_clinical_over_five();

  ProgressToClinicalEvent::schedule_event(Model::get_scheduler(), this, blood_parasite,
                                          Model::get_scheduler()->current_time() + time);
}

void Person::schedule_test_treatment_failure_event(ClonalParasitePopulation* blood_parasite, const int& testing_day,
                                                   const int& t_id) {
  TestTreatmentFailureEvent::schedule_event(Model::get_scheduler(), this, blood_parasite,
                                            Model::get_scheduler()->current_time() + testing_day, t_id);
}

int Person::complied_dosing_days(const int& dosing_day) const {
  if (Model::get_config()->get_epidemiological_parameters().get_p_compliance() < 1) {
    const auto p = Model::get_random()->random_flat(0.0, 1.0);
    if (p > Model::get_config()->get_epidemiological_parameters().get_p_compliance()) {
      // do not comply
      const auto a = (Model::get_config()->get_epidemiological_parameters().get_min_dosing_days() - dosing_day)
      / (1 - Model::get_config()->get_epidemiological_parameters().get_p_compliance());
      return static_cast<int>(std::ceil(a * p + Model::get_config()->get_epidemiological_parameters().get_min_dosing_days() - a));
    }
  }
  return dosing_day;
}

int Person::complied_dosing_days(const SCTherapy* therapy) {
  // Return the max day if we have full compliance
  if (therapy->full_compliance()) { return therapy->get_max_dosing_day(); }

  // Roll the dice
  auto rv = Model::get_random()->random_flat(0.0, 1.0);

  // Otherwise, iterate through the probabilities that they will complete the
  // therapy on the given day
  auto upper_bound = therapy->pr_completed_days[0];
  for (auto days = 1; days < therapy->pr_completed_days.size() + 1; days++) {
    if (rv < upper_bound) { return days; }
    upper_bound += therapy->pr_completed_days[days];
  }

  // We encountered an error, this should not happen
  throw std::runtime_error("Bounds of pr_completed_days exceeded: rv = "
                           + std::to_string(rv));
}

/*
 * From Temple Malaria Simulation Person.cpp
*/
// Give the therapy indicated to the individual, making note of the parasite
// that caused the clinical case. Note that we assume that MACTherapy is going
// to be fairly rare, but that additional bookkeeping needs to be done in the
// event of one.
void Person::receive_therapy(Therapy* therapy,
                             ClonalParasitePopulation* clinical_caused_parasite,
                             bool is_mac_therapy) {
  // Start by checking if this is a simple therapy with a single dosing regime
  auto* sc_therapy = dynamic_cast<SCTherapy*>(therapy);
  if (sc_therapy != nullptr) {
    receive_therapy(sc_therapy, is_mac_therapy);
  } else {
    // This is not a simple therapy, multiple treatments and dosing regimes may
    // be involved
    auto* mac_therapy = dynamic_cast<MACTherapy*>(therapy);
    assert(mac_therapy != nullptr);

    starting_drug_values_for_MAC_.clear();
    for (std::size_t i = 0; i < mac_therapy->get_therapy_ids().size(); i++) {
      const auto therapy_id = mac_therapy->get_therapy_ids()[i];
      const auto start_day = mac_therapy->get_start_at_days()[i];
      assert(start_day >= 1);

      // Verify the therapy that is part of the regimen
      sc_therapy =
          dynamic_cast<SCTherapy*>(Model::get_config()->get_therapy_parameters().therapy_db[therapy_id]);
      if (sc_therapy == nullptr) {
        auto message = "Complex therapy (" + std::to_string(therapy->get_id())
                       + ") contains a reference to an unknown therapy id ("
                       + std::to_string(therapy_id) + ")";
        throw std::runtime_error(message);
      }
      if (!sc_therapy->full_compliance()) {
        auto message = "Complex therapy (" + std::to_string(therapy->get_id())
                       + ") contains a reference to a therapy ("
                       + std::to_string(therapy_id)
                       + ") that has variable compliance";
        throw std::runtime_error(message);
      }

      if (start_day == 1) {
        receive_therapy(sc_therapy, true);
      } else {
        ReceiveTherapyEvent::schedule_event(
            Model::get_scheduler(), this, sc_therapy,
            Model::get_scheduler()->current_time() + start_day - 1,
            clinical_caused_parasite, true);
      }
    }
  }

  last_therapy_id_ = therapy->get_id();
}

void Person::receive_therapy(SCTherapy* sc_therapy, bool is_mac_therapy) {
  // Determine the dosing days
  auto dosing_days = complied_dosing_days(sc_therapy);

  // Add the treatment to the blood
  for (int drug_id : sc_therapy->drug_ids) {
    add_drug_to_blood(Model::get_config()->get_drug_parameters().drug_db->at(drug_id), dosing_days,
                      is_mac_therapy);
  }
}

void Person::add_drug_to_blood(DrugType* dt, const int& dosing_days, bool is_part_of_MAC_therapy) {

  // Prepare the drug object
  auto* drug = new Drug(dt);
  drug->set_dosing_days(dosing_days);
  drug->set_last_update_time(Model::get_scheduler()->current_time());

  // Find the mean and standard deviation for the drug, and use those values to
  // determine the drug level for this individual
  const auto sd = dt->age_group_specific_drug_concentration_sd()[age_class_];
  const auto mean_drug_absorption = dt->age_specific_drug_absorption()[age_class_];
  double drug_level = Model::get_random()->random_normal_truncated(mean_drug_absorption, sd);

  // If this is going to be part of a complex therapy regime then we need to
  // note this initial drug level
  if (is_part_of_MAC_therapy) {
    if (drugs_in_blood()->drugs()->find(dt->id()) != drugs_in_blood()->drugs()->end()) {
      // Long half-life drugs are already present in the blood
      drug_level = drugs_in_blood()->get_drug(dt->id())->starting_value();
    } else if (starting_drug_values_for_MAC_.find(dt->id()) != starting_drug_values_for_MAC_.end()) {
      // Short half-life drugs that were taken, but cleared the blood already
      drug_level = starting_drug_values_for_MAC_[dt->id()];
    }
    // Note the value for future use
    starting_drug_values_for_MAC_[dt->id()] = drug_level;
  }

  // Set the starting level for this course of treatment
  drug->set_starting_value(drug_level);

  if (drugs_in_blood_->is_drug_in_blood(dt)) {
    drug->set_last_update_value(drugs_in_blood_->get_drug(dt->id())->last_update_value());
  } else {
    drug->set_last_update_value(0.0);
  }

  drug->set_start_time(Model::get_scheduler()->current_time());
  drug->set_end_time(Model::get_scheduler()->current_time() + dt->get_total_duration_of_drug_activity(dosing_days));

  drugs_in_blood_->add_drug(drug);
}

void Person::schedule_update_by_drug_event(ClonalParasitePopulation* clinical_caused_parasite) {
  UpdateWhenDrugIsPresentEvent::schedule_event(Model::get_scheduler(), this, clinical_caused_parasite,
                                               Model::get_scheduler()->current_time() + 1);
}

void Person::schedule_end_clinical_event(ClonalParasitePopulation* clinical_caused_parasite) {
  int dClinical = Model::get_random()->random_normal(7, 2);
  dClinical = std::min<int>(std::max<int>(dClinical, 5), 14);

  EndClinicalEvent::schedule_event(Model::get_scheduler(), this, clinical_caused_parasite,
                                   Model::get_scheduler()->current_time() + dClinical);
}

void Person::schedule_end_clinical_by_no_treatment_event(ClonalParasitePopulation* clinical_caused_parasite) {
  auto d_clinical = Model::get_random()->random_normal(7, 2);
  d_clinical = std::min<int>(std::max<int>(d_clinical, 5), 14);

  EndClinicalByNoTreatmentEvent::schedule_event(Model::get_scheduler(), this, clinical_caused_parasite,
                                                Model::get_scheduler()->current_time() + d_clinical);
}

void Person::change_state_when_no_parasite_in_blood() {
  if (all_clonal_parasite_populations_->size() == 0) {
    if (liver_parasite_type_ == nullptr) {
      set_host_state(SUSCEPTIBLE);
    } else {
      set_host_state(EXPOSED);
    }
    immune_system_->set_increase(false);
  }
}

void Person::determine_relapse_or_not(ClonalParasitePopulation* clinical_caused_parasite) {
  if (all_clonal_parasite_populations_->contain(clinical_caused_parasite)) {
    const auto p = Model::get_random()->random_flat(0.0, 1.0);

    if (p <= Model::get_config()->get_epidemiological_parameters().get_p_relapse()) {
      // progress to clinical after several days
      clinical_caused_parasite->set_update_function(Model::get_instance().progress_to_clinical_update_function());
      clinical_caused_parasite->set_last_update_log10_parasite_density(
          Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_asymptomatic());
      schedule_relapse_event(clinical_caused_parasite, Model::get_config()->get_epidemiological_parameters().get_relapse_duration());

    } else {
      // progress to clearance
      if (clinical_caused_parasite->last_update_log10_parasite_density()
          > Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_asymptomatic()) {
        clinical_caused_parasite->set_last_update_log10_parasite_density(
            Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_asymptomatic());
      }
      clinical_caused_parasite->set_update_function(Model::get_instance().immunity_clearance_update_function());
    }
  }
}

void Person::determine_clinical_or_not(ClonalParasitePopulation* clinical_caused_parasite) {
  if (all_clonal_parasite_populations_->contain(clinical_caused_parasite)) {
    const auto p = Model::get_random()->random_flat(0.0, 1.0);
    if (p <= get_probability_progress_to_clinical()) {
      // progress to clinical after several days
      clinical_caused_parasite->set_update_function(Model::get_instance().progress_to_clinical_update_function());
      clinical_caused_parasite->set_last_update_log10_parasite_density(
          Model::get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_asymptomatic());
      schedule_relapse_event(clinical_caused_parasite, Model::get_config()->get_epidemiological_parameters().get_relapse_duration());

    } else {
      // progress to clearance
      clinical_caused_parasite->set_update_function(Model::get_instance().immunity_clearance_update_function());
    }
  }
}

void Person::schedule_relapse_event(ClonalParasitePopulation* clinical_caused_parasite, const int& time_until_relapse) {
  int duration = Model::get_random()->random_normal(time_until_relapse, 15);
  duration = std::min<int>(std::max<int>(duration, time_until_relapse - 15), time_until_relapse + 15);
  ProgressToClinicalEvent::schedule_event(Model::get_scheduler(), this, clinical_caused_parasite,
                                          Model::get_scheduler()->current_time() + duration);
}

void Person::update() {
  // std::cout << "Person Update " << get_id() << std::endl;
  // already update
  assert(host_state_ != DEAD);

  if (latest_update_time_ == Model::get_scheduler()->current_time()) return;

  //    std::cout << "ppu"<< std::endl;
  // update the density of each blood parasite in parasite population
  // parasite will be killed by immune system
  all_clonal_parasite_populations_->update();

  // update all drugs concentration
  drugs_in_blood_->update();

  // update drug activity on parasite
  all_clonal_parasite_populations_->update_by_drugs(drugs_in_blood_);

  immune_system_->update();

  update_current_state();

  // update biting level only less than 1 to save performance
  //  the other will be update in birthday event
  update_relative_biting_rate();

  latest_update_time_ = Model::get_scheduler()->current_time();
  //    std::cout << "End Person Update"<< std::endl;
}

void Person::update_relative_biting_rate() {
  if (Model::get_config()->get_epidemiological_parameters().get_using_age_dependent_biting_level()) {
    current_relative_biting_rate_ = innate_relative_biting_rate_ * get_age_dependent_biting_factor();
  } else{
    current_relative_biting_rate_ = innate_relative_biting_rate_;
  }
}

void Person::update_current_state() {
  // clear drugs <=0.1
  drugs_in_blood_->clear_cut_off_drugs_by_event(nullptr);
  // clear cured parasite
  all_clonal_parasite_populations_->clear_cured_parasites();

  if (all_clonal_parasite_populations_->size() == 0) {
    change_state_when_no_parasite_in_blood();
  } else {
    immune_system_->set_increase(true);
  }
}

void Person::randomly_choose_parasite() {
  if (today_infections_->empty()) {
    // already chose
    return;
  }
  if (today_infections_->size() == 1) {
    infected_by(today_infections_->at(0));
  } else {
    const std::size_t index_random_parasite = Model::get_random()->random_uniform(today_infections_->size());
    infected_by(today_infections_->at(index_random_parasite));
  }

  today_infections_->clear();
}

void Person::infected_by(const int& parasite_type_id) {
  // only infect if liver is available :D
  if (liver_parasite_type_ == nullptr) {
    if (host_state_ == SUSCEPTIBLE) {
      set_host_state(EXPOSED);
    }

    Genotype* genotype = Model::get_config()->get_genotype_parameters().genotype_db->at(parasite_type_id);
    liver_parasite_type_ = genotype;

    // move parasite to blood in next 7 days
    schedule_move_parasite_to_blood(genotype, 7);
  }
}

void Person::schedule_move_parasite_to_blood(Genotype* genotype, const int& time) {
  MoveParasiteToBloodEvent::schedule_event(Model::get_scheduler(), this, genotype, Model::get_scheduler()->current_time() + time);
}

void Person::schedule_mature_gametocyte_event(ClonalParasitePopulation* clinical_caused_parasite) {
  const auto day_mature_gametocyte = (age_ <= 5) ? Model::get_config()->get_epidemiological_parameters().get_days_mature_gametocyte_under_five()
                                                 : Model::get_config()->get_epidemiological_parameters().get_days_mature_gametocyte_over_five();
  MatureGametocyteEvent::schedule_event(Model::get_scheduler(), this, clinical_caused_parasite,
                                        Model::get_scheduler()->current_time() + day_mature_gametocyte);
}

/*
 * From Temple Malaria Simulation Person.cpp
 */
void Person::schedule_update_every_K_days_event(const int &time) {
  UpdateEveryKDaysEvent::schedule_event(
      Model::get_scheduler(), this, Model::get_scheduler()->current_time() + time);
}

void Person::randomly_choose_target_location() {
  if (today_target_locations_->empty()) {
    // already chose
    return;
  }

  auto target_location { -1 };
  if (today_target_locations_->size() == 1) {
    target_location = today_target_locations_->at(0);
  } else {
    const int index_random_location = Model::get_random()->random_uniform(today_target_locations_->size());
    target_location = today_target_locations_->at(index_random_location);
  }

  // Report the movement if need be
  if (utils::Cli::get_instance().get_record_cell_movement()) {
    auto person_index = static_cast<int>(PersonIndexAllHandler::get_index());
    MovementValidation::add_move(person_index, location_, target_location);
  }

  schedule_move_to_target_location_next_day_event(target_location);

  today_target_locations_->clear();

#ifdef ENABLE_TRAVEL_TRACKING
  // Update the day of the last initiated trip to the next day from current
  // time.
  day_that_last_trip_was_initiated_ = Model::get_scheduler()->current_time() + 1;

  // Check for district raster data availability for spatial analysis.
  if (SpatialData::get_instance().has_raster(
          SpatialData::SpatialFileType::Districts)) {
    auto &spatial_data = SpatialData::get_instance();

    // Determine the source and destination districts for the current trip.
    int source_district = spatial_data.get_district_lookup()[location_];
    int destination_district = spatial_data.get_district_lookup()[target_location];

    // If the trip crosses district boundaries, update the day of the last
    // outside-district trip to the next day from current time.
    if (source_district != destination_district) {
      day_that_last_trip_outside_district_was_initiated_ =
          Model::get_scheduler()->current_time() + 1;
    }
    /* fmt::print("Person {} moved from district {} to district {}\n", _uid, */
    /*            source_district, destination_district); */
          }
#endif
}

void Person::schedule_move_to_target_location_next_day_event(const int& location) {
  this->number_of_trips_taken_ += 1;
  CirculateToTargetLocationNextDayEvent::schedule_event(Model::get_scheduler(), this, location,
                                                        Model::get_scheduler()->current_time() + 1);
}

bool Person::has_return_to_residence_event() const {
  for (auto event_pair : *events()) {
    if (dynamic_cast<ReturnToResidenceEvent*>(event_pair.second) != nullptr) {
      return true;
    }
  }
  return false;
}

void Person::cancel_all_return_to_residence_events() const {
  for (auto event_pair : *events()) {
    if (dynamic_cast<ReturnToResidenceEvent*>(event_pair.second) != nullptr) {
      event_pair.second->executable = false;
    }
  }
}

bool Person::has_detectable_parasite() const {
  return all_clonal_parasite_populations_->has_detectable_parasite();
}

void Person::increase_number_of_times_bitten() {
  if (Model::get_scheduler()->current_time() >= Model::get_config()->get_simulation_timeframe().get_start_collect_data_day()) {
    number_of_times_bitten_++;
  }
}

double Person::get_age_dependent_biting_factor() const {
  //
  // 0.00 - 0.25  -  6.5
  // 0.25 - 0.50  -  8.0
  // 0.50 - 0.75  -  9.0
  // 0.75 - 1.00  -  9.5
  // 1.00 - 2.00  -  11.0
  // 2.00 - 3.00  -  13.5
  // 3.00 - 4.00  -  15.5
  // 4.00 - 5.00  -  17.5
  // + 2.75kg until 20
  // then divide by 61.5

  if (age_ < 1) {
    const auto age = ((Model::get_scheduler()->current_time() - birthday_) % Constants::DAYS_IN_YEAR)
                     / static_cast<double>(Constants::DAYS_IN_YEAR);
    if (age < 0.25) return 0.106;
    if (age < 0.5) return 0.13;
    if (age < 0.75) return 0.1463;
    return 0.1545;
  }
  if (age_ < 2) return 0.1789;
  if (age_ < 3) return 0.2195;
  if (age_ < 4) return 0.2520;
  if (age_ < 20) return (17.5 + (age_ - 4) * 2.75) / 61.5;
  return 1.0;
}

bool Person::isGametocytaemic() const {
  return all_clonal_parasite_populations_->is_gametocytaemic();
}

void Person::generate_prob_present_at_mda_by_age() {
  if (get_prob_present_at_mda_by_age().empty()) {
    for (auto i = 0; i < Model::get_config()->get_strategy_parameters().get_mda().get_mean_prob_individual_present_at_mda().size(); i++) {
      auto value = Model::get_random()->random_beta(Model::get_config()->get_strategy_parameters().get_mda().get_prob_individual_present_at_mda_distribution()[i].alpha,
                                              Model::get_config()->get_strategy_parameters().get_mda().get_prob_individual_present_at_mda_distribution()[i].beta);
      prob_present_at_mda_by_age_.push_back(value);
    }
  }
}

double Person::prob_present_at_mda() {
  auto i = 0;
  // std::cout << "hello " << i << std::endl;
  while (age_ > Model::get_config()->get_strategy_parameters().get_mda().get_age_bracket_prob_individual_present_at_mda()[i]
         && i < Model::get_config()->get_strategy_parameters().get_mda().get_age_bracket_prob_individual_present_at_mda().size()) {
    i++;
  }

  return prob_present_at_mda_by_age_[i];
}

bool Person::has_effective_drug_in_blood() const {
  for (const auto& kv_drug : *drugs_in_blood_->drugs()) {
    if (kv_drug.second->last_update_value() > 0.5) return true;
  }
  return false;
}

void Person::move_to_population(Population* target_population) {
  assert(population_ != target_population);

  population_->remove_person(this);
  target_population->add_person(this);
}

bool Person::has_birthday_event() const {
  for (auto event_pair : *events()) {
    if (dynamic_cast<BirthdayEvent*>(event_pair.second) != nullptr) {
      return true;
    }
  }
  return false;
}

bool Person::has_update_by_having_drug_event() const {
  for (auto event_pair : *events()) {
    if (dynamic_cast<UpdateWhenDrugIsPresentEvent*>(event_pair.second) != nullptr) {
      return true;
    }
  }
  return false;
}

double Person::p_infection_from_an_infectious_bite() const {
  return (1 - immune_system_->get_current_value()) / 8.333 + 0.04;
}

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

double Person::age_in_floating() const {
  auto days = Model::get_scheduler()->current_time() - birthday_;
  return days / static_cast<double>(Constants::DAYS_IN_YEAR);
}

/*
 * NEW KIEN
 */

void Person::increase_age_by_1_year() {
  age_++;
}

void Person::update_events(int time) {
  //Update all person attributes before execute events
  execute_events(time);
}

void Person::execute_events(int time) {
  if(event_queue.empty()) return;
  while (!event_queue.empty() && event_queue.top()->time <= time) {
    auto *event = event_queue.top();
    try {
      if (event->executable) {
        // if (Model::get_random()->random_uniform<double>(0,1) < 0.0001) {
        //   spdlog::debug("Running event {} time {} of person {} {}", event->name(), time, get_id(), event->get_id());
        // }
        event->perform_execute();
      }
    }
    catch (const std::exception& ex) {
      spdlog::error("Error in event {} time {} of person {} {}", event->name(), time, get_id(), event->get_id());
      spdlog::error("Error: {}", ex.what());
    }
    event_queue.pop();
  }
}

void Person::add_event(Event* event) {
  if (event->time > Model::get_config()->get_simulation_timeframe().get_total_time() || event->time < Model::get_scheduler()->current_time_) {
    if (event->time < Model::get_scheduler()->current_time()) {
      spdlog::error("Error when schedule event {} at {}. Current_time: {} - total time: {}",
      event->name(), event->time, Model::get_scheduler()->current_time_,Model::get_scheduler()->total_available_time_);
      spdlog::error("Cannot schedule event {} at {}. Current_time: {} - total time: {}",
        event->name(), event->time, Model::get_scheduler()->current_time_, Model::get_scheduler()->total_available_time_);
    }
    ObjectHelpers::delete_pointer<Event>(event);
  } else {
    event_queue.push(event);
    add_dispatcher(event);
    event->scheduler = Model::get_scheduler();
    event->executable = true;
  }
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
