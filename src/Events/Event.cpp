#include "Event.h"

#include <Core/Scheduler/Dispatcher.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include "Simulation/Model.h"
#include "Utils/Random.h"

Event::Event() {
  id_ = Model::get_random()->uuid();
}

Event::~Event(){
  if (dispatcher!=nullptr) {
    dispatcher->remove_dispatcher(this);
  }
  dispatcher = nullptr;
  scheduler = nullptr;
}

void Event::perform_execute() {
  if (executable) {
    if (dispatcher!=nullptr) {
      dispatcher->update_dispatcher();
    }
    execute();
    if (dispatcher!=nullptr) {
      dispatcher->remove_dispatcher(this);
      dispatcher = nullptr;
    }
    executable = false;
  }
}