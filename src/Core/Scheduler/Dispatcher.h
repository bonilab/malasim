#ifndef DISPATCHER_H
#define    DISPATCHER_H

#include <map>
#include <uuid.h>
#include <vector>

class Event;

class Dispatcher {
public:
 // Disallow copy and assign
 Dispatcher(const Dispatcher&) = delete;
 Dispatcher& operator=(const Dispatcher&) = delete;
 Dispatcher(Dispatcher&&) = delete;
 Dispatcher& operator=(Dispatcher&&) = delete;

private:
 // std::vector<Event*> *events_;
 std::map<uuids::uuid, Event*> *events_;
 public:
  Dispatcher();

  //    Dispatcher(const Dispatcher& orig);
  virtual ~Dispatcher();

  virtual void initialize();

  virtual void add_dispatcher(Event* event);

  virtual void remove_dispatcher(Event* event);

  virtual void clear_dispatcher_events();

  virtual void update_dispatcher();

};

#endif    /* DISPATCHER_H */

