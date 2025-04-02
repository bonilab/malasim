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
 // std::vector<Event*> *events_;
 // sorted by time
  std::multimap<int, std::unique_ptr<Event>> events_;

  // Helper method to erase all events at a specific time point
  void erase_events_at_time(int time) {
    events_.erase(time);
  }

public:
  std::multimap<int, std::unique_ptr<Event>>& get_events() {
    return events_;
  }
public:
  EventManager();

  //    EventManager(const EventManager& orig);
  virtual ~EventManager();

  virtual void initialize();

  // this will execute all events up to and including time
  virtual void execute_events(int time);

  // this will transfer ownership of the event to the event manager
  void schedule_event(Event* event);

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
        event->executable = false;
      }
    }
  }

  void cancel_all_events_except(Event* inEvent) const {
    for (auto& [time, event] : events_) {
      if (event.get() != inEvent) {
        event->executable = false;
      }
    }
  }

  template <typename T>
  void cancel_all_events_except(Event* inEvent) const {
    for (auto& [time, event] : events_) {
      if (event.get() != inEvent && dynamic_cast<T*>(event.get()) != nullptr) {
        event->executable = false;
      }
    }
  }
};

#endif    /* EVENT_MANAGER_H */

