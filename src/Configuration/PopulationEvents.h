#ifndef POPULATIONPOPULATIONEVENTS_H
#define POPULATIONPOPULATIONEVENTS_H

#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <map>
#include "date/date.h"

class PopulationEvents {
public:
    // Inner class: ParasiteCaseInfo
    class ParasiteCaseInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const date::year_month_day& get_date() const { return date_; }
        void set_date(const date::year_month_day& value) { date_ = value; }

        [[nodiscard]] int get_genotype_id() const { return genotype_id_; }
        void set_genotype_id(int value) { genotype_id_ = value; }

        [[nodiscard]] int get_number_of_cases() const { return number_of_cases_; }
        void set_number_of_cases(int value) { number_of_cases_ = value; }

    private:
        date::year_month_day date_;
        int genotype_id_;
        int number_of_cases_;
    };

    // Inner class: AlleleDistribution
    class AlleleDistribution {
    public:
        // Getters and Setters
        [[nodiscard]] int get_position() const { return position_; }
        void set_position(int value) { position_ = value; }

        [[nodiscard]] const std::vector<double>& get_distribution() const { return distribution_; }
        void set_distribution(const std::vector<double>& value) { distribution_ = value; }

    private:
        int position_;
        std::vector<double> distribution_;
    };

    // Inner class: PeriodicParasiteIntroductionInfo
    class PeriodicParasiteIntroductionInfo {
    public:
        // Getters and Setters
        [[nodiscard]] int get_location() const { return location_; }
        void set_location(int value) { location_ = value; }

        [[nodiscard]] int get_duration() const { return duration_; }
        void set_duration(int value) { duration_ = value; }

        [[nodiscard]] int get_number_of_cases() const { return number_of_cases_; }
        void set_number_of_cases(int value) { number_of_cases_ = value; }

        [[nodiscard]] const date::year_month_day& get_start_date() const { return start_date_; }
        void set_start_date(const date::year_month_day& value) { start_date_ = value; }

        [[nodiscard]] const date::year_month_day& get_end_date() const { return end_date_; }
        void set_end_date(const date::year_month_day& value) { end_date_ = value; }

        [[nodiscard]] const std::vector<AlleleDistribution>& get_allele_distributions() const { return allele_distributions_; }
        void set_allele_distributions(const std::vector<AlleleDistribution>& value) { allele_distributions_ = value; }

    private:
        int location_;
        int duration_;
        int number_of_cases_;
        date::year_month_day start_date_;
        date::year_month_day end_date_;
        std::vector<AlleleDistribution> allele_distributions_;
    };

    // Inner class: TreatmentCoverageInfo
    class TreatmentCoverageInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_type() const { return type_; }
        void set_type(const std::string& value) { type_ = value; }

        [[nodiscard]] const date::year_month_day& get_date() const { return date_; }
        void set_date(const date::year_month_day& value) { date_ = value; }

        [[nodiscard]] const std::vector<double>& get_p_treatment_for_under_5_by_location() const { return p_treatment_for_under_5_by_location_; }
        void set_p_treatment_for_under_5_by_location(const std::vector<double>& value) { p_treatment_for_under_5_by_location_ = value; }

        [[nodiscard]] const std::vector<double>& get_p_treatment_for_over_5_by_location() const { return p_treatment_for_over_5_by_location_; }
        void set_p_treatment_for_over_5_by_location(const std::vector<double>& value) { p_treatment_for_over_5_by_location_ = value; }

        [[nodiscard]] double get_annual_inflation_rate() const { return annual_inflation_rate_; }
        void set_annual_inflation_rate(double value) { annual_inflation_rate_ = value; }

        [[nodiscard]] const date::year_month_day& get_from_date() const { return from_date_; }
        void set_from_date(const date::year_month_day& value) { from_date_ = value; }

        [[nodiscard]] const date::year_month_day& get_to_date() const { return to_date_; }
        void set_to_date(const date::year_month_day& value) { to_date_ = value; }

        [[nodiscard]] const std::vector<double>& get_p_treatment_for_under_5_by_location_to() const { return p_treatment_for_under_5_by_location_to_; }
        void set_p_treatment_for_under_5_by_location_to(const std::vector<double>& value) { p_treatment_for_under_5_by_location_to_ = value; }

        [[nodiscard]] const std::vector<double>& get_p_treatment_for_over_5_by_location_to() const { return p_treatment_for_over_5_by_location_to_; }
        void set_p_treatment_for_over_5_by_location_to(const std::vector<double>& value) { p_treatment_for_over_5_by_location_to_ = value; }

    private:
        std::string type_;
        date::year_month_day date_;
        std::vector<double> p_treatment_for_under_5_by_location_;
        std::vector<double> p_treatment_for_over_5_by_location_;
        double annual_inflation_rate_ = 0.0;
        date::year_month_day from_date_;
        date::year_month_day to_date_;
        std::vector<double> p_treatment_for_under_5_by_location_to_;
        std::vector<double> p_treatment_for_over_5_by_location_to_;
    };

    // Inner class: MDAInfo
    class MDAInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const date::year_month_day& get_date() const { return date_; }
        void set_date(const date::year_month_day& value) { date_ = value; }

        [[nodiscard]] const std::vector<double>& get_fraction_population_targeted() const { return fraction_population_targeted_; }
        void set_fraction_population_targeted(const std::vector<double>& value) { fraction_population_targeted_ = value; }

        [[nodiscard]] int get_days_to_complete_all_treatments() const { return days_to_complete_all_treatments_; }
        void set_days_to_complete_all_treatments(int value) { days_to_complete_all_treatments_ = value; }

    private:
        date::year_month_day date_;
        std::vector<double> fraction_population_targeted_;
        int days_to_complete_all_treatments_;
    };

    // Inner class: EventInfo
    class EventInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const date::year_month_day& get_date() const { return date_; }
        void set_date(const date::year_month_day& value) { date_ = value; }

        [[nodiscard]] bool get_mutation_status() const { return mutation_status_; }
        void set_mutation_status(bool value) { mutation_status_ = value; }

        [[nodiscard]] double get_parasite_fraction() const { return parasite_fraction_; }
        void set_parasite_fraction(double value) { parasite_fraction_ = value; }

        [[nodiscard]] int get_location_id() const { return location_id_; }
        void set_location_id(int value) { location_id_ = value; }

        [[nodiscard]] const std::vector<ParasiteCaseInfo>& get_parasite_cases() const { return parasite_cases_; }
        void set_parasite_cases(const std::vector<ParasiteCaseInfo>& value) { parasite_cases_ = value; }

        [[nodiscard]] const PeriodicParasiteIntroductionInfo& get_periodic_parasite_info() const { return periodic_parasite_info_; }
        void set_periodic_parasite_info(const PeriodicParasiteIntroductionInfo& value) { periodic_parasite_info_ = value; }

        [[nodiscard]] int get_treatment_strategy_id() const { return treatment_strategy_id_; }
        void set_treatment_strategy_id(int value) { treatment_strategy_id_ = value; }

        [[nodiscard]] double get_interrupted_feeding_rate() const { return interrupted_feeding_rate_; }
        void set_interrupted_feeding_rate(double value) { interrupted_feeding_rate_ = value; }

        [[nodiscard]] double get_mutation_probability_per_locus() const { return mutation_probability_per_locus_; }
        void set_mutation_probability_per_locus(double value) { mutation_probability_per_locus_ = value; }

    private:
        date::year_month_day date_;
        bool mutation_status_ = false;
        double parasite_fraction_ = 0.0;
        int location_id_ = -1;
        std::vector<ParasiteCaseInfo> parasite_cases_;
        PeriodicParasiteIntroductionInfo periodic_parasite_info_;
        int treatment_strategy_id_ = -1;
        double interrupted_feeding_rate_ = 0.0;
        double mutation_probability_per_locus_ = 0.0;
    };

    // Inner class: Event
    class Event {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_event_name() const { return event_name_; }
        void set_event_name(const std::string& value) { event_name_ = value; }

        [[nodiscard]] const std::vector<EventInfo>& get_event_info() const { return event_info_; }
        void set_event_info(const std::vector<EventInfo>& value) { event_info_ = value; }

    private:
        std::string event_name_;
        std::vector<EventInfo> event_info_;
    };

    // Getters and Setters for PopulationEvents
    [[nodiscard]] const std::map<std::string, Event>& get_event_list() const { return event_list_; }
    void set_event_list(const std::map<std::string, Event>& value) { event_list_ = value; }

private:
    std::map<std::string, Event> event_list_;  // Changed from vector to map
};

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
// PopulationEvents::ParasiteCaseInfo YAML conversion
template<>
struct convert<PopulationEvents::ParasiteCaseInfo> {
    static Node encode(const PopulationEvents::ParasiteCaseInfo& rhs) {
        Node node;
        node["day"] = rhs.get_date();
        node["genotype_id"] = rhs.get_genotype_id();
        node["number_of_cases"] = rhs.get_number_of_cases();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::ParasiteCaseInfo& rhs) {
        if (!node["day"] || !node["genotype_id"] || !node["number_of_cases"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::ParasiteCaseInfo");
        }
        rhs.set_date(node["day"].as<date::year_month_day>());
        rhs.set_genotype_id(node["genotype_id"].as<int>());
        rhs.set_number_of_cases(node["number_of_cases"].as<int>());
        return true;
    }
};

// PopulationEvents::AlleleDistribution YAML conversion
template<>
struct convert<PopulationEvents::AlleleDistribution> {
    static Node encode(const PopulationEvents::AlleleDistribution& rhs) {
        Node node;
        node["position"] = rhs.get_position();
        node["distribution"] = rhs.get_distribution();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::AlleleDistribution& rhs) {
        if (!node["position"] || !node["distribution"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::AlleleDistribution");
        }
        rhs.set_position(node["position"].as<int>());
        rhs.set_distribution(node["distribution"].as<std::vector<double>>());
        return true;
    }
};

// PopulationEvents::PeriodicParasiteIntroductionInfo YAML conversion
template<>
struct convert<PopulationEvents::PeriodicParasiteIntroductionInfo> {
    static Node encode(const PopulationEvents::PeriodicParasiteIntroductionInfo& rhs) {
        Node node;
        node["location"] = rhs.get_location();
        node["duration"] = rhs.get_duration();
        node["number_of_cases"] = rhs.get_number_of_cases();
        node["start_day"] = rhs.get_start_date();
        node["end_day"] = rhs.get_end_date();
        node["allele_distributions"] = rhs.get_allele_distributions();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::PeriodicParasiteIntroductionInfo& rhs) {
        if (!node["location"] || !node["duration"] || !node["number_of_cases"] ||
            !node["start_day"] || !node["end_day"] || !node["allele_distributions"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::PeriodicParasiteIntroductionInfo");
        }
        rhs.set_location(node["location"].as<int>());
        rhs.set_duration(node["duration"].as<int>());
        rhs.set_number_of_cases(node["number_of_cases"].as<int>());
        rhs.set_start_date(node["start_day"].as<date::year_month_day>());
        rhs.set_end_date(node["end_day"].as<date::year_month_day>());
        rhs.set_allele_distributions(node["allele_distributions"].as<std::vector<PopulationEvents::AlleleDistribution>>());
        return true;
    }
};

// PopulationEvents::TreatmentCoverageInfo YAML conversion
template<>
struct convert<PopulationEvents::TreatmentCoverageInfo> {
    static Node encode(const PopulationEvents::TreatmentCoverageInfo& rhs) {
        Node node;
        node["type"] = rhs.get_type();
        node["day"] = rhs.get_date();
        node["p_treatment_for_under_5_by_location"] = rhs.get_p_treatment_for_under_5_by_location();
        node["p_treatment_for_over_5_by_location"] = rhs.get_p_treatment_for_over_5_by_location();
        node["annual_inflation_rate"] = rhs.get_annual_inflation_rate();
        node["from_day"] = rhs.get_from_date();
        node["to_day"] = rhs.get_to_date();
        node["p_treatment_for_under_5_by_location_to"] = rhs.get_p_treatment_for_under_5_by_location_to();
        node["p_treatment_for_over_5_by_location_to"] = rhs.get_p_treatment_for_over_5_by_location_to();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::TreatmentCoverageInfo& rhs) {
        if (!node["type"] || !node["day"] || !node["p_treatment_for_under_5_by_location"] ||
            !node["p_treatment_for_over_5_by_location"] || !node["annual_inflation_rate"] ||
            !node["from_day"] || !node["to_day"] || !node["p_treatment_for_under_5_by_location_to"] ||
            !node["p_treatment_for_over_5_by_location_to"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::TreatmentCoverageInfo");
        }
        rhs.set_type(node["type"].as<std::string>());
        rhs.set_date(node["day"].as<date::year_month_day>());
        rhs.set_p_treatment_for_under_5_by_location(node["p_treatment_for_under_5_by_location"].as<std::vector<double>>());
        rhs.set_p_treatment_for_over_5_by_location(node["p_treatment_for_over_5_by_location"].as<std::vector<double>>());
        rhs.set_annual_inflation_rate(node["annual_inflation_rate"].as<double>());
        rhs.set_from_date(node["from_day"].as<date::year_month_day>());
        rhs.set_to_date(node["to_day"].as<date::year_month_day>());
        rhs.set_p_treatment_for_under_5_by_location_to(node["p_treatment_for_under_5_by_location_to"].as<std::vector<double>>());
        rhs.set_p_treatment_for_over_5_by_location_to(node["p_treatment_for_over_5_by_location_to"].as<std::vector<double>>());
        return true;
    }
};

// PopulationEvents::MDAInfo YAML conversion
template<>
struct convert<PopulationEvents::MDAInfo> {
    static Node encode(const PopulationEvents::MDAInfo& rhs) {
        Node node;
        node["day"] = rhs.get_date();
        node["fraction_population_targeted"] = rhs.get_fraction_population_targeted();
        node["days_to_complete_all_treatments"] = rhs.get_days_to_complete_all_treatments();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::MDAInfo& rhs) {
        if (!node["day"] || !node["fraction_population_targeted"] || !node["days_to_complete_all_treatments"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::MDAInfo");
        }
        rhs.set_date(node["day"].as<date::year_month_day>());
        rhs.set_fraction_population_targeted(node["fraction_population_targeted"].as<std::vector<double>>());
        rhs.set_days_to_complete_all_treatments(node["days_to_complete_all_treatments"].as<int>());
        return true;
    }
};

// PopulationEvents::EventInfo YAML conversion
template<>
struct convert<PopulationEvents::EventInfo> {
    static Node encode(const PopulationEvents::EventInfo& rhs) {
        Node node;
        node["day"] = rhs.get_date();
        node["mutation_status"] = rhs.get_mutation_status();
        node["parasite_fraction"] = rhs.get_parasite_fraction();
        node["location_id"] = rhs.get_location_id();
        node["parasite_cases"] = rhs.get_parasite_cases();
        node["periodic_parasite_info"] = rhs.get_periodic_parasite_info();
        node["treatment_strategy_id"] = rhs.get_treatment_strategy_id();
        node["interrupted_feeding_rate"] = rhs.get_interrupted_feeding_rate();
        node["mutation_probability_per_locus"] = rhs.get_mutation_probability_per_locus();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::EventInfo& rhs) {
        if (!node["day"] || !node["mutation_status"] || !node["parasite_fraction"] || !node["location_id"] ||
            !node["parasite_cases"] || !node["periodic_parasite_info"] || !node["treatment_strategy_id"] ||
            !node["interrupted_feeding_rate"] || !node["mutation_probability_per_locus"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::EventInfo");
        }
        rhs.set_date(node["day"].as<date::year_month_day>());
        rhs.set_mutation_status(node["mutation_status"].as<bool>());
        rhs.set_parasite_fraction(node["parasite_fraction"].as<double>());
        rhs.set_location_id(node["location_id"].as<int>());
        rhs.set_parasite_cases(node["parasite_cases"].as<std::vector<PopulationEvents::ParasiteCaseInfo>>());
        rhs.set_periodic_parasite_info(node["periodic_parasite_info"].as<PopulationEvents::PeriodicParasiteIntroductionInfo>());
        rhs.set_treatment_strategy_id(node["treatment_strategy_id"].as<int>());
        rhs.set_interrupted_feeding_rate(node["interrupted_feeding_rate"].as<double>());
        rhs.set_mutation_probability_per_locus(node["mutation_probability_per_locus"].as<double>());
        return true;
    }
};

// PopulationEvents::Event YAML conversion
template<>
struct convert<PopulationEvents::Event> {
    static Node encode(const PopulationEvents::Event& rhs) {
        Node node;
        node["event_name"] = rhs.get_event_name();
        node["event_info"] = rhs.get_event_info();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::Event& rhs) {
        if (!node["event_name"] || !node["event_info"]) {
            throw std::runtime_error("Missing fields in PopulationEvents::Event");
        }
        rhs.set_event_name(node["event_name"].as<std::string>());
        rhs.set_event_info(node["event_info"].as<std::vector<PopulationEvents::EventInfo>>());
        return true;
    }
};

// PopulationEvents YAML conversion
template<>
struct convert<PopulationEvents> {
    static Node encode(const PopulationEvents& rhs) {
        Node node;
        for (const auto& [key, value] : rhs.get_event_list()) {
            node["event_list"][key] = value;
        }
        return node;
    }

    static bool decode(const Node& node, PopulationEvents& rhs) {
        if (!node["event_list"]) {
            throw std::runtime_error("Missing 'event_list' in PopulationEvents");
        }
        std::map<std::string, PopulationEvents::Event> event_list;
        for (const auto& element : node["event_list"]) {
            event_list[element.first.as<std::string>()] = element.second.as<PopulationEvents::Event>();
        }
        rhs.set_event_list(event_list);
        return true;
    }
};

}  // namespace YAML

#endif //POPULATIONPOPULATIONEVENTS_H
