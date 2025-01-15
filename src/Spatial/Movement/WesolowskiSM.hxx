/*
 * WesolowskiSM.hxx
 *
 * Movement model based upon gravity model in
 * https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1004267
 */
#ifndef SPATIAL_WESOLOWSKISM_H
#define SPATIAL_WESOLOWSKISM_H

#include <cmath>

#include "Helpers/NumberHelpers.hxx"
#include "Spatial/SpatialModel.hxx"
#include "Utils/TypeDef.hxx"
#include "yaml-cpp/yaml.h"

namespace Spatial {
class WesolowskiSM : public SpatialModel {
  WesolowskiSM(const WesolowskiSM &) = delete;
  void operator=(const WesolowskiSM &) = delete;
  WesolowskiSM(WesolowskiSM &&) = delete;
  WesolowskiSM &operator=(WesolowskiSM &&) = delete;

  double kappa_;
  double alpha_;
  double beta_;
  double gamma_;

public:
  [[nodiscard]] double get_kappa() const { return kappa_; }
  void set_kappa(const double &value) { kappa_ = value; }

  [[nodiscard]] double get_alpha() const { return alpha_; }
  void set_alpha(const double &value) { alpha_ = value; }

  [[nodiscard]] double get_beta() const { return beta_; }
  void set_beta(const double &value) { beta_ = value; }

  [[nodiscard]] double get_gamma() const { return gamma_; }
  void set_gamma(const double &value) { gamma_ = value; }

  explicit WesolowskiSM(double kappa, double alpha, double beta, double gamma)
      : kappa_(kappa), alpha_(alpha), beta_(beta), gamma_(gamma) {}

  ~WesolowskiSM() override = default;

  [[nodiscard]] DoubleVector get_v_relative_out_movement_to_destination(
      const int &from_location, const int &number_of_locations,
      const DoubleVector &relative_distance_vector,
      const IntVector &v_number_of_residents_by_location) const override {
    std::vector<double> v_relative_number_of_circulation_by_location(
        number_of_locations, 0);
    for (int target_location = 0; target_location < number_of_locations;
         target_location++) {
      if (NumberHelpers::is_zero(relative_distance_vector[target_location])) {
        v_relative_number_of_circulation_by_location[target_location] = 0;
      } else {
        // Gravity model:
        // N_{ij}=\frac{pop^\alpha_ipop^\beta_j}{d(i,j)^\gamma}\kappa
        v_relative_number_of_circulation_by_location[target_location] =
            kappa_
            * (pow(v_number_of_residents_by_location[from_location], alpha_)
               * pow(v_number_of_residents_by_location[target_location], beta_))
            / (pow(relative_distance_vector[target_location], gamma_));
      }
    }
    return v_relative_number_of_circulation_by_location;
  }
};
}  // namespace Spatial

#endif
