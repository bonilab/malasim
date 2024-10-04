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
    date::year_month_day ymd{};
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
    node["days_between_stdout_output"] = rhs.get_days_between_stdout_output();
    node["initial_seed_number"] = rhs.get_initial_seed_number();
    node["record_genome_db"] = rhs.get_record_genome_db();
    node["starting_date"] = rhs.get_starting_date();
    node["start_of_comparison_period"] = rhs.get_start_of_comparison_period();
    node["ending_date"] = rhs.get_ending_date();
    node["start_collect_data_day"] = rhs.get_start_collect_data_day();
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

    rhs.set_days_between_stdout_output(
        node["days_between_stdout_output"].as<int>());
    rhs.set_initial_seed_number(node["initial_seed_number"].as<int>());
    rhs.set_record_genome_db(node["record_genome_db"].as<bool>());
    rhs.set_starting_date(node["starting_date"].as<date::year_month_day>());
    rhs.set_start_of_comparison_period(
        node["start_of_comparison_period"].as<date::year_month_day>());
    rhs.set_ending_date(node["ending_date"].as<date::year_month_day>());
    rhs.set_start_collect_data_day(node["start_collect_data_day"].as<int>());
    return true;
  }
};

template <>
struct convert<TransmissionSettings> {
  static Node encode(const TransmissionSettings &rhs) {
    Node node;
    node["transmission_parameter"] = rhs.get_transmission_parameter();
    node["p_infection_from_an_infectious_bite"] =
        rhs.get_p_infection_from_an_infectious_bite();
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

    rhs.set_transmission_parameter(node["transmission_parameter"].as<double>());
    rhs.set_p_infection_from_an_infectious_bite(
        node["p_infection_from_an_infectious_bite"].as<double>());
    return true;
  }
};

template <>
struct convert<PopulationDemographic> {
  static Node encode(const PopulationDemographic &rhs) {
    Node node;
    node["number_of_age_classes"] = rhs.get_number_of_age_classes();
    node["age_structure"] = rhs.get_age_structure();
    node["initial_age_structure"] = rhs.get_initial_age_structure();
    node["birth_rate"] = rhs.get_birth_rate();
    node["death_rate_by_age_class"] = rhs.get_death_rate_by_age_class();
    node["mortality_when_treatment_fail_by_age_class"] =
        rhs.get_mortality_when_treatment_fail_by_age_class();
    node["artificial_rescaling_of_population_size"] =
        rhs.get_artificial_rescaling_of_population_size();
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

    int number_of_age_classes = node["number_of_age_classes"].as<int>();
    rhs.set_number_of_age_classes(number_of_age_classes);

    // Validate and assign age structure vectors
    auto age_structure = node["age_structure"].as<std::vector<int>>();
    rhs.set_age_structure(age_structure);

    auto initial_age_structure =
        node["initial_age_structure"].as<std::vector<int>>();
    rhs.set_initial_age_structure(initial_age_structure);

    rhs.set_birth_rate(node["birth_rate"].as<double>());

    auto death_rate_by_age_class =
        node["death_rate_by_age_class"].as<std::vector<double>>();
    rhs.set_death_rate_by_age_class(death_rate_by_age_class);

    auto mortality_when_treatment_fail_by_age_class =
        node["mortality_when_treatment_fail_by_age_class"]
            .as<std::vector<double>>();
    rhs.set_mortality_when_treatment_fail_by_age_class(
        mortality_when_treatment_fail_by_age_class);

    rhs.set_artificial_rescaling_of_population_size(
        node["artificial_rescaling_of_population_size"].as<double>());

    return true;
  }
};

}  // namespace YAML

#endif  // YAML_CONVERTERS_H

