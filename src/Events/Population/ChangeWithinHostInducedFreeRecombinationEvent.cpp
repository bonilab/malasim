#include "ChangeWithinHostInducedFreeRecombinationEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Helpers/StringHelpers.h"

ChangeWithinHostInducedFreeRecombinationEvent::ChangeWithinHostInducedFreeRecombinationEvent(const bool& value,
                                                                                             const int& at_time)
    : value { value } {
  time = at_time;
}
void ChangeWithinHostInducedFreeRecombinationEvent::execute() {
  Model::get_config()->get_mosquito_parameters().set_within_host_induced_free_recombination(value);
  spdlog::info("{}: Change within host induced free recombination to {}",
    StringHelpers::date_as_string(date::year_month_day{scheduler->calendar_date}), value);
}
