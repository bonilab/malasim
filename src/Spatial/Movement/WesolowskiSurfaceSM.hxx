/*
 * WesolowskiSurfaceSM.hxx
 *
 * Movement model based upon gravity model in
 * https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1004267
 * with the travel surface penalty from
 * https://www.nature.com/articles/s41598-022-26878-5 applied to the results.
 */
#ifndef SPATIAL_WESOLOWSKISURFACESM_H
#define SPATIAL_WESOLOWSKISURFACESM_H

#include <cmath>

#include "Utils/Helpers/NumberHelpers.hxx"
#include "Spatial/SpatialModel.hxx"
#include "Utils/TypeDef.hxx"
#include "yaml-cpp/yaml.h"

namespace Spatial {
class WesolowskiSurfaceSM : public SpatialModel {
  WesolowskiSurfaceSM(const WesolowskiSurfaceSM &) = delete;
  void operator=(const WesolowskiSurfaceSM &) = delete;
  WesolowskiSurfaceSM(WesolowskiSurfaceSM &&) = delete;
  WesolowskiSurfaceSM &operator=(WesolowskiSurfaceSM &&) = delete;

  double kappa_;
  double alpha_;
  double beta_;
  double gamma_;
  int number_of_locations_;
  // Travel surface, computed when the prepare method is called
  double* travel = nullptr;

public:
  [[nodiscard]] double get_kappa() const { return kappa_; }
  void set_kappa(const double &value) { kappa_ = value; }

  [[nodiscard]] double get_alpha() const { return alpha_; }
  void set_alpha(const double &value) { alpha_ = value; }

  [[nodiscard]] double get_beta() const { return beta_; }
  void set_beta(const double &value) { beta_ = value; }

  [[nodiscard]] double get_gamma() const { return gamma_; }
  void set_gamma(const double &value) { gamma_ = value; }

  explicit WesolowskiSurfaceSM(double kappa, double alpha, double beta, double gamma, int number_of_locations)
      : kappa_(kappa), alpha_(alpha), beta_(beta), gamma_(gamma), number_of_locations_(number_of_locations) {}

  ~WesolowskiSurfaceSM() override = default;

  void prepare() override { travel = prepare_surface(SpatialData::Travel, number_of_locations_); }

  [[nodiscard]] DoubleVector get_v_relative_out_movement_to_destination(
      const int &from_location, const int &number_of_locations,
      const DoubleVector &relative_distance_vector,
      const IntVector &v_number_of_residents_by_location) const override {
    std::vector<double> results(number_of_locations, 0);
    for (int destination = 0; destination < number_of_locations;
         destination++) {
      if (NumberHelpers::is_zero(relative_distance_vector[destination])) {
        results[destination] = 0;
      } else {
        // Gravity model:
        // N_{ij}=\frac{pop^\alpha_ipop^\beta_j}{d(i,j)^\gamma}\kappa
        auto probability =
            kappa_
            * (pow(v_number_of_residents_by_location[from_location], alpha_)
               * pow(v_number_of_residents_by_location[destination], beta_))
            / (pow(relative_distance_vector[destination], gamma_));

        // Travel penalty: Pr(j|i)' = Pr(j|i) / (1 + t_i + t_j)
        results[destination] =
            probability / (1 + travel[from_location] + travel[destination]);
      }
    }
    return results;
  }
};
}  // namespace Spatial

#endif
