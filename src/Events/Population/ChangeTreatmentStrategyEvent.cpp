#include "ChangeTreatmentStrategyEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Treatment/Strategies/IStrategy.h"
#include "Utils/Helpers/StringHelpers.h"

ChangeTreatmentStrategyEvent::ChangeTreatmentStrategyEvent(const int &at_time,
                                                           const int &strategy_id)
    : strategy_id(strategy_id) {
  time = at_time;
}

void ChangeTreatmentStrategyEvent::do_execute() {
  Model::get_instance().set_treatment_strategy(strategy_id);
  spdlog::info("Day {}: Change treatment strategy to {}",
               Model::get_scheduler()->current_time(),
               Model::get_treatment_strategy()->name);
}
