#ifndef STRATEGYPARAMETERS_H
#define STRATEGYPARAMETERS_H

#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <vector>
#include <string>

class StrategyParameters {
public:
    // Inner class: StrategyInfo
    class StrategyInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_name() const { return name_; }
        void set_name(const std::string& value) { name_ = value; }

        [[nodiscard]] const std::string& get_type() const { return type_; }
        void set_type(const std::string& value) { type_ = value; }

        [[nodiscard]] const std::vector<int>& get_therapy_ids() const { return therapy_ids_; }
        void set_therapy_ids(const std::vector<int>& value) { therapy_ids_ = value; }

        [[nodiscard]] const std::vector<double>& get_distribution() const { return distribution_; }
        void set_distribution(const std::vector<double>& value) { distribution_ = value; }

        [[nodiscard]] int get_cycling_time() const { return cycling_time_; }
        void set_cycling_time(int value) { cycling_time_ = value; }

        [[nodiscard]] double get_trigger_value() const { return trigger_value_; }
        void set_trigger_value(double value) { trigger_value_ = value; }

        [[nodiscard]] int get_delay_until_actual_trigger() const { return delay_until_actual_trigger_; }
        void set_delay_until_actual_trigger(int value) { delay_until_actual_trigger_ = value; }

        [[nodiscard]] int get_turn_off_days() const { return turn_off_days_; }
        void set_turn_off_days(int value) { turn_off_days_ = value; }

        [[nodiscard]] int get_update_duration_after_rebalancing() const { return update_duration_after_rebalancing_; }
        void set_update_duration_after_rebalancing(int value) { update_duration_after_rebalancing_ = value; }

        [[nodiscard]] const std::vector<int>& get_strategy_ids() const { return strategy_ids_; }
        void set_strategy_ids(const std::vector<int>& value) { strategy_ids_ = value; }

        [[nodiscard]] const std::vector<double>& get_start_distribution() const { return start_distribution_; }
        void set_start_distribution(const std::vector<double>& value) { start_distribution_ = value; }

        [[nodiscard]] const std::vector<double>& get_peak_distribution() const { return peak_distribution_; }
        void set_peak_distribution(const std::vector<double>& value) { peak_distribution_ = value; }

        [[nodiscard]] int get_peak_after() const { return peak_after_; }
        void set_peak_after(int value) { peak_after_ = value; }

    private:
        std::string name_;
        std::string type_;
        std::vector<int> therapy_ids_;
        std::vector<double> distribution_;
        int cycling_time_ = 0;
        double trigger_value_ = 0.0;
        int delay_until_actual_trigger_ = 0;
        int turn_off_days_ = 0;
        int update_duration_after_rebalancing_ = 0;
        std::vector<int> strategy_ids_;
        std::vector<double> start_distribution_;
        std::vector<double> peak_distribution_;
        int peak_after_ = 0;
    };

    // Inner class: MassDrugAdministration
    class MassDrugAdministration {
    public:
        // Getters and Setters
        [[nodiscard]] bool get_enable() const { return enable_; }
        void set_enable(bool value) { enable_ = value; }

        [[nodiscard]] int get_mda_therapy_id() const { return mda_therapy_id_; }
        void set_mda_therapy_id(int value) { mda_therapy_id_ = value; }

        [[nodiscard]] const std::vector<int>& get_age_bracket_prob_individual_present_at_mda() const { return age_bracket_prob_individual_present_at_mda_; }
        void set_age_bracket_prob_individual_present_at_mda(const std::vector<int>& value) { age_bracket_prob_individual_present_at_mda_ = value; }

        [[nodiscard]] const std::vector<double>& get_mean_prob_individual_present_at_mda() const { return mean_prob_individual_present_at_mda_; }
        void set_mean_prob_individual_present_at_mda(const std::vector<double>& value) { mean_prob_individual_present_at_mda_ = value; }

        [[nodiscard]] const std::vector<double>& get_sd_prob_individual_present_at_mda() const { return sd_prob_individual_present_at_mda_; }
        void set_sd_prob_individual_present_at_mda(const std::vector<double>& value) { sd_prob_individual_present_at_mda_ = value; }

    private:
        bool enable_;
        int mda_therapy_id_;
        std::vector<int> age_bracket_prob_individual_present_at_mda_;
        std::vector<double> mean_prob_individual_present_at_mda_;
        std::vector<double> sd_prob_individual_present_at_mda_;
    };

    // Getters and Setters for StrategyParameters
    [[nodiscard]] const std::vector<StrategyInfo>& get_strategy_db() const { return strategy_db_; }
    void set_strategy_db(const std::vector<StrategyInfo>& value) { strategy_db_ = value; }

    [[nodiscard]] int get_initial_strategy_id() const { return initial_strategy_id_; }
    void set_initial_strategy_id(int value) { initial_strategy_id_ = value; }

    [[nodiscard]] int get_recurrent_therapy_id() const { return recurrent_therapy_id_; }
    void set_recurrent_therapy_id(int value) { recurrent_therapy_id_ = value; }

    [[nodiscard]] const MassDrugAdministration& get_mass_drug_administration() const { return mass_drug_administration_; }
    void set_mass_drug_administration(const MassDrugAdministration& value) { mass_drug_administration_ = value; }

private:
    std::vector<StrategyInfo> strategy_db_;
    int initial_strategy_id_;
    int recurrent_therapy_id_;
    MassDrugAdministration mass_drug_administration_;
};

namespace YAML {

// StrategyParameters::StrategyInfo YAML conversion
template<>
struct convert<StrategyParameters::StrategyInfo> {
    static Node encode(const StrategyParameters::StrategyInfo& rhs) {
        Node node;
        node["name"] = rhs.get_name();
        node["type"] = rhs.get_type();
        node["therapy_ids"] = rhs.get_therapy_ids();
        node["distribution"] = rhs.get_distribution();
        node["cycling_time"] = rhs.get_cycling_time();
        node["trigger_value"] = rhs.get_trigger_value();
        node["delay_until_actual_trigger"] = rhs.get_delay_until_actual_trigger();
        node["turn_off_days"] = rhs.get_turn_off_days();
        node["update_duration_after_rebalancing"] = rhs.get_update_duration_after_rebalancing();
        node["strategy_ids"] = rhs.get_strategy_ids();
        node["start_distribution"] = rhs.get_start_distribution();
        node["peak_distribution"] = rhs.get_peak_distribution();
        node["peak_after"] = rhs.get_peak_after();
        return node;
    }

    static bool decode(const Node& node, StrategyParameters::StrategyInfo& rhs) {
        if (!node["name"] || !node["type"] || !node["therapy_ids"] ) {
            throw std::runtime_error("Missing fields in StrategyParameters::StrategyInfo");
        }
        rhs.set_name(node["name"].as<std::string>());
        rhs.set_type(node["type"].as<std::string>());
        rhs.set_therapy_ids(node["therapy_ids"].as<std::vector<int>>());
        if(node["distribution"])
            rhs.set_distribution(node["distribution"].as<std::vector<double>>());
        if(node["cycling_time"])
            rhs.set_cycling_time(node["cycling_time"].as<int>());
        if(node["trigger_value"])
            rhs.set_trigger_value(node["trigger_value"].as<double>());
        if(node["delay_until_actual_trigger"])
            rhs.set_delay_until_actual_trigger(node["delay_until_actual_trigger"].as<int>());
        if(node["turn_off_days"])
            rhs.set_turn_off_days(node["turn_off_days"].as<int>());
        if(node["update_duration_after_rebalancing"])
            rhs.set_update_duration_after_rebalancing(node["update_duration_after_rebalancing"].as<int>());
        if(node["strategy_ids"])
            rhs.set_strategy_ids(node["strategy_ids"].as<std::vector<int>>());
        if(node["start_distribution"])
            rhs.set_start_distribution(node["start_distribution"].as<std::vector<double>>());
        if(node["peak_distribution"])
            rhs.set_peak_distribution(node["peak_distribution"].as<std::vector<double>>());
        if(node["peak_after"])
            rhs.set_peak_after(node["peak_after"].as<int>());
        return true;
    }
};

// StrategyParameters::MassDrugAdministration YAML conversion
template<>
struct convert<StrategyParameters::MassDrugAdministration> {
    static Node encode(const StrategyParameters::MassDrugAdministration& rhs) {
        Node node;
        node["enable"] = rhs.get_enable();
        node["mda_therapy_id"] = rhs.get_mda_therapy_id();
        node["age_bracket_prob_individual_present_at_mda"] = rhs.get_age_bracket_prob_individual_present_at_mda();
        node["mean_prob_individual_present_at_mda"] = rhs.get_mean_prob_individual_present_at_mda();
        node["sd_prob_individual_present_at_mda"] = rhs.get_sd_prob_individual_present_at_mda();
        return node;
    }

    static bool decode(const Node& node, StrategyParameters::MassDrugAdministration& rhs) {
        if (!node["enable"] || !node["mda_therapy_id"] || !node["age_bracket_prob_individual_present_at_mda"] ||
            !node["mean_prob_individual_present_at_mda"] || !node["sd_prob_individual_present_at_mda"]) {
            throw std::runtime_error("Missing fields in StrategyParameters::MassDrugAdministration");
        }
        rhs.set_enable(node["enable"].as<bool>());
        rhs.set_mda_therapy_id(node["mda_therapy_id"].as<int>());
        rhs.set_age_bracket_prob_individual_present_at_mda(node["age_bracket_prob_individual_present_at_mda"].as<std::vector<int>>());
        rhs.set_mean_prob_individual_present_at_mda(node["mean_prob_individual_present_at_mda"].as<std::vector<double>>());
        rhs.set_sd_prob_individual_present_at_mda(node["sd_prob_individual_present_at_mda"].as<std::vector<double>>());
        return true;
    }
};

// StrategyParameters YAML conversion
template<>
struct convert<StrategyParameters> {
    static Node encode(const StrategyParameters& rhs) {
        Node node;
        node["strategy_db"] = rhs.get_strategy_db();
        node["initial_strategy_id"] = rhs.get_initial_strategy_id();
        node["recurrent_therapy_id"] = rhs.get_recurrent_therapy_id();
        node["mass_drug_administration"] = rhs.get_mass_drug_administration();
        return node;
    }

    static bool decode(const Node& node, StrategyParameters& rhs) {
        if (!node["strategy_db"] || !node["initial_strategy_id"] || !node["recurrent_therapy_id"] || !node["mass_drug_administration"]) {
            throw std::runtime_error("Missing fields in StrategyParameters");
        }
        rhs.set_strategy_db(node["strategy_db"].as<std::vector<StrategyParameters::StrategyInfo>>());
        rhs.set_initial_strategy_id(node["initial_strategy_id"].as<int>());
        rhs.set_recurrent_therapy_id(node["recurrent_therapy_id"].as<int>());
        rhs.set_mass_drug_administration(node["mass_drug_administration"].as<StrategyParameters::MassDrugAdministration>());
        return true;
    }
};

}  // namespace YAML

#endif //STRATEGYPARAMETERS_H
