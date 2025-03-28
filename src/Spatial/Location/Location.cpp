//
// Created by Nguyen Tran on 1/25/2018.
//

#include "Location.h"

namespace Spatial {
Location::Location() : id(0), coordinate{std::make_unique<Coordinate>(0, 0)},
population_size(10000), beta(0),
p_treatment_under_5(0), p_treatment_over_5(0), mosquito_size(100), mosquito_ifr(0.2) {}

Location::Location(const int id, float latitude, float longitude,
                   const int population_size)
    : id{id},
      population_size{population_size},
      beta{0.0f},
      p_treatment_under_5{0.0f},
      p_treatment_over_5{0.0f},
      mosquito_size{100},
      mosquito_ifr{0.2},
      coordinate{std::make_unique<Coordinate>(latitude, longitude)} {}

Location::~Location() = default;

Location::Location(const Location &org)
    : id{org.id},
      population_size{org.population_size},
      beta{org.beta},
      p_treatment_under_5{0},
      p_treatment_over_5{0},
      coordinate{std::make_unique<Coordinate>(org.coordinate->latitude,
                                              org.coordinate->longitude)},
      age_distribution{org.age_distribution},
      mosquito_size{org.mosquito_size},
      mosquito_ifr{org.mosquito_ifr}{}

std::ostream &operator<<(std::ostream &os, const Location &location) {
  os << "id: " << location.id
     << ", populationSize: " << location.population_size
     << ", beta: " << location.beta << ", coordinate: " << *location.coordinate
     << ", age_distribution: [";

  for (auto i : location.age_distribution) { os << i << ","; }
  os << "]";
  os << ", p_treatment: [" << location.p_treatment_under_5 << ","
     << location.p_treatment_over_5 << "]"
     << ", mosquito_size: " << location.mosquito_size
     << ", mosquito_ifr: " << location.mosquito_ifr  << std::endl;
  return os;
}

Location &Location::operator=(const Location &other) {
  id = other.id;
  beta = other.beta;
  population_size = other.population_size;
  p_treatment_under_5 = other.p_treatment_under_5;
  p_treatment_over_5 = other.p_treatment_over_5;
  mosquito_size = other.mosquito_size;
  mosquito_ifr = other.mosquito_ifr;
  coordinate = std::make_unique<Coordinate>(other.coordinate->latitude,
                                            other.coordinate->longitude);
  age_distribution = other.age_distribution;
  return *this;
}
}  // namespace Spatial
