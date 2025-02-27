#pragma once

#include <string>

// #include "Core/ObjectPool.h"
#include "Events/Event.h"

class IntroduceAmodiaquineMutantEvent : public Event {
public:
  //disallow copy, assign and move
  IntroduceAmodiaquineMutantEvent(const IntroduceAmodiaquineMutantEvent &) = delete;
  void operator=(const IntroduceAmodiaquineMutantEvent &) = delete;
  IntroduceAmodiaquineMutantEvent(IntroduceAmodiaquineMutantEvent &&) = delete;
  void operator=(IntroduceAmodiaquineMutantEvent &&) = delete;

  // OBJECTPOOL(IntroduceAQMutantEvent)
private:
  int location_;
  double fraction_;
  std::vector<std::tuple<int,int,char>> alleles_;

public:
  explicit IntroduceAmodiaquineMutantEvent(const int &location = -1,
                                  const int &execute_at = -1,
                                  const double &fraction = 0,
                                    const std::vector<std::tuple<int,int,char>> &allele_map = {});

  //    ImportationEvent(const ImportationEvent& orig);
  ~IntroduceAmodiaquineMutantEvent() override;

  std::string name() override { return "IntroduceAmodiaquineMutantEvent"; }

private:
  void execute() override;
};
