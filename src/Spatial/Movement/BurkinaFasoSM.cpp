#include "BurkinaFasoSM.h"

#include "Simulation/Model.h"

void Spatial::BurkinaFasoSM::prepare() {
  // Allow the work to be done
  prepare_kernel();
  AscFile* travel_raster =
      Model::get_spatial_data()->get_raster(SpatialData::SpatialFileType::TRAVEL);
  travel_ = std::move(prepare_surface(travel_raster, static_cast<int>(number_of_locations_)));
}

// TODO: review this as it seems to be not efficient
DoubleVector Spatial::BurkinaFasoSM::get_v_relative_out_movement_to_destination(
    const int &from_location, const int &number_of_locations,
    const DoubleVector &relative_distance_vector,
    const IntVector &v_number_of_residents_by_location) const {
  // Dependent objects should have been created already, so throw an exception
  // if they are not
  if (kernel_.empty() || travel_.empty()) {
    throw std::runtime_error(
        fmt::format("{} called without kernel or travel surface prepared", __FUNCTION__));
  }

  // Note the population size
  auto population = v_number_of_residents_by_location[from_location];

  // Note the source district
  auto source_district = Model::get_spatial_data()->get_admin_unit("district", from_location);

  // Prepare the vector for results
  std::vector<double> results(number_of_locations, 0.0);

  for (auto destination = 0; destination < number_of_locations; destination++) {
    // Continue if there is nothing to do
    if (NumberHelpers::is_zero(relative_distance_vector[destination])) { continue; }

    // Calculate the proportional probability
    double probability = std::pow(population, tau_) * kernel_[from_location][destination];

    // Adjust the probability by the friction surface
    probability = probability / (1 + travel_[from_location] + travel_[destination]);

    // If the source and the destination are both in the capital district,
    // penalize the travel by 50%
    if (source_district == capital_
        && Model::get_spatial_data()->get_admin_unit("district", destination) == capital_) {
      probability /= penalty_;
    }

    results[destination] = probability;
  }

  // Done, return the results
  return results;
}

