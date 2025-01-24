#ifndef THERAPYPARAMETERS_H
#define THERAPYPARAMETERS_H

#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <spdlog/spdlog.h>
#include "IConfigClass.h"

class TherapyParameters: IConfigClass {
public:
  std::vector<Therapy*> therapy_db;
  // Inner class: TherapyInfo
  class TherapyInfo {
  public:
    // Getters and Setters
    [[nodiscard]] const std::vector<int>& get_drug_ids() const { return drug_ids_; }
    void set_drug_ids(const std::vector<int>& value) { drug_ids_ = value; }

    [[nodiscard]] const std::vector<int>& get_dosing_days() const { return dosing_days_; }
    void set_dosing_days(const std::vector<int>& value) { dosing_days_ = value; }

  private:
    std::vector<int> drug_ids_;
    std::vector<int> dosing_days_;
  };

  // Getters and Setters for TherapyParameters
  [[nodiscard]] int get_tf_testing_day() const { return tf_testing_day_; }
  void set_tf_testing_day(int value) { tf_testing_day_ = value; }

  [[nodiscard]] double get_tf_rate() const { return tf_rate_; }
  void set_tf_rate(double value) { tf_rate_ = value; }

  [[nodiscard]] const std::map<int, TherapyInfo>& get_therapy_db_raw() const { return therapy_db_raw_; }
  void set_therapy_db_raw(const std::map<int, TherapyInfo>& value) { therapy_db_raw_ = value; }

  void process_config() override {
      spdlog::info("Processing TherapyParameters");
  };

private:
  int tf_testing_day_;
  double tf_rate_;
  std::map<int, TherapyInfo> therapy_db_raw_;  // Changed from vector to map
};

namespace YAML {

// TherapyParameters::TherapyInfo YAML conversion
template<>
struct convert<TherapyParameters::TherapyInfo> {
  static Node encode(const TherapyParameters::TherapyInfo& rhs) {
    Node node;
    node["drug_ids"] = rhs.get_drug_ids();
    node["dosing_days"] = rhs.get_dosing_days();
    return node;
  }

  static bool decode(const Node& node, TherapyParameters::TherapyInfo& rhs) {
    if (!node["drug_ids"] || !node["dosing_days"]) {
      throw std::runtime_error("Missing fields in TherapyParameters::TherapyInfo");
    }
    rhs.set_drug_ids(node["drug_ids"].as<std::vector<int>>());
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

    // Encode therapy_db as a map
    Node therapy_db_node;
    for (const auto& [key, value] : rhs.get_therapy_db()) {
      therapy_db_node[key] = value;
    }
    node["therapy_db"] = therapy_db_node;
    return node;
  }

  static bool decode(const Node& node, TherapyParameters& rhs) {
    if (!node["tf_testing_day"] || !node["tf_rate"] || !node["therapy_db"]) {
      throw std::runtime_error("Missing fields in TherapyParameters");
    }
    rhs.set_tf_testing_day(node["tf_testing_day"].as<int>());
    rhs.set_tf_rate(node["tf_rate"].as<double>());

    // Decode therapy_db as a map
    std::map<int, TherapyParameters::TherapyInfo> therapy_db;
    for (const auto& element : node["therapy_db"]) {
      int key = element.first.as<int>();
      therapy_db[key] = element.second.as<TherapyParameters::TherapyInfo>();
    }
    rhs.set_therapy_db(therapy_db);
    return true;
  }
};

}  // namespace YAML

#endif //THERAPYPARAMETERS_H
