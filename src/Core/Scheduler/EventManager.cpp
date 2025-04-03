/* 
 * File:   EventManager.cpp
 * Author: nguyentran
 * 
 * Created on May 3, 2013, 3:46 PM
 */

#include "EventManager.h"

#include "Events/Event.h"
#include "Utils/Helpers/ObjectHelpers.h"
#include "spdlog/spdlog.h"
#include "Simulation/Model.h"

// EventManager::EventManager(): events_() {}

// void EventManager::initialize() {
//   events_.clear();
// }

// EventManager::~EventManager() {
// }

// void EventManager::execute_events(int time) {
//   while (!events_.empty() && events_.begin()->first <= time) {
//     // Get all events at the current time point
//     int current_time = events_.begin()->first;
//     auto range = events_.equal_range(current_time);
    
//     // Execute all events at this time point
//     for (auto it = range.first; it != range.second; ++it) {
//       auto* event = it->second.get();
//       try {
//         if (event->is_executable()) {
//           event->execute();
//         }
//       }
//       catch (const std::exception& ex) {
//         spdlog::error("Error in event {} at time {}", event->name(), current_time);
//         spdlog::error("Error: {}", ex.what());
//       }
//     }
//     // Erase all events at this time point at once
//     erase_events_at_time(current_time);
//   }
// }
