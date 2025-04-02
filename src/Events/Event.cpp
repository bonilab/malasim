#include "Event.h"

#include <Core/Scheduler/EventManager.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include "Simulation/Model.h"
#include "Utils/Random.h"

void Event::execute() {
  if (executable_) {
    try {
      do_execute();
    } catch (const std::exception& e) {
      spdlog::error("Error executing event {}: {}", name(), e.what());
    }
    executable_ = false;
  }
}