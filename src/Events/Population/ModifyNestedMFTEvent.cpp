#include "ModifyNestedMFTEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Treatment/Strategies/IStrategy.h"
#include "Treatment/Strategies/NestedMFTMultiLocationStrategy.h"
#include "Treatment/Strategies/NestedMFTStrategy.h"
#include "Utils/Helpers/StringHelpers.h"

ModifyNestedMFTEvent::ModifyNestedMFTEvent(const int &at_time,
                                           const int &strategy_id)
    : strategy_id(strategy_id) {
  time = at_time;
}

void ModifyNestedMFTEvent::do_execute() {
  IStrategy* new_strategy = nullptr;
  if (Model::get_treatment_strategy()->type == IStrategy::NestedMFTMultiLocation) {
    new_strategy = Model::get_config()->strategy_db()[strategy_id];
    dynamic_cast<NestedMFTMultiLocationStrategy*>(Model::get_treatment_strategy())
        ->strategy_list[0] = new_strategy;
    new_strategy->adjust_started_time_point(Model::get_scheduler()->current_time());
  }

  if (Model::get_treatment_strategy()->type == IStrategy::NestedMFT) {
    new_strategy = Model::get_config()->strategy_db()[strategy_id];
    dynamic_cast<NestedMFTStrategy*>(Model::get_treatment_strategy())
        ->strategy_list[0] = new_strategy;
    new_strategy->adjust_started_time_point(Model::get_scheduler()->current_time());
  }

  if (new_strategy == nullptr) {
      spdlog::error("Modify Nested MFT Event error with null ptr.");
    exit(EXIT_FAILURE);
  }

  spdlog::info("{}: ModifyNestedMFTEvent: {}",
               StringHelpers::date_as_string(
                   date::year_month_day{Model::get_scheduler()->calendar_date}),
               new_strategy->name);
}
