/**
 * IntroduceMutantRasterEvent.hxx
 *
 * Introduce mutant genotypes into the simulation by randomly selecting infected
 * individuals and switching the relevant genotype from the wild type to the
 * specified mutant based upon the raster supplied.
 */
#ifndef INTRODUCEMUTANTRASTEREVENT_HXX
#define INTRODUCEMUTANTRASTEREVENT_HXX

#include <utility>
#include "Core/Scheduler/Scheduler.h"

#include "IntroduceMutantEventBase.h"
#include "Utils/Helpers/StringHelpers.h"

class IntroduceMutantRasterEvent : public IntroduceMutantEventBase {
public:
  //disallow copy and move
  IntroduceMutantRasterEvent(const IntroduceMutantRasterEvent &) = delete;
  IntroduceMutantRasterEvent(IntroduceMutantRasterEvent &&) = delete;

private:
  std::vector<int> locations_;

  void do_execute() override {
    // Use the locations to calculate the target fraction of mutations and
    // perform them
    auto target_fraction = calculate(locations_);
    auto count =
        (target_fraction > 0) ? mutate(locations_, target_fraction) : 0;

    // Log the event's operation
    for (auto allele : alleles_) {
      spdlog::info("{}: Introduce mutant raster event chromosome {} locus {} allele {} fraction: {} count: {}",
                    StringHelpers::date_as_string(date::year_month_day{scheduler->calendar_date}),
                   std::get<0>(allele), std::get<1>(allele), std::get<2>(allele),
                   target_fraction, count);
    }
  }

public:
  inline static const std::string EventName = "introduce_mutant_raster_event";

  IntroduceMutantRasterEvent(const int &time, std::vector<int> locations,
                             const double &fraction,
                            const  std::vector<std::tuple<int,int,char>> &alleles)
      : IntroduceMutantEventBase(fraction, alleles),
        locations_(std::move(locations)) {
    this->time = time;
  }

  ~IntroduceMutantRasterEvent() override = default;

  // Return the name of this event
  const std::string name() const override { return EventName; }
};

#endif
