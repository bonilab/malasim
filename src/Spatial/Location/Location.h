#ifndef SPATIAL_LOCATION_H
#define SPATIAL_LOCATION_H

#include <ostream>
#include <vector>

#include "Coordinate.h"

namespace Spatial {

/*!
 *  Location is the smallest entity in the spatial structure.
 *  In version 6,  Location is 5km x 5km grid cell if using raster data (grid based)
 *  or just hypothetical location or population if using location based data
 */

struct Location {
public:
  int id{0};
  int population_size{0};
  double beta{0};
  double p_treatment_under_5{0};
  double p_treatment_over_5{0};
  int mosquito_size{0};
  double mosquito_ifr{0};
  Coordinate coordinate{.latitude = 0, .longitude = 0};
  std::vector<double> age_distribution;

  friend std::ostream &operator<<(std::ostream &os, const Location &location) {
    os << "id: " << location.id << ", populationSize: " << location.population_size
       << ", beta: " << location.beta << ", coordinate: " << location.coordinate
       << ", age_distribution: [";

    for (auto age_class : location.age_distribution) { os << age_class << ","; }
    os << "]";
    os << ", p_treatment: [" << location.p_treatment_under_5 << "," << location.p_treatment_over_5
       << "]"
       << ", mosquito_size: " << location.mosquito_size
       << ", mosquito_ifr: " << location.mosquito_ifr << '\n';
    return os;
  }
};
}  // namespace Spatial

#endif  // SPATIAL_LOCATION_H
