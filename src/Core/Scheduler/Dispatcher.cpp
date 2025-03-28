/* 
 * File:   Dispatcher.cpp
 * Author: nguyentran
 * 
 * Created on May 3, 2013, 3:46 PM
 */

#include "Dispatcher.h"

#include "Events/Event.h"
#include "Utils/Helpers/ObjectHelpers.h"
#include "spdlog/spdlog.h"
#include "Simulation/Model.h"

Dispatcher::Dispatcher(): events_() {}

void Dispatcher::initialize() {
  events_.clear();
}

Dispatcher::~Dispatcher() {
}


void Dispatcher::execute_events(int time) {
  auto it = events_.begin();
  while (it != events_.end() && it->first <= time) {
    auto *event = it->second.get();

    try {
      if (event->executable) {
        event->perform_execute();
      }
    }
    catch (const std::exception& ex) {
      spdlog::error("Error in event {} at time {}", event->name(), time);
      spdlog::error("Error: {}", ex.what());
    }
    // Erase and advance in one step (safe pattern)
    it = get_events().erase(it);
  }
}

void Dispatcher::schedule_event(Event* event) {
  event->executable = true;
  event->dispatcher = this;
  event->scheduler = Model::get_scheduler();
  events_.insert(std::make_pair(event->time, std::unique_ptr<Event>(event)));
}