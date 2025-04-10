#ifndef GRIDBASEDPROCESSOR_H
#define GRIDBASEDPROCESSOR_H

#include <yaml-cpp/yaml.h>

#include "ISpatialSettingsProcessor.h"

class GridBasedProcessor : public ISpatialSettingsProcessor {
public:
  void process_config(const YAML::Node &node) override;
};

#endif
