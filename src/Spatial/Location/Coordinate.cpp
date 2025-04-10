//
// Created by Nguyen Tran on 1/25/2018.
//
#define _USE_MATH_DEFINES

#include "Coordinate.h"

#include <cmath>
#include <ostream>

namespace Spatial {

double Coordinate::calculate_distance_in_km(const Coordinate &from, const Coordinate &to) {
  // using Haversine
  double pi = M_PI / 180;
  int earth_radius = 6371;  // Radius of the Earth in km
  double d_lat = pi * (from.latitude - to.latitude);
  double d_lon = pi * (from.longitude - to.longitude);
  double aa = (sin(d_lat / 2) * sin(d_lat / 2))
              + (cos(from.latitude * pi) * cos(to.latitude * pi) * sin(d_lon / 2) * sin(d_lon / 2));
  double cc = 2 * atan2(sqrt(aa), sqrt(1 - aa));
  double result = earth_radius * cc;

  return result;
}

std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate) {
  os << "[latitude: " << coordinate.latitude << " - longitude: " << coordinate.longitude << "]";
  return os;
}
}  // namespace Spatial
