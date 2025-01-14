#include "Event.h"

#include <Core/Scheduler/Dispatcher.h>
#include <spdlog/spdlog.h>

#include <iostream>

Event::Event() = default;

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