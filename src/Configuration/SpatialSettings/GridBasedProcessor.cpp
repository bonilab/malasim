#include "GridBasedProcessor.h"

#include <spdlog/spdlog.h>

void GridBasedProcessor::process_config() {
  spdlog::info("Processing using GridBasedProcessor");
  auto node = get_spatial_settings()->get_node();
  auto grid_based = node.as<SpatialSettings::GridBased>();

  auto spatial_data = std::make_unique<SpatialData>(get_spatial_settings());
  spatial_data->process_config(node);
  get_spatial_settings()->set_spatial_data(std::move(spatial_data));
}
