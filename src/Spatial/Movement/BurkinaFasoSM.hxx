/*
 * BurkinaFaso.hxx
 *
 * Tuned movement model for Burkina Faso based upon Marshall et al. (2018),
 * with a penalty applied based upon the travel time to the nearest city.
 * Intradistrict movement in capital is also penalized as well.
 *
 * Marshall et al., 2018
 */
#ifndef BURKINAFASOSM_HXX
#define BURKINAFASOSM_HXX

#include "Utils/Helpers/NumberHelpers.hxx"
#include "Spatial/SpatialModel.hxx"
#include "Spatial/GIS/SpatialData.h"
#include "Utils/TypeDef.hxx"

namespace Spatial {
class BurkinaFasoSM : public SpatialModel {
public:
  BurkinaFasoSM(const BurkinaFasoSM &) = delete;
  BurkinaFasoSM operator=(const BurkinaFasoSM &) = delete;
  BurkinaFasoSM(BurkinaFasoSM &&) = delete;
  BurkinaFasoSM &operator=(BurkinaFasoSM &&) = delete;


  [[nodiscard]] double get_tau() const { return tau_; }
  void set_tau(const double &value) { tau_ = value; }

  [[nodiscard]] double get_alpha() const { return alpha_; }
  void set_alpha(const double &value) { alpha_ = value; }

  [[nodiscard]] double get_rho() const { return rho_; }
  void set_rho(const double &value) { rho_ = value; }

  [[nodiscard]] double get_capital() const { return capital_; }
  void set_capital(const double &value) { capital_ = value; }

  [[nodiscard]] double get_penalty() const { return penalty_; }
  void set_penalty(const double &value) { penalty_ = value; }

private:
  double tau_;
  double alpha_;
  double rho_;
  double capital_;
  double penalty_;
  unsigned long number_of_locations_;
  std::vector<std::vector<double>> spatial_distance_matrix_;

  // These variables will be computed when the prepare method is called
  std::vector<double> travel_;
  std::vector<std::vector<double>> kernel_;

  // Precompute the kernel function for the movement model
  void prepare_kernel() {
    // Prepare the kernel object
    kernel_.resize(number_of_locations_);

    // Iterate through all the locations and calculate the kernel
    for (auto source = 0UL; source < number_of_locations_; source++) {
      kernel_[source].resize(number_of_locations_);
      for (auto destination = 0UL; destination < number_of_locations_; destination++) {
        // spdlog::info(
        //     "Calculating kernel for source {} and destination {} spatial_distance {}",
        //     source, destination, spatial_distance_matrix_[source][destination]);
        kernel_[source][destination] =
            std::pow(1 + (spatial_distance_matrix_[source][destination] / rho_), (-alpha_));
      }
    }
  }

public:
  explicit BurkinaFasoSM(double tau, double alpha, double rho, double capital, double penalty,
                         int number_of_locations,
                         std::vector<std::vector<double>> spatial_distance_matrix)
      : tau_(tau), alpha_(alpha), rho_(rho), capital_(capital), penalty_(penalty),
        number_of_locations_(number_of_locations), spatial_distance_matrix_(spatial_distance_matrix) {}

  // Destructor can be removed or simplified since vectors handle cleanup automatically
  ~BurkinaFasoSM() override = default;

  void prepare() override {
    // Allow the work to be done
    prepare_kernel();
    AscFile* travel_raster = SpatialData::get_instance().get_raster(SpatialData::SpatialFileType::Travel);
    travel_ = std::move(prepare_surface(travel_raster, number_of_locations_));
  }

  [[nodiscard]] DoubleVector get_v_relative_out_movement_to_destination(
      const int &from_location, const int &number_of_locations,
      const DoubleVector &relative_distance_vector,
      const IntVector &v_number_of_residents_by_location) const override {
    // Dependent objects should have been created already, so throw an exception
    // if they are not
    if (kernel_.empty() || travel_.empty()) {
      throw std::runtime_error(fmt::format(
          "{} called without kernel or travel surface prepared", __FUNCTION__));
    }

    // Note the population size
    auto population = v_number_of_residents_by_location[from_location];

    // Note the source district
    auto source_district =
        SpatialData::get_instance().get_admin_unit("district", from_location);

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
          std::pow(population, tau_) * kernel_[from_location][destination];

      // Adjust the probability by the friction surface
      probability =
          probability / (1 + travel_[from_location] + travel_[destination]);

      // If the source and the destination are both in the capital district,
      // penalize the travel by 50%
      if (source_district == capital_
          && SpatialData::get_instance().get_admin_unit("district", destination)
                 == capital_) {
        probability /= penalty_;
      }

      results[destination] = probability;
    }

    // Done, return the results
    return results;
  }
};
}  // namespace Spatial

#endif
