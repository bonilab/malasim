#include "PersonIndexAll.h"

PersonIndexAll::PersonIndexAll() = default;

PersonIndexAll::~PersonIndexAll() { v_person_.clear(); }

void PersonIndexAll::add(std::unique_ptr<Person> person) {
  v_person_.push_back(std::move(person));
  v_person_.back()->PersonIndexAllHandler::set_index(v_person_.size() - 1);
}

void PersonIndexAll::remove(Person* person) {
  if (v_person_.empty()) {
    throw std::runtime_error("PersonIndexAll is empty");
  }
  if (person->PersonIndexAllHandler::get_index() >= v_person_.size()) {
    throw std::runtime_error("Invalid index in PersonIndexAll::remove");
  }

  // Optimization for removing the last element to avoid self-move (optional)
  size_t index_to_remove = person->PersonIndexAllHandler::get_index();
  if (index_to_remove == v_person_.size() - 1) {
    v_person_.pop_back();
  } else {
    // Original logic: move the last element to current position
    v_person_.back()->PersonIndexAllHandler::set_index(index_to_remove);
    v_person_[index_to_remove] = std::move(v_person_.back());
    v_person_.pop_back();
  }
}

void PersonIndexAll::clear() { v_person_.clear(); }

std::size_t PersonIndexAll::size() const { return v_person_.size(); }

void PersonIndexAll::update() { v_person_.shrink_to_fit(); }
