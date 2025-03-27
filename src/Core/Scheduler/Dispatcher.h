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
 void operator=(const Dispatcher&) = delete;
 Dispatcher(Dispatcher&&) = delete;
 Dispatcher& operator=(Dispatcher&&) = delete;

private:
 // std::vector<Event*> *events_;
public:
 public:
  Dispatcher();

  //    Dispatcher(const Dispatcher& orig);
  virtual ~Dispatcher();

  virtual void initialize();


};

#endif    /* DISPATCHER_H */

