/*
 * SpatialModel.hxx
 *
 * Base abstract class for the spatial movement models that are implemented in
 * the simulation.
 */
#ifndef SPATIAL_SPATIALMODEL_H
#define SPATIAL_SPATIALMODEL_H

#include <spdlog/spdlog.h>

#include "Simulation/Model.h"
#include "Spatial/GIS/SpatialData.h"
#include "Utils/TypeDef.hxx"

class Config;

namespace Spatial {
class SpatialModel {
  SpatialModel(SpatialModel const &) = delete;
  void operator=(SpatialModel const &) = delete;
  SpatialModel(SpatialModel &&) = delete;
  SpatialModel &operator=(SpatialModel &&) = delete;

protected:
  // Prepare the travel raster for the movement model
  static std::vector<double> prepare_surface(const AscFile* travel_raster, int number_of_locations) {
    // Get the travel times raster
    spdlog::info("Preparing travel surface...");
    if (travel_raster == nullptr) {
      throw std::runtime_error(
          fmt::format("{} called without travel data loaded", __FUNCTION__));
    }
    // Initialize vector with the correct size
    std::vector<double> travel;
    travel.reserve(number_of_locations);

    // Use the min and max to normalize the raster into a vector
    for (auto row = 0; row < travel_raster->NROWS; row++) {
      for (auto col = 0; col < travel_raster->NCOLS; col++) {
        if (travel_raster->data[row][col] == travel_raster->NODATA_VALUE) { continue; }
        travel.push_back(travel_raster->data[row][col]);
      }
    }

    // Return the pointer to the array
    return travel;
  }
public:
  SpatialModel() = default;

  virtual ~SpatialModel() = default;

  // Allow the spatial model to perform any preparation it must do.
  virtual void prepare() {}

  [[nodiscard]] virtual DoubleVector get_v_relative_out_movement_to_destination(
      const int &from_location, const int &number_of_locations,
      const DoubleVector &relative_distance_vector,
      const IntVector &v_number_of_residents_by_location) const = 0;
  ;
};
}  // namespace Spatial

#endif
