#ifndef ISPATIALSETTINGSPROCESSOR_H
#define ISPATIALSETTINGSPROCESSOR_H

#include <yaml-cpp/yaml.h>

class ISpatialSettingsProcessor {
  public:
    virtual ~ISpatialSettingsProcessor() = default;
    virtual void process_config(const YAML::Node &node) = 0;
};

#endif