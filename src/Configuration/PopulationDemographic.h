#ifndef POPULATION_DEMOGRAPHIC_H
#define POPULATION_DEMOGRAPHIC_H

#include <vector>

struct PopulationDemographic {
  int number_of_age_classes;
  std::vector<int> age_structure;
  std::vector<int> initial_age_structure;
  double birth_rate;
  std::vector<double> death_rate_by_age_class;
  std::vector<double> mortality_when_treatment_fail_by_age_class;
  double artificial_rescaling_of_population_size;
};

#endif  // POPULATION_DEMOGRAPHIC_H

