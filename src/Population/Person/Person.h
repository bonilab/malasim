#ifndef PERSON_H
#define PERSON_H

#include <queue>
#include <vector>
#include <memory>
#include <uuid.h>
#include <Core/Scheduler/EventManager.h>
#include "Events/Event.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Utils/Index/PersonIndexAllHandler.h"
#include "Utils/Index/PersonIndexByLocationMovingLevelHandler.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClassHandler.h"
#include "date/date.h"

class SCTherapy;

namespace utils {
    class Random;
}

class Config;
class Population;
class Event;
class ImmuneSystem;

class Person :  public PersonIndexAllHandler,
                public PersonIndexByLocationStateAgeClassHandler,
                public PersonIndexByLocationMovingLevelHandler {
  // OBJECTPOOL(Person)

  // Disable copy and assignment
  Person(const Person&) = delete;
  void operator=(const Person&) = delete;
  Person(Person&&) = delete;
  Person& operator=(Person&&) = delete;

public:
  enum RecurrenceStatus {
    NONE = 0,
    WITHOUT_SYMPTOM = 1,
    WITH_SYMPTOM = 2
  };


  enum Property {
    LOCATION = 0,
    HOST_STATE,
    AGE,
    AGE_CLASS,
    BITING_LEVEL,
    MOVING_LEVEL,
    EXTERNAL_POPULATION_MOVING_LEVEL
  };

  enum HostStates { SUSCEPTIBLE = 0, EXPOSED = 1, ASYMPTOMATIC = 2, CLINICAL = 3, DEAD = 4, NUMBER_OF_STATE = 5 };

  Person();

  ~Person();
private:
  int age_;
  Population* population_{nullptr};
  int location_{};
  int residence_location_{};
  HostStates host_state_;
  int age_class_{};
  int birthday_{};
  int latest_update_time_{};
  int moving_level_{};
  std::vector<int> today_infections_;
  std::vector<int> today_target_locations_;
  std::vector<double> prob_present_at_mda_by_age_;
  int number_of_times_bitten_{};
  int number_of_trips_taken_{};
  int last_therapy_id_{};
  std::map<int, double> starting_drug_values_for_MAC_;
  double innate_relative_biting_rate_;
  double current_relative_biting_rate_;
  std::unique_ptr<ImmuneSystem> immune_system_{nullptr};
  std::unique_ptr<SingleHostClonalParasitePopulations> all_clonal_parasite_populations_{nullptr};
  std::unique_ptr<DrugsInBlood> drugs_in_blood_{nullptr};
  Genotype* liver_parasite_type_{nullptr};
  int latest_time_received_public_treatment_{-1};
  RecurrenceStatus recurrence_status_{RecurrenceStatus::NONE};
#ifdef ENABLE_TRAVEL_TRACKING
  int day_that_last_trip_was_initiated_{-1};
  int day_that_last_trip_outside_district_was_initiated_{-1};
public:
  int get_day_that_last_trip_was_initiated() const { return day_that_last_trip_was_initiated_; }
  int get_day_that_last_trip_outside_district_was_initiated() const { return day_that_last_trip_outside_district_was_initiated_; }
#endif

private:
  EventManager<PersonEvent> event_manager_;

public:
    void initialize();

    // Delegate event management methods to event_manager_
    void add_event(PersonEvent* event);
    void update_events(int time) { event_manager_.execute_events(time); }
    
    template <typename T>
    bool has_event() const { return event_manager_.has_event<T>(); }
    
    template <typename T>
    void cancel_all_events() { event_manager_.cancel_all_events<T>(); }
    
    void cancel_all_events_except(PersonEvent* event) { 
        event_manager_.cancel_all_events_except(event); 
    }
    
    template <typename T>
    void cancel_all_events_except(PersonEvent* event) {
        event_manager_.cancel_all_events_except<T>(event);
    }

    std::multimap<int, std::unique_ptr<PersonEvent>>& get_events() {
      return event_manager_.get_events();
    }

    void increase_age_by_1_year();

    void update();
    
    Population* get_population() const { return population_; }
    void set_population(Population* population) { population_ = population; }

    void set_age_class(const int& value);

    void set_moving_level(const int& value);

    ImmuneSystem* get_immune_system() {
        return immune_system_.get();
    }
    
    // set and take ownership of immune system
    void set_immune_system(ImmuneSystem* value);

    SingleHostClonalParasitePopulations* get_all_clonal_parasite_populations() {
      return all_clonal_parasite_populations_.get();
    }

    int get_location() const { return location_; }

    int get_residence_location() const { return residence_location_; }

    void set_residence_location(int residence_location) { residence_location_ = residence_location; }

    void set_host_state(const HostStates& value);

    Person::HostStates get_host_state() const { return host_state_; }

    void set_age(const int& value);
    int get_age() const { return age_; }

    int get_age_class() const { return age_class_; }

    int get_birthday() const { return birthday_; }
    void set_birthday(int birthday) { birthday_ = birthday; }

    virtual int get_latest_update_time() const { return latest_update_time_; }
    virtual void set_latest_update_time(int lastest_update_time) { latest_update_time_ = lastest_update_time; }

    int get_moving_level() const { return moving_level_; }

    std::vector<int>& get_today_infections() { return today_infections_; }

    std::vector<int>& get_today_target_locations() { return today_target_locations_; }

    std::vector<double> get_prob_present_at_mda_by_age() const { return prob_present_at_mda_by_age_; }
    void set_prob_present_at_mda_by_age(const std::vector<double>& prob_present_at_mda_by_age) { prob_present_at_mda_by_age_ = prob_present_at_mda_by_age; }

    int get_number_of_times_bitten() const { return number_of_times_bitten_; }
    void set_number_of_times_bitten(int number_of_times_bitten) { number_of_times_bitten_ = number_of_times_bitten; }

    int get_number_of_trips_taken() const { return number_of_trips_taken_; }
    void set_number_of_trips_taken(int number_of_trips_taken) { number_of_trips_taken_ = number_of_trips_taken; }

    int get_last_therapy_id() const { return last_therapy_id_; }
    void set_last_therapy_id(int last_therapy_id) { last_therapy_id_ = last_therapy_id; }

    std::map<int, double> get_starting_drug_values_for_MAC() const { return starting_drug_values_for_MAC_; }
    void set_starting_drug_values_for_MAC(const std::map<int, double>& starting_drug_values_for_MAC) { starting_drug_values_for_MAC_ = starting_drug_values_for_MAC; }

    double get_innate_relative_biting_rate() const { return innate_relative_biting_rate_; }
    void set_innate_relative_biting_rate(double innate_relative_biting_rate) { innate_relative_biting_rate_ = innate_relative_biting_rate; }

    double get_current_relative_biting_rate() const { return current_relative_biting_rate_; }
    void set_current_relative_biting_rate(double current_relative_biting_rate) { current_relative_biting_rate_ = current_relative_biting_rate; }

    int get_latest_time_received_public_treatment() const { return latest_time_received_public_treatment_; }
    void set_latest_time_received_public_treatment(int latest_time_received_public_treatment) { latest_time_received_public_treatment_ = latest_time_received_public_treatment; }

    RecurrenceStatus get_recurrence_status() const { return recurrence_status_; }
    void set_recurrence_status(RecurrenceStatus recurrence_status) { recurrence_status_ = recurrence_status; }

    void update_relative_bitting_rate();

    void NotifyChange(const Property &property, const void *oldValue, const void *newValue);

    void set_location(const int& value);

    DrugsInBlood *drugs_in_blood() {
      return drugs_in_blood_.get();
    }

    Genotype* liver_parasite_type() {
      return liver_parasite_type_;
    }
    void set_liver_parasite_type(Genotype *liver_parasite_type) {
      liver_parasite_type_ = liver_parasite_type;
    }

  ClonalParasitePopulation *add_new_parasite_to_blood(Genotype *parasite_type) const;

  static double relative_infectivity(const double &log10_parasite_density);

  virtual double get_probability_progress_to_clinical();
  //
  virtual bool will_progress_to_death_when_receive_no_treatment();
  //
  virtual bool will_progress_to_death_when_recieve_treatment();

  void cancel_all_other_progress_to_clinical_events_except(PersonEvent *event);

  void change_all_parasite_update_function(ParasiteDensityUpdateFunction *from,
                                           ParasiteDensityUpdateFunction *to) const;

  int complied_dosing_days(const int &dosing_day) const;

  void receive_therapy(Therapy *therapy, ClonalParasitePopulation *clinical_caused_parasite,
                       bool is_part_of_MAC_therapy = false, bool is_public_sector = true);

  void add_drug_to_blood(DrugType *dt, const int &dosing_days, bool is_part_of_MAC_therapy = false);

  void change_state_when_no_parasite_in_blood();

  void determine_symptomatic_recrudescence(
      ClonalParasitePopulation* clinical_caused_parasite);

  void determine_clinical_or_not(
      ClonalParasitePopulation* clinical_caused_parasite);

  void update_current_state();

  void randomly_choose_parasite();

  void infected_by(const int &parasite_type_id);

  void randomly_choose_target_location();

  bool has_return_to_residence_event() const;

  void cancel_all_return_to_residence_events();

  bool has_detectable_parasite() const;

  void increase_number_of_times_bitten();

  bool has_birthday_event() const;

  bool has_update_by_having_drug_event() const;

  double get_age_dependent_biting_factor() const;

  void update_relative_biting_rate();

  double p_infection_from_an_infectious_bite() const;

  bool isGametocytaemic() const;

  void generate_prob_present_at_mda_by_age();

  double prob_present_at_mda();

  bool has_effective_drug_in_blood() const;

  static double draw_random_relative_biting_rate(utils::Random *pRandom, Config *pConfig);

  void receive_therapy(SCTherapy* sc_therapy, bool is_mac_therapy);

  int complied_dosing_days(const SCTherapy* therapy);

  double age_in_floating(int simulation_time) const;

  // Helper methods for scheduling
  int calculate_future_time(int days_from_now) const;

  template<typename T>
  void schedule_basic_event(T* event) {
    event->set_person(this);
    add_event(event);
  }

public:
  // Group 1: Clinical Event Scheduling
  // void schedule_clinical_event(ClonalParasitePopulation* parasite, int days_delay);
  void schedule_end_clinical_event(ClonalParasitePopulation* parasite);
  void schedule_progress_to_clinical_event(ClonalParasitePopulation* parasite);
  void schedule_clinical_recurrence_event(ClonalParasitePopulation* parasite);
  void schedule_test_treatment_failure_event(ClonalParasitePopulation* parasite, int testing_day, int therapy_id = 0);
  void schedule_report_treatment_failure_death_event(int therapy_id, int testing_day);
  void schedule_rapt_event(int days_delay);
  void schedule_receive_mda_therapy_event(Therapy* therapy, int days_delay);
  void schedule_receive_therapy_event(ClonalParasitePopulation* parasite, Therapy* therapy, int days_delay, bool is_part_of_MAC_therapy = false);
  void schedule_switch_immune_component_event(int days_delay);

  // Group 2: Parasite Event Scheduling  
  void schedule_move_parasite_to_blood(Genotype* genotype, int days_delay);
  void schedule_mature_gametocyte_event(ClonalParasitePopulation* parasite);
  void schedule_update_by_drug_event(ClonalParasitePopulation* parasite);

  // Group 3: Movement Event Scheduling
  void schedule_move_to_target_location_next_day_event(int target_location);
  void schedule_return_to_residence_event(int length_of_trip);

  // Group 4: Person Event Scheduling
  void schedule_birthday_event(int days_to_next_birthday = -1);

};

#endif //PERSON_H