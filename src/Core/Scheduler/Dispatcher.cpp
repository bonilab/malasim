/* 
 * File:   Dispatcher.cpp
 * Author: nguyentran
 * 
 * Created on May 3, 2013, 3:46 PM
 */

#include "Dispatcher.h"

#include <Population/Person/Person.h>
#include "Events/Event.h"

Dispatcher::Dispatcher() : events_(nullptr) {}

void Dispatcher::initialize() {
  events_ = new std::map<uuids::uuid, Event*>();
}

Dispatcher::~Dispatcher() {
  Dispatcher::clear_dispatcher_events();
  delete events_;
}

void Dispatcher::add_dispatcher(Event* event) {
  events_->insert({event->get_id(), event});
}

void Dispatcher::remove_dispatcher(Event* event) {
  events_->erase(event->get_id());
}

void Dispatcher::clear_dispatcher_events() {
  if (events_== nullptr) return;
  if (events_->empty()) return;
  for(auto & event : *events_) {
    delete event.second;
  }
  events_->clear();
}

void Dispatcher::update_dispatcher() {
  // events_->shrink_to_fit();
}
