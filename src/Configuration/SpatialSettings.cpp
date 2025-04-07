#include "SpatialSettings.h"

#include "Simulation/Model.h"

void SpatialSettings::process_config() {
  spdlog::info("Processing SpatialSettings");
  if (mode_ == "grid_based") {
    spdlog::info("Grid based");
    /* Number of location, locations and spatial matrix are calculated in SpatialData class */
    Model::get_spatial_data()->parse(node_);
  } else if (mode_ == "location_based") {
    spdlog::info("Location based");
    // process location_based
    location_based_.set_number_of_locations(
        static_cast<int>(location_based_.get_population_size_by_location().size()));
    number_of_location_ = location_based_.get_number_of_locations();
    spatial_distance_matrix_ =
        std::vector<std::vector<double>>(static_cast<uint64_t>(number_of_location_));
    for (auto from_location = 0; from_location < number_of_location_; from_location++) {
      spatial_distance_matrix_[from_location].resize(static_cast<uint64_t>(number_of_location_));
      for (auto to_location = 0; to_location < number_of_location_; to_location++) {
        spatial_distance_matrix_[from_location][to_location] =
            Spatial::Coordinate::calculate_distance_in_km(
                *this->get_location_based().get_location_info()[from_location].coordinate,
                *this->get_location_based().get_location_info()[to_location].coordinate);
      }
    }
    location_db = location_based_.get_location_info();
  } else {
    throw std::runtime_error("Unknown mode in 'spatial_settings'.");
  }
}

