/*
 * AnnualBetaUpdateEvent.hxx
 *
 * This event class provides the ability to alter the beta parameter on an
 * annual basis.
 */
#ifndef ANNUALBETAUPDATEEVENT_HXX
#define ANNUALBETAUPDATEEVENT_HXX

#include <math.h>

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Events/Event.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "Simulation/Model.h"
#include "Utils/Helpers/StringHelpers.h"

class AnnualBetaUpdateEvent : public WorldEvent {
private:
  float rate_ = 0.0;

  // Execute the annual beta update event
  void do_execute() override {
    // Grab a reference to the location_db to work with
    auto &location_db = Model::get_config()->location_db();

    // Iterate through and adjust the betas
    auto count = Model::get_config()->number_of_locations();
    for (auto ndx = 0; ndx < count; ndx++) {
      location_db[ndx].beta = adjust(location_db[ndx].beta, rate_);
    }

    // Schedule for one year from now
    auto time = Model::get_scheduler()->get_days_to_next_year();
    auto event = std::make_unique<AnnualBetaUpdateEvent>(rate_, time);
    Model::get_scheduler()->schedule_population_event(std::move(event));

    // Log on demand
    spdlog::debug(
        "Annual beta update event: {} - {} {}",
        Model::get_scheduler()->get_current_date_string(),
        rate_,
        location_db[0].beta);
  }

  // Update the bete by the given rate, round up ate the fifth decimal place but
  // clamp at 0.0
  double adjust(double beta, double rate) {
    beta += (beta * rate);
    beta = int(beta * pow(10, 5)) / pow(10, 5);
    return (beta < 0.0) ? 0.0 : beta;
  }

public:
  inline static const std::string EventName = "annual_beta_update_event";

  AnnualBetaUpdateEvent(float rate, int start) : rate_(rate) { set_time(start); }
  ~AnnualBetaUpdateEvent() = default;

  // Return the name of this event
  const std::string name() const override { return EventName; }
};

#endif
