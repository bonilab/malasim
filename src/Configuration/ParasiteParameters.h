#ifndef PARASITEPARAMETERS_H
#define PARASITEPARAMETERS_H

class ParasiteDensityLevels {
public:
    // Getters and Setters
    [[nodiscard]] double get_log_parasite_density_cured() const { return log_parasite_density_cured_; }
    void set_log_parasite_density_cured(double value) { log_parasite_density_cured_ = value; }

    [[nodiscard]] double get_log_parasite_density_from_liver() const { return log_parasite_density_from_liver_; }
    void set_log_parasite_density_from_liver(double value) { log_parasite_density_from_liver_ = value; }

    [[nodiscard]] double get_log_parasite_density_asymptomatic() const { return log_parasite_density_asymptomatic_; }
    void set_log_parasite_density_asymptomatic(double value) { log_parasite_density_asymptomatic_ = value; }

    [[nodiscard]] double get_log_parasite_density_clinical() const { return log_parasite_density_clinical_; }
    void set_log_parasite_density_clinical(double value) { log_parasite_density_clinical_ = value; }

    [[nodiscard]] double get_log_parasite_density_clinical_from() const { return log_parasite_density_clinical_from_; }
    void set_log_parasite_density_clinical_from(double value) { log_parasite_density_clinical_from_ = value; }

    [[nodiscard]] double get_log_parasite_density_clinical_to() const { return log_parasite_density_clinical_to_; }
    void set_log_parasite_density_clinical_to(double value) { log_parasite_density_clinical_to_ = value; }

    [[nodiscard]] double get_log_parasite_density_detectable() const { return log_parasite_density_detectable_; }
    void set_log_parasite_density_detectable(double value) { log_parasite_density_detectable_ = value; }

    [[nodiscard]] double get_log_parasite_density_detectable_pfpr() const { return log_parasite_density_detectable_pfpr_; }
    void set_log_parasite_density_detectable_pfpr(double value) { log_parasite_density_detectable_pfpr_ = value; }

    [[nodiscard]] double get_log_parasite_density_pyrogenic() const { return log_parasite_density_pyrogenic_; }
    void set_log_parasite_density_pyrogenic(double value) { log_parasite_density_pyrogenic_ = value; }

private:
    double log_parasite_density_cured_;
    double log_parasite_density_from_liver_;
    double log_parasite_density_asymptomatic_;
    double log_parasite_density_clinical_;
    double log_parasite_density_clinical_from_;
    double log_parasite_density_clinical_to_;
    double log_parasite_density_detectable_;
    double log_parasite_density_detectable_pfpr_;
    double log_parasite_density_pyrogenic_;
};

class RecombinationParameters {
public:
    // Getters and Setters
    [[nodiscard]] double get_within_chromosome_recombination_rate() const { return within_chromosome_recombination_rate_; }
    void set_within_chromosome_recombination_rate(double value) { within_chromosome_recombination_rate_ = value; }

    [[nodiscard]] bool get_using_free_recombination() const { return using_free_recombination_; }
    void set_using_free_recombination(bool value) { using_free_recombination_ = value; }

private:
    double within_chromosome_recombination_rate_;
    bool using_free_recombination_;
};

class ParasiteParameters {
public:
    // Getters and Setters
    [[nodiscard]] const ParasiteDensityLevels& get_parasite_density_levels() const { return parasite_density_levels_; }
    void set_parasite_density_levels(const ParasiteDensityLevels& value) { parasite_density_levels_ = value; }

    [[nodiscard]] const RecombinationParameters& get_recombination_parameters() const { return recombination_parameters_; }
    void set_recombination_parameters(const RecombinationParameters& value) { recombination_parameters_ = value; }

private:
    ParasiteDensityLevels parasite_density_levels_;
    RecombinationParameters recombination_parameters_;
};

namespace YAML {
// ParasiteDensityLevels YAML conversion
template<>
struct convert<ParasiteDensityLevels> {
    static Node encode(const ParasiteDensityLevels& rhs) {
        Node node;
        node["log_parasite_density_cured"] = rhs.get_log_parasite_density_cured();
        node["log_parasite_density_from_liver"] = rhs.get_log_parasite_density_from_liver();
        node["log_parasite_density_asymptomatic"] = rhs.get_log_parasite_density_asymptomatic();
        node["log_parasite_density_clinical"] = rhs.get_log_parasite_density_clinical();
        node["log_parasite_density_clinical_from"] = rhs.get_log_parasite_density_clinical_from();
        node["log_parasite_density_clinical_to"] = rhs.get_log_parasite_density_clinical_to();
        node["log_parasite_density_detectable"] = rhs.get_log_parasite_density_detectable();
        node["log_parasite_density_detectable_pfpr"] = rhs.get_log_parasite_density_detectable_pfpr();
        node["log_parasite_density_pyrogenic"] = rhs.get_log_parasite_density_pyrogenic();
        return node;
    }

    static bool decode(const Node& node, ParasiteDensityLevels& rhs) {
        if(!node["log_parasite_density_cured"] || !node["log_parasite_density_from_liver"] ||
           !node["log_parasite_density_asymptomatic"] || !node["log_parasite_density_clinical"] ||
           !node["log_parasite_density_clinical_from"] || !node["log_parasite_density_clinical_to"] ||
           !node["log_parasite_density_detectable"] || !node["log_parasite_density_detectable_pfpr"] ||
           !node["log_parasite_density_pyrogenic"])
            throw std::runtime_error("Missing fields in ParasiteDensityLevels");

        rhs.set_log_parasite_density_cured(node["log_parasite_density_cured"].as<double>());
        rhs.set_log_parasite_density_from_liver(node["log_parasite_density_from_liver"].as<double>());
        rhs.set_log_parasite_density_asymptomatic(node["log_parasite_density_asymptomatic"].as<double>());
        rhs.set_log_parasite_density_clinical(node["log_parasite_density_clinical"].as<double>());
        rhs.set_log_parasite_density_clinical_from(node["log_parasite_density_clinical_from"].as<double>());
        rhs.set_log_parasite_density_clinical_to(node["log_parasite_density_clinical_to"].as<double>());
        rhs.set_log_parasite_density_detectable(node["log_parasite_density_detectable"].as<double>());
        rhs.set_log_parasite_density_detectable_pfpr(node["log_parasite_density_detectable_pfpr"].as<double>());
        rhs.set_log_parasite_density_pyrogenic(node["log_parasite_density_pyrogenic"].as<double>());
        return true;
    }
};

// RecombinationParameters YAML conversion
template<>
struct convert<RecombinationParameters> {
    static Node encode(const RecombinationParameters& rhs) {
        Node node;
        node["within_chromosome_recombination_rate"] = rhs.get_within_chromosome_recombination_rate();
        node["using_free_recombination"] = rhs.get_using_free_recombination();
        return node;
    }

    static bool decode(const Node& node, RecombinationParameters& rhs) {
        if (!node["within_chromosome_recombination_rate"] || !node["using_free_recombination"])
            throw std::runtime_error("Missing fields in RecombinationParameters");

        rhs.set_within_chromosome_recombination_rate(node["within_chromosome_recombination_rate"].as<double>());
        rhs.set_using_free_recombination(node["using_free_recombination"].as<bool>());
        return true;
    }
};

// ParasiteParameters YAML conversion
template<>
struct convert<ParasiteParameters> {
    static Node encode(const ParasiteParameters& rhs) {
        Node node;
        node["parasite_density_levels"] = rhs.get_parasite_density_levels();
        node["recombination_parameters"] = rhs.get_recombination_parameters();
        return node;
    }

    static bool decode(const Node& node, ParasiteParameters& rhs) {
        if (!node["parasite_density_levels"] || !node["recombination_parameters"])
            throw std::runtime_error("Missing fields in ParasiteParameters");

        rhs.set_parasite_density_levels(node["parasite_density_levels"].as<ParasiteDensityLevels>());
        rhs.set_recombination_parameters(node["recombination_parameters"].as<RecombinationParameters>());
        return true;
    }
};
}

#endif //PARASITEPARAMETERS_H
