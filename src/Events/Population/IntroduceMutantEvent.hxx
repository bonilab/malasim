/**
 * IntroduceMutantEvent.hxx
 *
 * Introduce mutant genotypes into the simulation by randomly selecting infected
 * individuals and switching the relevant genotype from the wild type to the
 * specified mutant based upon the district id supplied.
 */
#ifndef INTRODUCEMUTANTEVENT_HXX
#define INTRODUCEMUTANTEVENT_HXX

#include <string>

// #include "Core/ObjectPool.h"
#include "Core/Scheduler/Scheduler.h"
#include "Events/Event.h"
#include "Spatial/GIS/SpatialData.h"
#include "IntroduceMutantEventBase.h"
#include "Utils/Helpers/StringHelpers.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"

class IntroduceMutantEvent : public IntroduceMutantEventBase {
public:
  //disallow copy and move
  IntroduceMutantEvent(const IntroduceMutantEvent &) = delete;
  IntroduceMutantEvent(IntroduceMutantEvent &&) = delete;
private:
  int district_;

  void execute() override {
    // Calculate the target fraction of the district infections and perform them
    // as needed
    auto locations =
        SpatialData::get_instance().get_district_locations(district_);
    double target_fraction = calculate(locations);
    auto count = (target_fraction > 0) ? mutate(locations, target_fraction) : 0;

    // Log the event's operation
    spdlog::info(
        "Introduce mutant event: {} : Introduce mutant event, target fraction: "
        "{}, mutations: {}",
        StringHelpers::date_as_string(date::year_month_day{scheduler->calendar_date}), target_fraction, count);
  }

public:
  inline static const std::string EventName = "introduce_mutant_event";

  explicit IntroduceMutantEvent(const int &time, const int &district,
                                const double &fraction,
                                const std::vector<std::tuple<int,int,char>> &alleles)
      : IntroduceMutantEventBase(fraction, alleles),
        district_(district) {
    this->time = time;
  }

  ~IntroduceMutantEvent() override = default;

  // Return the name of this event
  std::string name() override { return EventName; }
};

#endif
