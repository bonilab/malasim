// src/Population/Population.cpp

#include "Population.h"

#include <Utils/Random.h>
#include <spdlog/spdlog.h>

#include <random>
#include <Simulation/Model.h>
#include "Person/Person.h"
#include "Events/BirthdayEvent.h"

Population::Population(Model *model) : model_(model) {
}

Population::~Population() {
  persons_.clear();
}

void Population::initialize() {
  persons_.clear();
  for (int i = 0; i < 10000; ++i) {
    auto *person = new Person();
    person->initialize();
    if (model_->get_random()->random_uniform(0, 1) < 0.5) { // 50% chance to schedule the event
    const auto event = std::make_shared<BirthdayEvent>();
    event->executable = true;
    event->schedule_event(model_->get_scheduler(), person, model_->get_random()->random_uniform(0, 365));
    }
    add_person(person);
  }
}

void Population::add_person(Person* person) {
  persons_.push_back(person);
}

void Population::remove_person(Person* person) {
  persons_.erase(std::ranges::remove(persons_, person).begin(), persons_.end());
}

size_t Population::size() const {
  return persons_.size();
}

void Population::update(int current_time) {
  for (const auto& person : persons_) {
    // spdlog::info("Updating person {} with events {}", person->get_id(), person->event_queue.size());
    person->update(current_time);
  }
}