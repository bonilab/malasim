/**
 * IntroduceMutantEventBase.h
 *
 * Base class for introduce mutant event.
 */
#ifndef INTRODUCEMUTANTEVENTBASE_H
#define INTRODUCEMUTANTEVENTBASE_H

#include <vector>

// #include "Core/PropertyMacro.h"
#include "Events/Event.h"

class IntroduceMutantEventBase : public Event {
protected:
  double fraction_;
  std::vector<std::tuple<int,int,char>> alleles_;

  IntroduceMutantEventBase(const double &fraction,
                           const std::vector<std::tuple<int,int,char>> &alleles)
      : fraction_(fraction), alleles_(alleles) {}

  double calculate(std::vector<int> &locations) const;
  int mutate(std::vector<int> &locations, double target_fraction) const;
};

#endif
