#ifndef SPATIAL_COORDINATE_H
#define SPATIAL_COORDINATE_H

#include <ostream>

namespace Spatial {
struct Coordinate {
public:
  float latitude;
  float longitude;

  static double calculate_distance_in_km(const Coordinate &from, const Coordinate &to) {
    // using Haversine
    constexpr double PI = M_PI / 180;
    constexpr int EARTH_RADIUS = 6371;  // Radius of the Earth in km
    double d_lat = PI * (from.latitude - to.latitude);
    double d_lon = PI * (from.longitude - to.longitude);
    double aa =
        (sin(d_lat / 2) * sin(d_lat / 2))
        + (cos(from.latitude * PI) * cos(to.latitude * PI) * sin(d_lon / 2) * sin(d_lon / 2));
    double cc = 2 * atan2(sqrt(aa), sqrt(1 - aa));
    double result = EARTH_RADIUS * cc;

    return result;
  }

  friend std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate) {
    os << "[latitude: " << coordinate.latitude << " - longitude: " << coordinate.longitude << "]";
    return os;
  }
};
}  // namespace Spatial

#endif  // SPATIAL_COORDINATE_H
