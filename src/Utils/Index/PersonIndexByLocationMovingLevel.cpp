#include "PersonIndexByLocationMovingLevel.h"
#include "Configuration/Config.h"
#include "Simulation/Model.h"
#include <cassert>

PersonIndexByLocationMovingLevel::PersonIndexByLocationMovingLevel(const int &no_location, const int &no_level) {
  Initialize(no_location, no_level);
}

PersonIndexByLocationMovingLevel::~PersonIndexByLocationMovingLevel() {
  vPerson_.clear();
}

void PersonIndexByLocationMovingLevel::Initialize(const int &no_location, const int &no_level) {
  vPerson_.clear();

  PersonPtrVector ppv;
  PersonPtrVector2 ppv2;
  ppv2.assign(no_level, ppv);

  vPerson_.assign(no_location, ppv2);
}

void PersonIndexByLocationMovingLevel::add(Person *p) {
  assert(vPerson_.size() > p->get_location() && p->get_location() >= 0);
  assert(vPerson_[p->get_location()].size() > p->get_moving_level());
  add(p, p->get_location(), p->get_moving_level());
}

void PersonIndexByLocationMovingLevel::remove(Person *p) {
  remove_without_set_index(p);
  p->PersonIndexByLocationMovingLevelHandler::set_index(-1);
}

void PersonIndexByLocationMovingLevel::notify_change(Person *p, const Person::Property &property, const void *oldValue,
                                                     const void *newValue) {
  switch (property) {
    case Person::LOCATION:change_property(p, *(int *) newValue, p->get_moving_level());
      break;
    case Person::MOVING_LEVEL:change_property(p, p->get_location(), *(int *) newValue);
      break;
    default:break;
  }
}

std::size_t PersonIndexByLocationMovingLevel::size() const {
  return 0;
}

void PersonIndexByLocationMovingLevel::add(Person *p, const int &location, const int &moving_level) {
  vPerson_[location][moving_level].push_back(p);
  p->PersonIndexByLocationMovingLevelHandler::set_index(vPerson_[location][moving_level].size() - 1);
}

void PersonIndexByLocationMovingLevel::remove_without_set_index(Person *p) {
  vPerson_[p->get_location()][p->get_moving_level()].back()->PersonIndexByLocationMovingLevelHandler::set_index(
      p->PersonIndexByLocationMovingLevelHandler::get_index());
  vPerson_[p->get_location()][p->get_moving_level()][p->PersonIndexByLocationMovingLevelHandler::get_index()] =
      vPerson_[p->get_location()][p->get_moving_level()].back();
  vPerson_[p->get_location()][p->get_moving_level()].pop_back();
}

void PersonIndexByLocationMovingLevel::change_property(Person *p, const int &location, const int &moving_level) {
  //remove from old position
  remove_without_set_index(p); //to save 1 set and improve performance since the index of p will changed when add

  //add to new position
  add(p, location, moving_level);
}

void PersonIndexByLocationMovingLevel::update() {
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    for (int ml = 0; ml < Model::get_config()->get_movement_settings().get_circulation_info().get_number_of_moving_levels(); ml++) {
      std::vector<Person *>(vPerson_[location][ml]).swap(vPerson_[location][ml]);
    }
  }
}
