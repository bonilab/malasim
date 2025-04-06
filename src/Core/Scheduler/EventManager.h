#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>

#include "spdlog/spdlog.h"

template <typename EventType>
class EventManager {
public:
  // Disallow copy and assign
  EventManager(const EventManager &) = delete;
  void operator=(const EventManager &) = delete;
  EventManager(EventManager &&) = delete;
  EventManager &operator=(EventManager &&) = delete;

private:
  std::multimap<int, std::unique_ptr<EventType>> events_;

  // Helper method to erase all events at a specific time point
  void erase_events_at_time(int time) { events_.erase(time); }

public:
  std::multimap<int, std::unique_ptr<EventType>> &get_events() { return events_; }

  // Constructor and destructor
  EventManager() = default;
  virtual ~EventManager() = default;

  // Initialize/clear events
  virtual void initialize() { events_.clear(); }

  // Execute all events up to and including time
  virtual void execute_events(int time) {
    while (!events_.empty() && events_.begin()->first <= time) {
      // Get all events at the current time point
      int current_time = events_.begin()->first;
      auto range = events_.equal_range(current_time);

      // Execute all events at this time point
      for (auto it = range.first; it != range.second; ++it) {
        auto* event = it->second.get();
        try {
          if (event->is_executable()) { event->execute(); }
        } catch (const std::exception &ex) {
          spdlog::error("Error in event {} at time {}", event->name(), current_time);
          spdlog::error("Error: {}", ex.what());
        }
      }
      // Erase all events at this time point at once
      erase_events_at_time(current_time);
    }
  }

  // Schedule an event and transfer ownership
  void schedule_event(EventType* event) {
    if (event) {
      event->set_executable(true);
      events_.emplace(event->get_time(), std::unique_ptr<EventType>(event));
    }
  }

  void cancel_event(EventType* event) {
    if (event) { event->set_executable(false); }
  }

  // Convenience method to check if any event exists
  bool has_event() const { return !events_.empty(); }

  template <typename T>
  bool has_event() const {
    for (const auto &[time, event] : events_) {
      if (dynamic_cast<T*>(event.get()) != nullptr) { return true; }
    }
    return false;
  }

  // Cancel all events of a specific type
  template <typename T>
  void cancel_all_events() {
    for (auto &[time, event] : events_) {
      if (dynamic_cast<T*>(event.get()) != nullptr) { event->set_executable(false); }
    }
  }

  // Cancel all events except the specified one
  void cancel_all_events_except(EventType* inEvent) {
    for (auto &[time, event] : events_) {
      if (event.get() != inEvent) { event->set_executable(false); }
    }
  }

  // Cancel all events of type T except the specified one
  template <typename T>
  void cancel_all_events_except(EventType* inEvent) {
    for (auto &[time, event] : events_) {
      if (event.get() != inEvent && dynamic_cast<T*>(event.get()) != nullptr) {
        event->set_executable(false);
      }
    }
  }

  // Clear all events
  void clear_all_events() { events_.clear(); }
};

#endif /* EVENT_MANAGER_H */

