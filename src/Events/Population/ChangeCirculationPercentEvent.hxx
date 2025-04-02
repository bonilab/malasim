/*
 * ChangeCirculationPercentEvent.hxx
 *
 * Define and implement an event to change the daily percentage of the
 * population circulating.
 */
#ifndef CHANGE_CIRCULATION_PERCENT_EVENT_HXX
#define CHANGE_CIRCULATION_PERCENT_EVENT_HXX

#include "Configuration/Config.h"
#include "Events/Event.h"
#include "Simulation/Model.h"

class ChangeCirculationPercentEvent : public Event {
private:
  float rate_ = 0.0;

  // Execute the event to change the circulation percentage
  void do_execute() override {
    MovementSettings::CirculationInfo circulation_info =
        Model::get_config()->get_movement_settings().get_circulation_info();
    circulation_info.set_circulation_percent(rate_);
    Model::get_config()->get_movement_settings().set_circulation_info(
        circulation_info);

    // Log on demand
    spdlog::debug(
        "Change circulation percent event: {} - {}",
        StringHelpers::date_as_string(
            date::year_month_day{Model::get_scheduler()->calendar_date}),
        rate_);
  }

public:
  inline static const std::string EventName =
      "change_circulation_percent_event";

  ChangeCirculationPercentEvent(float rate, int start) : rate_(rate) {
    set_time(start);
  }
  ~ChangeCirculationPercentEvent() override = default;

  // Return the name of this event
  const std::string name() const override { return EventName; }
};

#endif
