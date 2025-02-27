/*
 * RotateStrategyEvent.cpp
 *
 * Implement the class to rotate treatment strategies on a regular basis.
 */
#include "RotateStrategyEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Treatment/Strategies/IStrategy.h"
#include "Utils/Helpers/StringHelpers.h"

RotateStrategyEvent::RotateStrategyEvent(int at_time, int years,
                                         int new_strategy_id,
                                         int next_strategy_id)
    : years_(years),
      new_strategy_id_(new_strategy_id),
      next_strategy_id_(next_strategy_id) {
  time = at_time;
}

void RotateStrategyEvent::execute() {
  // Change to the new treatment strategy
  Model::get_instance().get_model()->set_treatment_strategy(new_strategy_id_);
  spdlog::info(
      "{}: Switching treatment strategy to {} ",
      StringHelpers::date_as_string(date::year_month_day{Model::get_instance().get_scheduler()->calendar_date}),
      Model::get_instance().get_treatment_strategy()->name);

  // Queue the next iteration of this event
  auto next_time = Model::get_instance().get_scheduler()->current_time() + (years_ * 365);
  auto* event = new RotateStrategyEvent(next_time, years_, next_strategy_id_,
                                        new_strategy_id_);
  scheduler->schedule_population_event(event);
}
