#include "WesolowskiSurfaceSM.h"

#include "Simulation/Model.h"

void Spatial::WesolowskiSurfaceSM::prepare() {
  AscFile* travel_raster =
      Model::get_spatial_data()->get_raster(SpatialData::SpatialFileType::TRAVEL);
  travel = std::move(prepare_surface(travel_raster, number_of_locations_));
}

