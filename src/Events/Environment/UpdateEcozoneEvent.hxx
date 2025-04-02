/*
 * UpdateEcozoneEvent.hxx
 *
 * Update the ecozone from the previous type to the new type.
 */
#ifndef UPDATEECOZONEEVENT_H
#define UPDATEECOZONEEVENT_H

#include "Configuration/Config.h"
#include "Configuration/SeasonalitySettings.h"
#include "Utils/TypeDef.hxx"
#include "Events/Event.h"
#include "Simulation/Model.h"
#include <spdlog/spdlog.h>

class UpdateEcozoneEvent : public Event {
private:
  int from_;
  int to_;

  // Execute the import event
  void do_execute() override {
    // Scan all the locations, if they match the old values, then replace them
    // with the new
    spdlog::info("Updating ecozone from {} to {}", from_, to_);

    // Defer to the object for the actual update
    auto seasons =Model::get_config()->get_seasonality_settings().get_seasonal_equation();
    // if (seasons == nullptr) {
    //   throw std::runtime_error(
    //       "Configuration called for seasonality to be updated with a mode that "
    //       "does not support it.");
    // }
    seasons->update_seasonality(from_, to_);
  }

public:
  inline static const std::string EventName = "update_ecozone_event";

  UpdateEcozoneEvent(int from, int to, int start) : from_(from), to_(to) {
    set_time(start);
  }

  ~UpdateEcozoneEvent() override = default;

  const std::string name() const override { return EventName; }
};

#endif
