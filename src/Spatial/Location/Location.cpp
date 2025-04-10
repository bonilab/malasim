//
// Created by Nguyen Tran on 1/25/2018.
//

#include "Location.h"

namespace Spatial {

std::ostream &operator<<(std::ostream &os, const Location &location) {
  os << "id: " << location.id << ", populationSize: " << location.population_size
     << ", beta: " << location.beta << ", coordinate: " << location.coordinate
     << ", age_distribution: [";

  for (auto age_class : location.age_distribution) { os << age_class << ","; }
  os << "]";
  os << ", p_treatment: [" << location.p_treatment_under_5 << "," << location.p_treatment_over_5
     << "]"
     << ", mosquito_size: " << location.mosquito_size << ", mosquito_ifr: " << location.mosquito_ifr
     << '\n';
  return os;
}

}  // namespace Spatial
