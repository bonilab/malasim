#ifndef DRUGTYPE_H
#define DRUGTYPE_H

#include "Utils/TypeDef.hxx"

struct ResistantAALocation {
  int chromosome_id { -1 };
  int gene_id { -1 };
  int aa_id { -1 };
  int aa_index_in_aa_string { -1 };
  bool is_copy_number { false };
};

class Genotype;
class Config;

class DrugType {
public:
  //disallow copy and assign
  DrugType(const DrugType&) = delete;
  void operator=(const DrugType&) = delete;

  int id() const { return id_; }
  void set_id(int id) { id_ = id; }

  std::string name() const { return name_; }
  void set_name(std::string name) { name_ = name; }

  double drug_half_life() const { return drug_half_life_; }
  void set_drug_half_life(double drug_half_life) { drug_half_life_ = drug_half_life; }

  double maximum_parasite_killing_rate() const { return maximum_parasite_killing_rate_; }
  void set_maximum_parasite_killing_rate(double maximum_parasite_killing_rate) { maximum_parasite_killing_rate_ = maximum_parasite_killing_rate; }

  DoubleVector& age_group_specific_drug_concentration_sd()  { return age_group_specific_drug_concentration_sd_; }
  void set_age_group_specific_drug_concentration_sd(DoubleVector age_group_specific_drug_concentration_sd) { age_group_specific_drug_concentration_sd_ = age_group_specific_drug_concentration_sd; }

  DoubleVector& age_specific_drug_absorption() { return age_specific_drug_absorption_; }
  void set_age_specific_drug_absorption(DoubleVector age_specific_drug_absorption) { age_specific_drug_absorption_ = age_specific_drug_absorption; }

  double k() const { return k_; }
  void set_k(double k) { k_ = k; }

  double cut_off_percent() const { return cut_off_percent_; }
  void set_cut_off_percent(double cut_off_percent) { cut_off_percent_ = cut_off_percent; }

private:
  int id_ { -1 };
  std::string name_ {};
  double drug_half_life_ { 0 };
  double maximum_parasite_killing_rate_ { 0 };
  std::vector<double> age_group_specific_drug_concentration_sd_ {};
  std::vector<double> age_specific_drug_absorption_ {};
  double k_ { 0 };
  double cut_off_percent_ { 0 };

public:
  double base_EC50 { 0 };
  std::vector<ResistantAALocation> resistant_aa_locations {};

public:
  DrugType();

  virtual ~DrugType();

  virtual double get_parasite_killing_rate_by_concentration(const double &concentration, const double &EC50_power_n);

  virtual double n();

  virtual void set_n(const double &n);

  int get_total_duration_of_drug_activity(const int &dosing_days) const;

  void populate_resistant_aa_locations();

private:
  double n_;
  //    double EC50_;
};

#endif /* DRUGTYPE_H */
