#ifndef DISPATCHER_H
#define    DISPATCHER_H

#include <map>
#include <uuid.h>
#include <vector>
#include "Events/Event.h"

class Dispatcher {
public:
 // Disallow copy and assign
 Dispatcher(const Dispatcher&) = delete;
 void operator=(const Dispatcher&) = delete;
 Dispatcher(Dispatcher&&) = delete;
 Dispatcher& operator=(Dispatcher&&) = delete;

private:
 // std::vector<Event*> *events_;
 // sorted by time
  std::multimap<int, std::unique_ptr<Event>> events_;

public:
  std::multimap<int, std::unique_ptr<Event>>& get_events() {
    return events_;
  }
public:
  Dispatcher();

  //    Dispatcher(const Dispatcher& orig);
  virtual ~Dispatcher();

  virtual void initialize();

  // this will execute all events at the given time
  void execute_events(int time);

  // this will transfer ownership of the event to the dispatcher
  void schedule_event(Event* event);

  template <typename T>
  bool has_event() const {
    for (auto& [time, event] : events_) {
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

  void cancel_all_events_except(Event* inEvent) const{
    for (auto& [time, event] : events_) {
      if (event.get() != inEvent) {
        event->executable = false;
      }
    }
  }


  template <typename T>
  void cancel_all_events_except(Event* inEvent) const{
    for (auto& [time, event] : events_) {
      if (event.get() != inEvent && dynamic_cast<T*>(event.get()) != nullptr) {
        event->executable = false;
      }
    }
  }
};

#endif    /* DISPATCHER_H */

