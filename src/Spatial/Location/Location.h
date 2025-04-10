#ifndef SPATIAL_LOCATION_H
#define SPATIAL_LOCATION_H

#include <ostream>
#include <vector>

#include "Coordinate.h"

namespace Spatial {

/*!
 *  Location is the smallest entity in the spatial structure.
 *  Location could be district, province, or zone depends on the availability of
 * the data
 */

struct Location {
public:
  int id{0};
  int population_size{0};
  float beta{0};
  float p_treatment_under_5{0};
  float p_treatment_over_5{0};
  int mosquito_size{0};
  double mosquito_ifr{0};
  Coordinate coordinate{.latitude = 0, .longitude = 0};
  std::vector<double> age_distribution;

  friend std::ostream &operator<<(std::ostream &os, const Location &location);
};
}  // namespace Spatial

#endif  // SPATIAL_LOCATION_H
