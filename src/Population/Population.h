#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <memory>
#include <string>
#include "Core/Scheduler/EventManager.h"

#include "Person/Person.h"

class Model;
class PersonIndexAll;
class PersonIndexByLocationStateAgeClass;
class PersonIndexByLocationBitingLevel;
class Population {
public:
  // Disable copy and assignment
  Population(const Population&) = delete;
  void operator=(const Population&) = delete;

  Population(Model* model);

  virtual ~Population();

  void initialize();
  //
  // void update(int current_time);
  //
  // // Add a person to the population
  void add_person(Person* person);
  //
  // // Remove a person from the population
  void remove_person(Person* person);

  /**
   * This function removes person pointer out of all of the person indexes
   * This will also delete the @person out of memory
   * @param person
   */
  virtual void remove_dead_person(Person *person);

  // /** Return the total number of individuals in the simulation. */
  // virtual std::size_t size();

  /** Return the total number of individuals in the given location. */
  virtual std::size_t size_at(const int &location);

  /**
   * Return the number of individuals in the population
   * If the input location is -1, return total size
   * @param location
   */
  virtual std::size_t size(const int &location = -1, const int &age_class = -1);

  virtual std::size_t size(const int &location, const Person::HostStates &hs, const int &age_class);

  std::size_t size_residents_only(const int &location);

  /**
   * Notify change of a particular person's property to all person indexes
   * @param p
   * @param property
   * @param oldValue
   * @param newValue
   */
  virtual void notify_change(Person *p, const Person::Property &property, const void *oldValue, const void *newValue);

  virtual void perform_infection_event();

  void introduce_initial_cases();
  //
  void introduce_parasite(const int &location, Genotype *parasite_type, const int &num_of_infections);

  void initial_infection(Person *person, Genotype *parasite_type) const;

  void persist_current_force_of_infection_to_use_N_days_later();

  void perform_birth_event();

  void perform_death_event();

  void generate_individual(int location, int age_class);

  void give_1_birth(const int &location);

  void clear_all_dead_state_individual();

  void perform_circulation_event();

  void perform_circulation_for_1_location(const int &from_location, const int &target_location,
  const int &number_of_circulations, std::vector<Person *> &today_circulations);

  bool has_0_case();

  void initialize_person_indices();

  void update_all_individuals();

  void update_all_individual_events();

  void update_current_foi();

  // Notify the population that a person has moved from the source location, to
  // the destination location
  void notify_movement(int source, int destination);

private:
  // std::vector<Person*> persons_;
  Model *model_;
  PersonIndexPtrList *person_index_list_;
  PersonIndexAll* all_persons_;
  IntVector popsize_by_location_;

public:
  Model* get_model() {
    return model_;
  }
  void set_model(Model* model) {
    model_ = model;
  }
  PersonIndexPtrList* person_index_list() {
      return person_index_list_;
  }
  void set_person_index_list(PersonIndexPtrList* person_index_list) {
      person_index_list_ = person_index_list;
  }
  PersonIndexAll* all_persons() {
      return all_persons_;
  }
  void set_all_persons(PersonIndexAll* all_persons) {
      all_persons_ = all_persons;
  }

public:
  std::vector<std::vector<double>> individual_foi_by_location;
  std::vector<std::vector<double>> individual_relative_biting_by_location;
  std::vector<std::vector<double>> individual_relative_moving_by_location;

  std::vector<double> sum_relative_biting_by_location;
  std::vector<double> sum_relative_moving_by_location;

  std::vector<double> current_force_of_infection_by_location;
  std::vector<std::vector<double>> force_of_infection_for_N_days_by_location;
  std::vector<std::vector<Person *>> all_alive_persons_by_location;

  template <typename T>
  T* get_person_index();

  IntVector get_popsize_by_location() {
    return popsize_by_location_;
  }
  void set_popsize_by_location(const IntVector &popsize_by_location) {
    popsize_by_location_ = popsize_by_location;
  }
};

template <typename T>
T* Population::get_person_index() {
  for (PersonIndex* person_index : *person_index_list_) {
    if (dynamic_cast<T*>(person_index) != nullptr) {
      T* pi = dynamic_cast<T*>(person_index);
      return pi;
    }
  }
  return nullptr;
}
#endif //POPULATION_H