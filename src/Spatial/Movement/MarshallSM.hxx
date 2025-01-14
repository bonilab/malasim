/*
 * MarshallSM.hxx
 *
 * Gravity model for human migration based upon a distance kernel function.
 *
 * Marshall et al., 2018
 */
#ifndef MARSHALLSM_HXX
#define MARSHALLSM_HXX

#include "Configuration/Config.h"
#include "Helpers/NumberHelpers.hxx"
#include "Simulation/Model.h"
#include "Spatial/SpatialModel.hxx"
#include "Utils/TypeDef.hxx"
#include "yaml-cpp/yaml.h"

namespace Spatial {
class MarshallSM : public SpatialModel {
  MarshallSM(const MarshallSM &) = delete;
  void operator=(const MarshallSM &) = delete;
  MarshallSM(MarshallSM &&) = delete;
  MarshallSM &operator=(MarshallSM &&) = delete;

  [[nodiscard]] double get_tau() const { return tau_; }
  void set_tau(const double &value) { tau_ = value; }

  [[nodiscard]] double get_alpha() const { return alpha_; }
  void set_alpha(const double &value) { alpha_ = value; }

  [[nodiscard]] double get_rho() const { return log_rho_; }
  void set_log_rho(const double &value) { log_rho_ = value; }

  double tau_;
  double alpha_;
  double log_rho_;
  int number_of_locations_;
  std::vector<std::vector<double>> spatial_distance_matrix_;

  // Pointer to the kernel object since it only needs to be computed once
  double** kernel = nullptr;

  // Precompute the kernel function for the movement model
  void prepare_kernel() {
    // Allocate the memory
    kernel = new double*[number_of_locations_];

    // Iterate through all  the locations and calculate the kernel
    for (auto source = 0; source < number_of_locations_; source++) {
      kernel[source] = new double[number_of_locations_];
      for (auto destination = 0; destination < number_of_locations_; destination++) {
        kernel[source][destination] =
            std::pow(1 + (spatial_distance_matrix_[source][destination] / log_rho_), (-alpha_));
      }
    }
  }

public:
  explicit MarshallSM(double tau, double alpha, double log_rho, int number_of_locations,
    std::vector<std::vector<double>> spatial_distance_matrix)
      : tau_(tau), alpha_(alpha), log_rho_(log_rho), number_of_locations_(number_of_locations),
  spatial_distance_matrix_(spatial_distance_matrix) {}

  ~MarshallSM() override {
    if (kernel != nullptr) {
      for (auto ndx = 0; ndx < number_of_locations_; ndx++) { delete kernel[ndx]; }
      delete kernel;
    }
  }

  void prepare() override {
    prepare_kernel();
  }

  [[nodiscard]] DoubleVector get_v_relative_out_movement_to_destination(
      const int &from_location, const int &number_of_locations,
      const DoubleVector &relative_distance_vector,
      const IntVector &v_number_of_residents_by_location) const override {
    // Note the population size
    auto population = v_number_of_residents_by_location[from_location];

    // Prepare the vector for results
    std::vector<double> results(number_of_locations, 0.0);

    for (auto destination = 0; destination < number_of_locations;
         destination++) {
      // Continue if there is nothing to do
      if (NumberHelpers::is_zero(relative_distance_vector[destination])) {
        continue;
      }

      // Calculate the proportional probability
      double probability =
          std::pow(population, tau_) * kernel[from_location][destination];
      results[destination] = probability;
    }

    // Done, return the results
    return results;
  }
};
}  // namespace Spatial

#endif
