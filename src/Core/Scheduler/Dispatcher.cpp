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
  while (!events_.empty() && events_.begin()->first <= time) {
    // Get all events at the current time point
    int current_time = events_.begin()->first;
    auto range = events_.equal_range(current_time);
    
    // Execute all events at this time point
    for (auto it = range.first; it != range.second; ++it) {
      auto* event = it->second.get();
      try {
        if (event->executable) {
          event->execute();
        }
      }
      catch (const std::exception& ex) {
        spdlog::error("Error in event {} at time {}", event->name(), current_time);
        spdlog::error("Error: {}", ex.what());
      }
    }
    // Erase all events at this time point at once
    erase_events_at_time(current_time);
  }
}

void Dispatcher::schedule_event(Event* event) {
  event->executable = true;
  event->dispatcher = this;
  event->scheduler = Model::get_scheduler();
  events_.insert(std::make_pair(event->time, std::unique_ptr<Event>(event)));
}