#include "NonInfantImmuneComponent.h"
#include "Simulation/Model.h"
#include "Configuration/Config.h"


//OBJECTPOOL_IMPL(NonInfantImmuneComponent)

NonInfantImmuneComponent::NonInfantImmuneComponent(ImmuneSystem *immune_system) : ImmuneComponent(immune_system) {
}

NonInfantImmuneComponent::~NonInfantImmuneComponent() {
}

double NonInfantImmuneComponent::get_acquire_rate(const int &age) const {
  //    return FastImmuneComponent::acquireRate;

  return (age > 80) ? Model::get_instance().get_config()->get_immune_system_parameters().acquire_rate_by_age[80]
                    : Model::get_instance().get_config()->get_immune_system_parameters().acquire_rate_by_age[age];

}

double NonInfantImmuneComponent::get_decay_rate(const int &age) const {
  return Model::get_instance().get_config()->get_immune_system_parameters().decay_rate;
}