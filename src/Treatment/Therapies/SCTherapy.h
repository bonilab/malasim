#ifndef SCTHERAPY_H
#define    SCTHERAPY_H

#include "Therapy.h"
#include <vector>

class DrugType;

class SCTherapy : public Therapy {
  //disallow copy and assign
  SCTherapy(const SCTherapy &orig) = delete;
  void operator=(const SCTherapy &orig) = delete;

private:
  bool full_compliance_;
  int max_dosing_day;

public:
  bool full_compliance() const {
    return full_compliance_;
  }
  void set_full_compliance(bool full_compliance);

public:
  int artemisinin_id;
  std::vector<int> dosing_day;
  std::vector<double> pr_completed_days;

public:

  SCTherapy()
      : Therapy(),
        dosing_day{},
        artemisinin_id{-1},
        full_compliance_{false},
        max_dosing_day{-1} {};
    ~SCTherapy() override = default;

    // Add the drug, note if it is artemisinin or not.
    void add_drug(int drug_id) override;

    // Calculate the max dosing day, expected to be called by TherapyBuilder
    void calculate_max_dosing_day();

    // Return the artemisinin id or -1 if this treatment is not an artemisinin.
    int get_artemisinin_id() const { return artemisinin_id; }

    // Return the maximum dosing day
    int get_max_dosing_day() const;

    void print(std::ostream &os) const;
};

#endif    /* SCTHERAPY_H */

