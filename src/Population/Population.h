
#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <memory>

#include "Person/Person.h"

class Model;

class Population {
public:
  // Disable copy and assignment
  Population(const Population&) = delete;
  Population& operator=(const Population&) = delete;
  Population(Population&&) = delete;
  Population& operator=(Population&&) = delete;

  Population(Model *model);

  ~Population();

  void initialize();
  //
  void update(int current_time);
  //
  // // Add a person to the population
  void add_person(Person* person);
  //
  // // Remove a person from the population
  void remove_person(Person* person);

  // Get the number of persons in the population
  size_t size() const;

private:
  std::vector<Person*> persons_;
  Model *model_;
};

#endif //POPULATION_H