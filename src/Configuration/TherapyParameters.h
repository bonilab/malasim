#ifndef THERAPYPARAMETERS_H
#define THERAPYPARAMETERS_H

#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <vector>

class TherapyParameters {
public:
  // Inner class: TherapyInfo
  class TherapyInfo {
  public:
    // Getters and Setters
    [[nodiscard]] const std::vector<int>& get_drug_id() const { return drug_id_; }
    void set_drug_id(const std::vector<int>& value) { drug_id_ = value; }

    [[nodiscard]] const std::vector<int>& get_dosing_days() const { return dosing_days_; }
    void set_dosing_days(const std::vector<int>& value) { dosing_days_ = value; }

  private:
    std::vector<int> drug_id_;
    std::vector<int> dosing_days_;
  };

  // Getters and Setters for TherapyParameters
  [[nodiscard]] int get_tf_testing_day() const { return tf_testing_day_; }
  void set_tf_testing_day(int value) { tf_testing_day_ = value; }

  [[nodiscard]] double get_tf_rate() const { return tf_rate_; }
  void set_tf_rate(double value) { tf_rate_ = value; }

  [[nodiscard]] const std::vector<TherapyInfo>& get_therapy_db() const { return therapy_db_; }
  void set_therapy_db(const std::vector<TherapyInfo>& value) { therapy_db_ = value; }

private:
  int tf_testing_day_;
  double tf_rate_;
  std::vector<TherapyInfo> therapy_db_;
};

namespace YAML {

// TherapyParameters::TherapyInfo YAML conversion
template<>
struct convert<TherapyParameters::TherapyInfo> {
  static Node encode(const TherapyParameters::TherapyInfo& rhs) {
    Node node;
    node["drug_id"] = rhs.get_drug_id();
    node["dosing_days"] = rhs.get_dosing_days();
    return node;
  }

  static bool decode(const Node& node, TherapyParameters::TherapyInfo& rhs) {
    if (!node["drug_id"] || !node["dosing_days"]) {
      throw std::runtime_error("Missing fields in TherapyParameters::TherapyInfo");
    }
    rhs.set_drug_id(node["drug_id"].as<std::vector<int>>());
    rhs.set_dosing_days(node["dosing_days"].as<std::vector<int>>());
    return true;
  }
};

// TherapyParameters YAML conversion
template<>
struct convert<TherapyParameters> {
  static Node encode(const TherapyParameters& rhs) {
    Node node;
    node["tf_testing_day"] = rhs.get_tf_testing_day();
    node["tf_rate"] = rhs.get_tf_rate();
    node["therapy_db"] = rhs.get_therapy_db();
    return node;
  }

  static bool decode(const Node& node, TherapyParameters& rhs) {
    if (!node["tf_testing_day"] || !node["tf_rate"] || !node["therapy_db"]) {
      throw std::runtime_error("Missing fields in TherapyParameters");
    }
    rhs.set_tf_testing_day(node["tf_testing_day"].as<int>());
    rhs.set_tf_rate(node["tf_rate"].as<double>());
    rhs.set_therapy_db(node["therapy_db"].as<std::vector<TherapyParameters::TherapyInfo>>());
    return true;
  }
};

}  // namespace YAML


#endif //THERAPYPARAMETERS_H