/*
 * SpatialModel.hxx
 *
 * Base abstract class for the spatial movement models that are implemented in
 * the simulation.
 */
#ifndef SPATIAL_SPATIALMODEL_H
#define SPATIAL_SPATIALMODEL_H

#include <spdlog/spdlog.h>

#include "GIS/SpatialData.h"
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
  static double* prepare_surface(const SpatialData::SpatialFileType type, int number_of_locations) {
    // Get the travel times raster
    spdlog::info("Preparing travel surface...");
    AscFile* raster = SpatialData::get_instance().get_raster(type);
    if (raster == nullptr) {
      throw std::runtime_error(
          fmt::format("{} called without travel data loaded", __FUNCTION__));
    }

    // Use the min and max to normalize the raster into an array
    auto id = 0;
    auto* travel = new double[number_of_locations];
    for (auto row = 0; row < raster->NROWS; row++) {
      for (auto col = 0; col < raster->NCOLS; col++) {
        if (raster->data[row][col] == raster->NODATA_VALUE) { continue; }
        travel[id] = raster->data[row][col];
        id++;
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
