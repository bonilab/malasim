/*
 * AnnualCoverageUpdateEvent.hxx
 *
 * This event class provides the ability to update the treatment coverage
 * (i.e., access to treatment) on an annual basis.
 */
#ifndef ANNUALCOVERAGEUPDATEEVENT_HXX
#define ANNUALCOVERAGEUPDATEEVENT_HXX

#include <cmath>


#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Events/Event.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "Simulation/Model.h"
#include "Treatment/ITreatmentCoverageModel.h"

class AnnualCoverageUpdateEvent : public Event {
private:
  float rate_ = 0.0;

  // Execute the annual coverage update event
  void do_execute() override {
    auto tcm_db = Model::get_treatment_coverage();

    // Iterate though and adjust the rates
    auto count = Model::get_config()->number_of_locations();
    for (auto ndx = 0; ndx < count; ndx++) {
      tcm_db->p_treatment_under_5[ndx] =
          adjust(tcm_db->p_treatment_under_5[ndx], rate_);
      tcm_db->p_treatment_over_5[ndx] =
          adjust(tcm_db->p_treatment_over_5[ndx], rate_);
    }

    // Schedule for one year from now
    auto time =
        Model::get_scheduler()->current_time()
        + TimeHelpers::number_of_days_to_next_year(Model::get_scheduler()->calendar_date);
    AnnualCoverageUpdateEvent* event =
        new AnnualCoverageUpdateEvent(rate_, time);
    Model::get_scheduler()->schedule_population_event(event);

    // Log on demand
    spdlog::debug(
        "Annual coverage update event: {} - {} {}",
        StringHelpers::date_as_string(
            date::year_month_day{Model::get_scheduler()->calendar_date}),
        rate_,
        tcm_db->p_treatment_under_5[0]);
  }

  // Update the coverage by the given rate, round up at the third decimal place
  // but clamped at 1.0 (100%)
  double adjust(double coverage, double rate) {
    coverage += (1 - coverage) * rate;
    coverage = int(coverage * pow(10, 3)) / pow(10, 3);
    return (coverage < 1.0) ? coverage : 1.0;
  }

public:
  inline static const std::string EventName = "annual_coverage_update_event";

  AnnualCoverageUpdateEvent(float rate, int start) : rate_(rate) {
    set_time(start);
  }
  ~AnnualCoverageUpdateEvent() = default;

  // Return the name of this event
  const std::string name() const override { return EventName; }
};

#endif
