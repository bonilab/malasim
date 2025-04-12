#pragma once

#include <string>
#include <vector>
#include <tuple>

// #include "Core/ObjectPool.h"
#include "Events/Event.h"

class IntroduceLumefantrineMutantEvent : public WorldEvent {
public:
    // Disallow copy
    IntroduceLumefantrineMutantEvent(const IntroduceLumefantrineMutantEvent&) = delete;
    IntroduceLumefantrineMutantEvent& operator=(const IntroduceLumefantrineMutantEvent&) = delete;

    // Disallow move
    IntroduceLumefantrineMutantEvent(IntroduceLumefantrineMutantEvent&&) = delete;
    IntroduceLumefantrineMutantEvent& operator=(IntroduceLumefantrineMutantEvent&&) = delete;

  // OBJECTPOOL(IntroduceLumefantrineMutantEvent)

private:
  int location_;
  double fraction_;
  std::vector<std::tuple<int,int,char>> alleles_;

public:
  explicit IntroduceLumefantrineMutantEvent(const int &location = -1,
                                              const int &execute_at = -1,
                                            const double &fraction = 0,
                                            const std::vector<std::tuple<int,int,char>> &allele_map = {});

  //    ImportationEvent(const ImportationEvent& orig);
  ~IntroduceLumefantrineMutantEvent() override;

  const std::string name() const override { return "IntroduceLumefantrineMutantEvent"; }

private:
  void do_execute() override;
};
