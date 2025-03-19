#ifndef MACTHERAPY_H
#define MACTHERAPY_H

#include <vector>
#include "Therapy.h"

class MACTherapy : public Therapy {
  //disallow copy and assign
  MACTherapy(const MACTherapy &orig) = delete;
  void operator=(const MACTherapy &orig) = delete;

public:
  [[nodiscard]] const std::vector<int> &get_therapy_ids() const {
    return therapy_ids_;
  }
  void set_therapy_ids(const std::vector<int> &therapy_ids) {
    therapy_ids_ = therapy_ids;
  }
  [[nodiscard]] const std::vector<int> &get_start_at_days() const {
    return start_at_days_;
  }
  void set_start_at_days(const std::vector<int> &start_at_days) {
    start_at_days_ = start_at_days;
  }

private:
  std::vector<int> therapy_ids_;
  std::vector<int> start_at_days_;

 public:
  MACTherapy();

  //    MACTherapy(const MACTherapy& orig);
  virtual ~MACTherapy();

  void add_therapy_id(const int &therapy_id);

  void add_schedule(const int &start_at_day);

};

#endif    /* MACTHERAPY_H */
