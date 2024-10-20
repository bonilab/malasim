#ifndef MODEL_SETTINGS_H
#define MODEL_SETTINGS_H

#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include "Utils/YamlFile.h"
#include "Utils/Random.h"

class ModelSettings {
public:
  // Getters
  [[nodiscard]] int get_days_between_stdout_output() const {
    return days_between_stdout_output_;
  }
  // Setters with validation
  void set_days_between_stdout_output(int value) {
    if (value <= 0)
      throw std::invalid_argument(
          "days_between_stdout_output must be greater than 0");
    days_between_stdout_output_ = value;
  }
  [[nodiscard]] int get_initial_seed_number() const {
    return initial_seed_number_;
  }
  void set_initial_seed_number(int value) {
    if (value < 0) {
      spdlog::warn("Initial seed number is non-negative. Using random seed instead.");
      //TODO: Implement random seed generation
    }
    else {
      initial_seed_number_ = value;
    }
  }
  [[nodiscard]] bool get_record_genome_db() const { return record_genome_db_; }
  void set_record_genome_db(bool value) { record_genome_db_ = value; }

private:
  int days_between_stdout_output_;
  int initial_seed_number_;
  bool record_genome_db_;
};

namespace YAML {
template <>
struct convert<ModelSettings> {
  static Node encode(const ModelSettings &rhs) {
    Node node;
    node["days_between_stdout_output"] = rhs.get_days_between_stdout_output();
    node["initial_seed_number"] = rhs.get_initial_seed_number();
    node["record_genome_db"] = rhs.get_record_genome_db();
    return node;
  }

  static bool decode(const Node &node, ModelSettings &rhs) {
    if (!node["days_between_stdout_output"]) {
      throw std::runtime_error("Missing 'days_between_stdout_output' field.");
    }
    if (!node["initial_seed_number"]) {
      throw std::runtime_error("Missing 'initial_seed_number' field.");
    }
    if (!node["record_genome_db"]) {
      throw std::runtime_error("Missing 'record_genome_db' field.");
    }

    // TODO: Add more error checking for each field

    rhs.set_days_between_stdout_output(
        node["days_between_stdout_output"].as<int>());
    rhs.set_initial_seed_number(node["initial_seed_number"].as<int>());
    rhs.set_record_genome_db(node["record_genome_db"].as<bool>());
    return true;
  }
};
}  // namespace YAML

#endif  // MODEL_SETTINGS_H

