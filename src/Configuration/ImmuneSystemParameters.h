#ifndef IMMUNESYSTEMPARAMETERS_H
#define IMMUNESYSTEMPARAMETERS_H
#include "IConfigClass.h"

class ImmuneSystemParameters : IConfigClass {
public:
    // Getters and Setters
    [[nodiscard]] double get_b1() const { return b1_; }
    void set_b1(double value) { b1_ = value; }

    [[nodiscard]] double get_b2() const { return b2_; }
    void set_b2(double value) { b2_ = value; }

    [[nodiscard]] int get_duration_for_naive() const { return duration_for_naive_; }
    void set_duration_for_naive(int value) {
        if (value < 0) throw std::invalid_argument("duration_for_naive must be non-negative");
        duration_for_naive_ = value;
    }

    [[nodiscard]] int get_duration_for_fully_immune() const { return duration_for_fully_immune_; }
    void set_duration_for_fully_immune(int value) {
        if (value < 0) throw std::invalid_argument("duration_for_fully_immune must be non-negative");
        duration_for_fully_immune_ = value;
    }

    [[nodiscard]] double get_mean_initial_condition() const { return mean_initial_condition_; }
    void set_mean_initial_condition(double value) { mean_initial_condition_ = value; }

    [[nodiscard]] double get_sd_initial_condition() const { return sd_initial_condition_; }
    void set_sd_initial_condition(double value) { sd_initial_condition_ = value; }

    [[nodiscard]] double get_immune_inflation_rate() const { return immune_inflation_rate_; }
    void set_immune_inflation_rate(double value) { immune_inflation_rate_ = value; }

    [[nodiscard]] double get_min_clinical_probability() const { return min_clinical_probability_; }
    void set_min_clinical_probability(double value) {
        if (value < 0 || value > 1) throw std::invalid_argument("min_clinical_probability must be between 0 and 1");
        min_clinical_probability_ = value;
    }

    [[nodiscard]] double get_max_clinical_probability() const { return max_clinical_probability_; }
    void set_max_clinical_probability(double value) {
        if (value < 0 || value > 1) throw std::invalid_argument("max_clinical_probability must be between 0 and 1");
        max_clinical_probability_ = value;
    }

    [[nodiscard]] double get_immune_effect_on_progression_to_clinical() const { return immune_effect_on_progression_to_clinical_; }
    void set_immune_effect_on_progression_to_clinical(double value) { immune_effect_on_progression_to_clinical_ = value; }

    [[nodiscard]] int get_age_mature_immunity() const { return age_mature_immunity_; }
    void set_age_mature_immunity(int value) {
        if (value < 0) throw std::invalid_argument("age_mature_immunity must be non-negative");
        age_mature_immunity_ = value;
    }

    [[nodiscard]] double get_factor_effect_age_mature_immunity() const { return factor_effect_age_mature_immunity_; }
    void set_factor_effect_age_mature_immunity(double value) { factor_effect_age_mature_immunity_ = value; }

    [[nodiscard]] double get_midpoint() const { return midpoint_; }
    void set_midpoint(double value) { midpoint_ = value; }

    void process_config() override {};

private:
    double b1_;
    double b2_;
    int duration_for_naive_;
    int duration_for_fully_immune_;
    double mean_initial_condition_;
    double sd_initial_condition_;
    double immune_inflation_rate_;
    double min_clinical_probability_;
    double max_clinical_probability_;
    double immune_effect_on_progression_to_clinical_;
    int age_mature_immunity_;
    double factor_effect_age_mature_immunity_;
    double midpoint_;
};

namespace YAML{
// ImmuneSystemParameters YAML conversion
template<>
struct convert<ImmuneSystemParameters> {
    static Node encode(const ImmuneSystemParameters& rhs) {
        Node node;
        node["b1"] = rhs.get_b1();
        node["b2"] = rhs.get_b2();
        node["duration_for_naive"] = rhs.get_duration_for_naive();
        node["duration_for_fully_immune"] = rhs.get_duration_for_fully_immune();
        node["mean_initial_condition"] = rhs.get_mean_initial_condition();
        node["sd_initial_condition"] = rhs.get_sd_initial_condition();
        node["immune_inflation_rate"] = rhs.get_immune_inflation_rate();
        node["min_clinical_probability"] = rhs.get_min_clinical_probability();
        node["max_clinical_probability"] = rhs.get_max_clinical_probability();
        node["immune_effect_on_progression_to_clinical"] = rhs.get_immune_effect_on_progression_to_clinical();
        node["age_mature_immunity"] = rhs.get_age_mature_immunity();
        node["factor_effect_age_mature_immunity"] = rhs.get_factor_effect_age_mature_immunity();
        node["midpoint"] = rhs.get_midpoint();
        return node;
    }

    static bool decode(const Node& node, ImmuneSystemParameters& rhs) {
        if (!node["b1"] || !node["b2"] || !node["duration_for_naive"] || !node["duration_for_fully_immune"]
            || !node["mean_initial_condition"] || !node["sd_initial_condition"]
            || !node["immune_inflation_rate"] || !node["min_clinical_probability"]
            || !node["max_clinical_probability"] || !node["immune_effect_on_progression_to_clinical"]
            || !node["age_mature_immunity"] || !node["factor_effect_age_mature_immunity"]
            || !node["midpoint"]) {
            throw std::runtime_error("Missing fields in ImmuneSystemParameters");
        }

        rhs.set_b1(node["b1"].as<double>());
        rhs.set_b2(node["b2"].as<double>());
        rhs.set_duration_for_naive(node["duration_for_naive"].as<int>());
        rhs.set_duration_for_fully_immune(node["duration_for_fully_immune"].as<int>());
        rhs.set_mean_initial_condition(node["mean_initial_condition"].as<double>());
        rhs.set_sd_initial_condition(node["sd_initial_condition"].as<double>());
        rhs.set_immune_inflation_rate(node["immune_inflation_rate"].as<double>());
        rhs.set_min_clinical_probability(node["min_clinical_probability"].as<double>());
        rhs.set_max_clinical_probability(node["max_clinical_probability"].as<double>());
        rhs.set_immune_effect_on_progression_to_clinical(node["immune_effect_on_progression_to_clinical"].as<double>());
        rhs.set_age_mature_immunity(node["age_mature_immunity"].as<int>());
        rhs.set_factor_effect_age_mature_immunity(node["factor_effect_age_mature_immunity"].as<double>());
        rhs.set_midpoint(node["midpoint"].as<double>());
        return true;
    }
};
}
#endif //IMMUNESYSTEMPARAMETERS_H
