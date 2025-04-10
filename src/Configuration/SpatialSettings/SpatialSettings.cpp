#include "SpatialSettings.h"

#include "GridBasedProcessor.h"
#include "LocationBasedProcessor.h"
#include "Simulation/Model.h"

void SpatialSettings::process_config() {
  spdlog::info("Processing SpatialSettings");
  std::unique_ptr<ISpatialSettingsProcessor> processor = nullptr;
  if (mode_ == GRID_BASED_MODE) {
    processor = std::make_unique<GridBasedProcessor>();
    spdlog::info("Grid based");
    /* Number of location, locations and spatial matrix are calculated in SpatialData class */
    Model::get_spatial_data()->parse(node_);
  } else if (mode_ == LOCATION_BASED_MODE) {
    processor = std::make_unique<LocationBasedProcessor>();
    spdlog::info("Location based");
    // process location_based
    location_db_.clear();
    // loop index based on location_based_.get_location_info()
    for (const auto &location : location_based_.get_location_info()) {
      auto new_location = location;
      auto pop_index = (location.id >= location_based_.get_population_size_by_location().size()) ? 0 : location.id;
      new_location.population_size = location_based_.get_population_size_by_location()[pop_index];

      auto age_index = (location.id >= location_based_.get_age_distribution_by_location().size()) ? 0 : location.id;
      new_location.age_distribution = location_based_.get_age_distribution_by_location()[age_index];

      auto beta_index = (location.id >= location_based_.get_beta_by_location().size()) ? 0 : location.id;
      new_location.beta = location_based_.get_beta_by_location()[beta_index];

      auto p_treatment_under_5_index = (location.id >= location_based_.get_p_treatment_under_5_by_location().size()) ? 0 : location.id;
      new_location.p_treatment_under_5 = location_based_.get_p_treatment_under_5_by_location()[p_treatment_under_5_index];

      auto p_treatment_over_5_index = (location.id >= location_based_.get_p_treatment_over_5_by_location().size()) ? 0 : location.id;
      new_location.p_treatment_over_5 = location_based_.get_p_treatment_over_5_by_location()[p_treatment_over_5_index];

      location_db_.push_back(new_location);
    }

    // populate the distance matrix
    number_of_location_ = location_db_.size();
    spatial_distance_matrix_ =
        std::vector<std::vector<double>>(static_cast<uint64_t>(number_of_location_));
    for (auto from_location = 0; from_location < number_of_location_; from_location++) {
      spatial_distance_matrix_[from_location].resize(static_cast<uint64_t>(number_of_location_));
      for (auto to_location = 0; to_location < number_of_location_; to_location++) {
        spatial_distance_matrix_[from_location][to_location] =
            Spatial::Coordinate::calculate_distance_in_km(
                location_db_[from_location].coordinate,
                location_db_[to_location].coordinate);
      }
    }

    // check if age distribution by location size is equal to number of locations
    if (location_based_.get_age_distribution_by_location().size() != number_of_location_) {
      throw std::runtime_error(
          "Age distribution by location size should be equal to number of locations");
    }
    // check if age distribution by location size is equal to initial age structure size
    for (const auto &age_distribution : location_based_.get_age_distribution_by_location()) {
      if (age_distribution.size() <= 0) {
        throw std::runtime_error("Number of age distribution by location should be greater than 0");
      }
    }
  } else {
    throw std::runtime_error("Unknown mode in 'spatial_settings'.");
  }
  processor->process_config(node_);
}

