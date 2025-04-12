/*
 * EnvironmentEventBuilder.h
 *
 * Define the functions that are needed to prepare events that are related to
 * the simulated enviornment.
 */
#ifndef ENVIRONMENTEVENTBUILDER_H
#define ENVIRONMENTEVENTBUILDER_H

#include <vector>
#include <memory>

#include "Events/Event.h"

class Config;

namespace YAML {
class Node;
}

class EnvironmentEventBuilder {
public:
  static std::vector<std::unique_ptr<WorldEvent>> build(const YAML::Node &node);

  static std::vector<std::unique_ptr<WorldEvent>> build_update_ecozone_event(const YAML::Node &node,
                                                        Config* config);
};

#endif
