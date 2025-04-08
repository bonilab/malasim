#include "PersonIndexAll.h"

PersonIndexAll::PersonIndexAll() = default;

PersonIndexAll::~PersonIndexAll() { v_person_.clear(); }

void PersonIndexAll::add(Person* person) {
  v_person_.push_back(person);
  person->PersonIndexAllHandler::set_index(v_person_.size() - 1);
}

void PersonIndexAll::remove(Person* person) {
  // move the last element to current position and remove the last holder
  v_person_.back()->PersonIndexAllHandler::set_index(person->PersonIndexAllHandler::get_index());
  v_person_[person->PersonIndexAllHandler::get_index()] = v_person_.back();
  v_person_.pop_back();
  person->PersonIndexAllHandler::set_index(-1);
  //    delete p;
  //    p = nullptr;
}

std::size_t PersonIndexAll::size() const { return v_person_.size(); }

void PersonIndexAll::notify_change(Person* person, const Person::Property &property,
                                   const void* old_value, const void* new_value) {}

void PersonIndexAll::update() { v_person_.shrink_to_fit(); }
