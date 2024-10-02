#ifndef YAML_CONVERTERS_H
#define YAML_CONVERTERS_H

#include <yaml-cpp/yaml.h>

#include "ModelSettings.h"
#include "PopulationDemographic.h"
#include "TransmissionSettings.h"

namespace YAML {
template <>
struct convert<date::year_month_day> {
  static Node encode(const date::year_month_day &rhs) {
    std::stringstream ss;
    ss << rhs;
    return Node(ss.str());
  }

  static bool decode(const Node &node, date::year_month_day &rhs) {
    if (!node.IsScalar()) {
      throw std::runtime_error("Invalid date format: not a scalar.");
    }

    std::stringstream ss(node.as<std::string>());
    date::year_month_day ymd;
    ss >> date::parse("%F", ymd);  // %F matches YYYY-MM-DD format

    if (ss.fail()) {
      throw std::runtime_error("Invalid date format: failed to parse.");
    }

    rhs = ymd;
    return true;
  }
};

template <>
struct convert<ModelSettings> {
  static Node encode(const ModelSettings &rhs) {
    Node node;
    node["days_between_stdout_output"] = rhs.days_between_stdout_output;
    node["initial_seed_number"] = rhs.initial_seed_number;
    node["record_genome_db"] = rhs.record_genome_db;
    node["starting_date"] = rhs.starting_date;
    node["start_of_comparison_period"] = rhs.start_of_comparison_period;
    node["ending_date"] = rhs.ending_date;
    node["start_collect_data_day"] = rhs.start_collect_data_day;
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
    if (!node["starting_date"]) {
      throw std::runtime_error("Missing 'starting_date' field.");
    }
    if (!node["start_of_comparison_period"]) {
      throw std::runtime_error("Missing 'start_of_comparison_period' field.");
    }
    if (!node["ending_date"]) {
      throw std::runtime_error("Missing 'ending_date' field.");
    }
    if (!node["start_collect_data_day"]) {
      throw std::runtime_error("Missing 'start_collect_data_day' field.");
    }

    // TODO: Add more error checking for each field

    rhs.days_between_stdout_output =
        node["days_between_stdout_output"].as<int>();
    rhs.initial_seed_number = node["initial_seed_number"].as<int>();
    rhs.record_genome_db = node["record_genome_db"].as<bool>();
    rhs.starting_date = node["starting_date"].as<date::year_month_day>();
    rhs.start_of_comparison_period =
        node["start_of_comparison_period"].as<date::year_month_day>();
    rhs.ending_date = node["ending_date"].as<date::year_month_day>();
    rhs.start_collect_data_day = node["start_collect_data_day"].as<int>();
    return true;
  }
};

template <>
struct convert<TransmissionSettings> {
  static Node encode(const TransmissionSettings &rhs) {
    Node node;
    node["transmission_parameter"] = rhs.transmission_parameter;
    node["p_infection_from_an_infectious_bite"] =
        rhs.p_infection_from_an_infectious_bite;
    return node;
  }

  static bool decode(const Node &node, TransmissionSettings &rhs) {
    if (!node["transmission_parameter"]) {
      throw std::runtime_error("Missing 'transmission_parameter' field.");
    }
    if (!node["p_infection_from_an_infectious_bite"]) {
      throw std::runtime_error(
          "Missing 'p_infection_from_an_infectious_bite' field.");
    }
    // TODO: Add more error checking for each field

    rhs.transmission_parameter = node["transmission_parameter"].as<double>();
    rhs.p_infection_from_an_infectious_bite =
        node["p_infection_from_an_infectious_bite"].as<double>();
    return true;
  }
};

template <>
struct convert<PopulationDemographic> {
  static Node encode(const PopulationDemographic &rhs) {
    Node node;
    node["number_of_age_classes"] = rhs.number_of_age_classes;
    node["age_structure"] = rhs.age_structure;
    node["initial_age_structure"] = rhs.initial_age_structure;
    node["birth_rate"] = rhs.birth_rate;
    node["death_rate_by_age_class"] = rhs.death_rate_by_age_class;
    node["mortality_when_treatment_fail_by_age_class"] =
        rhs.mortality_when_treatment_fail_by_age_class;
    node["artificial_rescaling_of_population_size"] =
        rhs.artificial_rescaling_of_population_size;
    return node;
  }

  static bool decode(const Node &node, PopulationDemographic &rhs) {
    if (!node["number_of_age_classes"]) {
      throw std::runtime_error("Missing 'number_of_age_classes' field.");
    }
    if (!node["age_structure"]) {
      throw std::runtime_error("Missing 'age_structure' field.");
    }
    if (!node["initial_age_structure"]) {
      throw std::runtime_error("Missing 'initial_age_structure' field.");
    }
    if (!node["birth_rate"]) {
      throw std::runtime_error("Missing 'birth_rate' field.");
    }
    if (!node["death_rate_by_age_class"]) {
      throw std::runtime_error("Missing 'death_rate_by_age_class' field.");
    }
    if (!node["mortality_when_treatment_fail_by_age_class"]) {
      throw std::runtime_error(
          "Missing 'mortality_when_treatment_fail_by_age_class' field.");
    }
    if (!node["artificial_rescaling_of_population_size"]) {
      throw std::runtime_error(
          "Missing 'artificial_rescaling_of_population_size' field.");
    }

    rhs.number_of_age_classes = node["number_of_age_classes"].as<int>();
    rhs.age_structure = node["age_structure"].as<std::vector<int>>();
    rhs.initial_age_structure =
        node["initial_age_structure"].as<std::vector<int>>();
    rhs.birth_rate = node["birth_rate"].as<double>();
    rhs.death_rate_by_age_class =
        node["death_rate_by_age_class"].as<std::vector<double>>();
    rhs.mortality_when_treatment_fail_by_age_class =
        node["mortality_when_treatment_fail_by_age_class"]
            .as<std::vector<double>>();
    rhs.artificial_rescaling_of_population_size =
        node["artificial_rescaling_of_population_size"].as<double>();
    return true;
  }
};

}  // namespace YAML

#endif  // YAML_CONVERTERS_H
