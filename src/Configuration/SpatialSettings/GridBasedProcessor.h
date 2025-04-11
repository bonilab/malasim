#ifndef GRIDBASEDPROCESSOR_H
#define GRIDBASEDPROCESSOR_H

#include <yaml-cpp/yaml.h>

#include "Configuration/SpatialSettings/SpatialSettings.h"
#include "ISpatialSettingsProcessor.h"

class GridBasedProcessor : public ISpatialSettingsProcessor {
public:
  GridBasedProcessor(const GridBasedProcessor &) = default;
  GridBasedProcessor(GridBasedProcessor &&) = delete;
  GridBasedProcessor &operator=(const GridBasedProcessor &) = default;
  GridBasedProcessor &operator=(GridBasedProcessor &&) = delete;
  explicit GridBasedProcessor(SpatialSettings* spatial_settings)
      : ISpatialSettingsProcessor(spatial_settings) {}

  ~GridBasedProcessor() override = default;
  void process_config() override;
};

#endif
