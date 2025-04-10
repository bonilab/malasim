#ifndef LOCATIONBASEDPROCESSOR_H
#define LOCATIONBASEDPROCESSOR_H

#include <yaml-cpp/yaml.h>

#include "ISpatialSettingsProcessor.h"

class LocationBasedProcessor : public ISpatialSettingsProcessor {
  public:
    void process_config(const YAML::Node &node) override;
};

#endif
