#include "ImmuneComponent.h"
#include "ImmuneSystem.h"
#include "Population/Person/Person.h"
#include "Population/Population.h"
#include "Simulation/Model.h"
#include "Configuration/Config.h"
#include "Utils/Random.h"
#include <cmath>

#include "Core/Scheduler/Scheduler.h"

ImmuneComponent::ImmuneComponent(ImmuneSystem *immune_system) : immune_system_(immune_system), latest_value_(0.0) {}

ImmuneComponent::~ImmuneComponent() {
  immune_system_ = nullptr;
}

double ImmuneComponent::get_current_value() {
  const auto currentTime = Model::get_scheduler()->current_time();
  auto temp = 0.0;
  if (immune_system_!=nullptr) {
    if (immune_system_->person()!=nullptr) {
      const auto duration = currentTime - immune_system_->person()->get_latest_update_time();

      const auto age = immune_system_->person()->get_age();
      if (immune_system_->increase()) {
        //increase I(t) = 1 - (1-I0)e^(-b1*t)

        temp = 1 - (1 - latest_value_)*exp(-get_acquire_rate(age)*duration);

        //        temp = lastImmuneLevel;
        //        double b1 = GetAcquireRate(immuneSystem->person->age);
        //        for (int i = 0; i < duration; i++) {
        //            temp+= b1*(1-temp);
        //        }

      } else {
        //decrease I(t) = I0 * e ^ (-b2*t);
        temp = latest_value_*exp(-get_decay_rate(age)*duration);
        temp = (temp < 0.00001) ? 0.0 : temp;
      }

    }
  }
  return temp;
}

void ImmuneComponent::update() {
  latest_value_ = get_current_value();

}

void ImmuneComponent::draw_random_immune() {
  const auto ims = Model::get_config()->get_immune_system_parameters();
  latest_value_ = Model::get_random()->random_beta(ims.alpha_immune, ims.beta_immune);
}
