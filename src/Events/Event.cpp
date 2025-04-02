#include "Event.h"

#include <Core/Scheduler/EventManager.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include "Simulation/Model.h"
#include "Utils/Random.h"

Event::Event() {}

Event::~Event() {
  event_manager = nullptr;
}

void Event::execute() {
  if (executable) {
    try {
      do_execute();
    } catch (const std::exception& e) {
      spdlog::error("Error executing event {}: {}", name(), e.what());
    }
    executable = false;
  }
}