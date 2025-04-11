#ifndef ISPATIALSETTINGSPROCESSOR_H
#define ISPATIALSETTINGSPROCESSOR_H

#include <yaml-cpp/yaml.h>

#include "Configuration/SpatialSettings/SpatialSettings.h"

class ISpatialSettingsProcessor {
public:
  ISpatialSettingsProcessor(const ISpatialSettingsProcessor &) = default;
  ISpatialSettingsProcessor(ISpatialSettingsProcessor &&) = delete;
  ISpatialSettingsProcessor &operator=(const ISpatialSettingsProcessor &) = default;
  ISpatialSettingsProcessor &operator=(ISpatialSettingsProcessor &&) = delete;
  explicit ISpatialSettingsProcessor(SpatialSettings* spatial_settings)
      : spatial_settings_(spatial_settings) {}
  virtual ~ISpatialSettingsProcessor() = default;
  virtual void process_config() = 0;

  SpatialSettings* get_spatial_settings() { return spatial_settings_; }

private:
  SpatialSettings* spatial_settings_;
};

#endif

