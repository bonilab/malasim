#include "TurnOffMutationEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Utils/Helpers/StringHelpers.h"

TurnOffMutationEvent::TurnOffMutationEvent(const int &at_time) {
  set_time(at_time);
}

void TurnOffMutationEvent::do_execute() {
  Model::get_config()->get_genotype_parameters().set_mutation_probability_per_locus(0.0);
  spdlog::info("{}: turn mutation off",
    StringHelpers::date_as_string(date::year_month_day{Model::get_scheduler()->calendar_date}));
}
