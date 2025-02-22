#ifndef NOVELDRUGSWITCHINGSTRATEGY_H
#define NOVELDRUGSWITCHINGSTRATEGY_H

#include <vector>
#include "IStrategy.h"
#include "NestedMFTStrategy.h"

class NovelDrugIntroductionStrategy : public NestedMFTStrategy {
  //disallow copy and assign and move
  NovelDrugIntroductionStrategy(const NovelDrugIntroductionStrategy&) = delete;
  void operator=(const NovelDrugIntroductionStrategy&) = delete;
  NovelDrugIntroductionStrategy(NovelDrugIntroductionStrategy&&) = delete;
  NovelDrugIntroductionStrategy& operator=(NovelDrugIntroductionStrategy&&) = delete;

public:
  bool is_switched{false};
  int newly_introduced_strategy_id{0};
  double replacement_fraction{0.0};
  int replacement_duration{0};
  double tf_threshold{0.1};

  NovelDrugIntroductionStrategy();

  virtual ~NovelDrugIntroductionStrategy() = default;

  std::string to_string() const override;

  void monthly_update() override;
};

#endif // NOVELDRUGSWITCHINGSTRATEGY_H
