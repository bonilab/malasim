#include "InfantImmuneComponent.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"
#include "ImmuneSystem.h"
#include <cmath>

#include "Core/Scheduler/Scheduler.h"

InfantImmuneComponent::InfantImmuneComponent(ImmuneSystem *immune_system) : ImmuneComponent(immune_system) {}

InfantImmuneComponent::~InfantImmuneComponent() = default;

double InfantImmuneComponent::get_acquire_rate(const int &age) const {
  return 0;
}

double InfantImmuneComponent::get_decay_rate(const int &age) const {
  return 0.0315;
}

double InfantImmuneComponent::get_current_value() {
  const auto current_time = Model::get_scheduler()->current_time();
  auto temp = 0.0;
  if (immune_system()!=nullptr) {
    if (immune_system()->person()!=nullptr) {
      const auto duration = current_time - immune_system()->person()->get_latest_update_time();
      //decrease I(t) = I0 * e ^ (-b2*t);
      temp = latest_value()*exp(-get_decay_rate(0)*duration);
    }
  }
  return temp;
}
