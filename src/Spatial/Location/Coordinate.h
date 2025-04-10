#ifndef SPATIAL_COORDINATE_H
#define SPATIAL_COORDINATE_H

#include <ostream>

namespace Spatial {
struct Coordinate {
public:
  float latitude;
  float longitude;

  static double calculate_distance_in_km(const Coordinate &from, const Coordinate &to);

  friend std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate);
};

}  // namespace Spatial

#endif  // SPATIAL_COORDINATE_H
