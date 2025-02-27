#ifndef TREATMENTCOVERAGEMODEL_H
#define TREATMENTCOVERAGEMODEL_H

#include <vector>
#include <yaml-cpp/yaml.h>

#include "Configuration/Config.h"

class ITreatmentCoverageModel {
  //disallow copy and move constructors and assign operators
  ITreatmentCoverageModel(const ITreatmentCoverageModel &) = delete;
  void operator=(const ITreatmentCoverageModel &) = delete;
  ITreatmentCoverageModel(ITreatmentCoverageModel &&) = delete;
  ITreatmentCoverageModel &operator=(ITreatmentCoverageModel &&) = delete;

 public:
  std::string type;
  int starting_time{0};
  std::vector<double> p_treatment_under_5;
  std::vector<double> p_treatment_over_5;

  ITreatmentCoverageModel() = default;

  virtual ~ITreatmentCoverageModel() = default;

  virtual double get_probability_to_be_treated(const int &location, const int &age);

  virtual void monthly_update() = 0;

  static ITreatmentCoverageModel *build_steady_tcm(const YAML::Node &node, Config *config);

  static void read_p_treatment(const YAML::Node &node, std::vector<double> &p_treatments, int number_of_locations);

  static ITreatmentCoverageModel *build_inflated_tcm(const YAML::Node &node, Config *config);

  static ITreatmentCoverageModel *build_linear_tcm(const YAML::Node &node, Config *config);

  static ITreatmentCoverageModel *build(const YAML::Node &node, Config *config);
};

#endif // TREATMENTCOVERAGEMODEL_H
