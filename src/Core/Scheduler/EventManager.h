#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>
#include <vector>
#include "Events/Event.h"

class EventManager {
public:
 // Disallow copy and assign
 EventManager(const EventManager&) = delete;
 void operator=(const EventManager&) = delete;
 EventManager(EventManager&&) = delete;
 EventManager& operator=(EventManager&&) = delete;

private:
  std::multimap<int, std::unique_ptr<Event>> events_;

  // Helper method to erase all events at a specific time point
  void erase_events_at_time(int time) {
    events_.erase(time);
  }

public:
  std::multimap<int, std::unique_ptr<Event>>& get_events() {
    return events_;
  }

  EventManager();
  virtual ~EventManager();
  virtual void initialize();

  // this will execute all events up to and including time
  virtual void execute_events(int time);

  // Schedule an event and transfer ownership
  void schedule_event(Event* event) {
    if (event) {
      event->set_executable(true);
      events_.emplace(event->get_time(), std::unique_ptr<Event>(event));
    }
  }

  // Convenience method to check if any event exists
  bool has_event() const {
    return !events_.empty();
  }

  template <typename T>
  bool has_event() const {
    for (auto& [time, event] : events_) {
      // For Event base class, return true if any event exists
      if constexpr (std::is_same_v<T, Event>) {
        return !events_.empty();
      }
      // For specific event types, use dynamic_cast to check exact type
      if (dynamic_cast<T*>(event.get()) != nullptr) {
        return true;
      }
    }
    return false;
  }

  template <typename T>
  void cancel_all_events() const {
    for (auto& [time, event] : events_) {
      if (dynamic_cast<T*>(event.get()) != nullptr) {
        event->set_executable(false);
      }
    }
  }

  void cancel_all_events_except(Event* inEvent) const {
    for (auto& [time, event] : events_) {
      if (event.get() != inEvent) {
        event->set_executable(false);
      }
    }
  }

  template <typename T>
  void cancel_all_events_except(Event* inEvent) const {
    for (auto& [time, event] : events_) {
      if (event.get() != inEvent && dynamic_cast<T*>(event.get()) != nullptr) {
        event->set_executable(false);
      }
    }
  }
};

#endif    /* EVENT_MANAGER_H */

