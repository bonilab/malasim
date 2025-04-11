#ifndef LOCATIONBASEDPROCESSOR_H
#define LOCATIONBASEDPROCESSOR_H

#include <yaml-cpp/yaml.h>

#include "Configuration/SpatialSettings/SpatialSettings.h"
#include "ISpatialSettingsProcessor.h"

class LocationBasedProcessor : public ISpatialSettingsProcessor {
public:
  LocationBasedProcessor(const LocationBasedProcessor &) = default;
  LocationBasedProcessor(LocationBasedProcessor &&) = delete;
  LocationBasedProcessor &operator=(const LocationBasedProcessor &) = default;
  LocationBasedProcessor &operator=(LocationBasedProcessor &&) = delete;

  explicit LocationBasedProcessor(SpatialSettings* spatial_settings)
      : ISpatialSettingsProcessor(spatial_settings) {}
  ~LocationBasedProcessor() override = default;
  void process_config() override;
};

#endif
