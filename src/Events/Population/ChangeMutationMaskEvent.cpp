#include "ChangeMutationMaskEvent.h"

#include "Configuration/Config.h"
#include "Utils/Helpers/StringHelpers.h"

ChangeMutationMaskEvent::ChangeMutationMaskEvent(const std::string &mask, const int &at_time)
    : mask_{mask}{
  time = at_time;
}

void ChangeMutationMaskEvent::do_execute() {
  Model::get_config()->get_genotype_parameters().set_mutation_mask(mask_);
  spdlog::info("{}: change mutation mask to {}",
    StringHelpers::date_as_string(date::year_month_day{Model::get_scheduler()->calendar_date}), mask_);
}