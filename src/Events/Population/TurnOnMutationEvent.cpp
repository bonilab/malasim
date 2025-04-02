#include "TurnOnMutationEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Utils/Helpers/StringHelpers.h"

TurnOnMutationEvent::TurnOnMutationEvent(const int &at_time, const double &mutation_probability) :
  mutation_probability{mutation_probability} {
  time = at_time;
}

void TurnOnMutationEvent::do_execute() {
  Model::get_config()->get_genotype_parameters().set_mutation_probability_per_locus(mutation_probability);
    spdlog::info("{}: turn mutation on with probability {}",
        StringHelpers::date_as_string(date::year_month_day{scheduler->calendar_date}),
        mutation_probability);
}
