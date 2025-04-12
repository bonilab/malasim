#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>
#include <memory>

#include "spdlog/spdlog.h"

template <typename EventType>
class EventManager {
public:
  // Disallow copy
  EventManager(const EventManager&) = delete;
  EventManager& operator=(const EventManager&) = delete;

  // Disallow move
  EventManager(EventManager&&) = delete;
  EventManager& operator=(EventManager&&) = delete;

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
      // take the first event
      auto event = events_.begin()->second.get();
      // execute the event
      event->execute();
      // set the event as not executable
      event->set_executable(false);
      // then erase the event
      events_.erase(events_.begin());
    }
  }

  // Schedule an event and transfer ownership
  void schedule_event(std::unique_ptr<EventType> event) {
    if (event) {
      event->set_executable(true);
      events_.emplace(event->get_time(), std::move(event));
    }
  }

  void cancel_event(EventType* event) {
    if (event) { event->set_executable(false); }
  }

  // Convenience method to check if any event exists
  [[nodiscard]] bool has_event() const { return !events_.empty(); }

  template <typename T>
  [[nodiscard]] bool has_event() const {
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
  void cancel_all_events_except(EventType* in_event) {
    for (auto &[time, event] : events_) {
      if (event.get() != in_event) { event->set_executable(false); }
    }
  }

  // Cancel all events of type T except the specified one
  template <typename T>
  void cancel_all_events_except(EventType* in_event) {
    for (auto &[time, event] : events_) {
      if (event.get() != in_event && dynamic_cast<T*>(event.get()) != nullptr) {
        event->set_executable(false);
      }
    }
  }

  // Clear all events
  void clear_all_events() { events_.clear(); }
};

#endif /* EVENT_MANAGER_H */

