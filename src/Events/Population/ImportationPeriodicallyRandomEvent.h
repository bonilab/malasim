/*
 * ImportationPeriodicallyRandomEvent.h
 *
 * Import the indicated genotype on a populated weighted basis at the time
 * step indicated, reschedule the event to occur periodically afterwards.
 */
#ifndef IMPORTATIONPERIODICALLYRANDOMEVENT_HXX
#define IMPORTATIONPERIODICALLYRANDOMEVENT_HXX

#include <date/date.h>

#include "Events/Event.h"
#include "Population/Person/Person.h"

/**
 * @brief ImportationPeriodicallyRandomEvent is a class that imports infections
 * on a population weighted basis at the time step indicated, rescheduling the
 * event to occur periodically afterwards.
 * Import for a particular month of the year and reschedule for the same month
 * of the following year.
 */
class ImportationPeriodicallyRandomEvent : public WorldEvent {
private:
  int count_ = 0; // Number of infections to inflict per month
  int genotypeId_ = 0; // Genotype id to inflict
  double log_parasite_density_ = 0.0; // Log parasite density to inflict

  // Execute the import event
  void do_execute() override;

  // Get a random index to perform the importation event at, the population
  // is used to weight the random pull
  static std::size_t get_location();

  // Inflict the act infection upon the individual
  void infect(Person* person, int genotypeId) const;

public:
  inline static const std::string EventName =
      "importation_periodically_random_event";

  ImportationPeriodicallyRandomEvent(int genotypeId, int start, int count,
                                     double log_parasite_density)
      : genotypeId_(genotypeId), count_(count) {
    set_time(start);
    log_parasite_density_ = log_parasite_density;
  }
  ~ImportationPeriodicallyRandomEvent() override = default;

  // Return the name of this event
  const std::string name() const override { return EventName; }
};

#endif
