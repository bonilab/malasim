#include "ChangeInterruptedFeedingRateEvent.h"

#include "Configuration/Config.h"
#include "Utils/Helpers/StringHelpers.h"

ChangeInterruptedFeedingRateEvent::ChangeInterruptedFeedingRateEvent(const int &location, const double &ifr, const int &at_time)
    : location{location},
      ifr{ifr} {
  time = at_time;
}

void ChangeInterruptedFeedingRateEvent::execute() {
  Model::get_instance().get_config()->get_spatial_settings().location_db[location].mosquito_ifr = ifr;
  spdlog::info("{}: Change interrupted feeding rate at location {} to {}",
    StringHelpers::date_as_string(date::year_month_day{scheduler->calendar_date}), location,ifr);
}