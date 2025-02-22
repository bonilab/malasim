#ifndef SPATIAL_COORDINATE_H
#define SPATIAL_COORDINATE_H

#include <ostream>

namespace Spatial {
class Coordinate {
  //disable copy and move
  Coordinate(const Coordinate &) = delete;
  void operator=(const Coordinate &) = delete;

public:
  float latitude;
  float longitude;

public:
  Coordinate(float latitude = 0, float longitude = 0);

  virtual ~Coordinate();

public:
  static double calculate_distance_in_km(const Coordinate &from,
                                         const Coordinate &to);

  friend std::ostream &operator<<(std::ostream &os,
                                  const Coordinate &coordinate);
};

}  // namespace Spatial

#endif  // SPATIAL_COORDINATE_H
