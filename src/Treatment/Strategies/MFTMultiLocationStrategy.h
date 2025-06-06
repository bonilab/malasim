#ifndef POMS_MFTDIFFERENTDISTRIBUTIONBYLOCATIONSTRATEGY_H
#define POMS_MFTDIFFERENTDISTRIBUTIONBYLOCATIONSTRATEGY_H

#include "IStrategy.h"
#include "Utils/TypeDef.h"

class MFTMultiLocationStrategy : public IStrategy {
  //disallow copy and assign and move
  MFTMultiLocationStrategy(const MFTMultiLocationStrategy &) = delete;
  void operator=(const MFTMultiLocationStrategy &) = delete;
  MFTMultiLocationStrategy(MFTMultiLocationStrategy &&) = delete;
  MFTMultiLocationStrategy &operator=(MFTMultiLocationStrategy &&) = delete;

 public:
  std::vector<Therapy *> therapy_list;
  // DoubleVector2 distribution_by_location;
  DoubleVector2 distribution;
  DoubleVector2 start_distribution;
  DoubleVector2 peak_distribution;
  int starting_time{0};
  int peak_after{0};

  MFTMultiLocationStrategy();

  //    MFTStrategy(const MFTStrategy& orig);
  ~MFTMultiLocationStrategy() override;

  void add_therapy(Therapy *therapy) override;

  Therapy *get_therapy(Person *person) override;

  std::string to_string() const override;

  void update_end_of_time_step() override;

  void adjust_started_time_point(const int &current_time) override;

  void monthly_update() override;

};

#endif //POMS_MFTDIFFERENTDISTRIBUTIONBYLOCATIONSTRATEGY_H
