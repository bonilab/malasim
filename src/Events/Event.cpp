#include "Event.h"

#include <Core/Scheduler/Dispatcher.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include "Simulation/Model.h"
#include "Utils/Random.h"

Event::Event() {}

Event::~Event() {
  dispatcher = nullptr;
  scheduler = nullptr;
}

void Event::perform_execute() {
  if (executable) {
    execute();
    executable = false;
  }
}