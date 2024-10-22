#ifndef GENOTYPEPARAMETERS_H
#define GENOTYPEPARAMETERS_H
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <stdexcept>

class GenotypeParameters {
public:
    // Inner class: MultiplicativeEffectOnEC50For2OrMoreMutations
    class MultiplicativeEffectOnEC50For2OrMoreMutations {
    public:
        // Getters and Setters
        [[nodiscard]] int get_drug_id() const { return drug_id_; }
        void set_drug_id(int value) { drug_id_ = value; }

        [[nodiscard]] const double& get_factor() const { return factor_; }
        void set_factor(const double& value) { factor_ = value; }

    private:
        int drug_id_;
        double factor_;
    };

    // Inner class: MultiplicativeEffectOnEC50
    class MultiplicativeEffectOnEC50 {
    public:
        // Getters and Setters
        [[nodiscard]] int get_drug_id() const { return drug_id_; }
        void set_drug_id(int value) { drug_id_ = value; }

        [[nodiscard]] const std::vector<double>& get_factors() const { return factors_; }
        void set_factors(const std::vector<double>& value) { factors_ = value; }

    private:
        int drug_id_;
        std::vector<double> factors_;
    };

    // Inner class: AminoAcidPosition
    class AminoAcidPosition {
    public:
        // Getters and Setters
        [[nodiscard]] int get_position() const { return position_; }
        void set_position(int value) { position_ = value; }

        [[nodiscard]] const std::vector<std::string>& get_amino_acids() const { return amino_acids_; }
        void set_amino_acids(const std::vector<std::string>& value) { amino_acids_ = value; }

        [[nodiscard]] const std::vector<double>& get_daily_crs() const { return daily_crs_; }
        void set_daily_crs(const std::vector<double>& value) { daily_crs_ = value; }

        [[nodiscard]] const std::vector<MultiplicativeEffectOnEC50>& get_multiplicative_effect_on_EC50() const { return multiplicative_effect_on_EC50_; }
        void set_multiplicative_effect_on_EC50(const std::vector<MultiplicativeEffectOnEC50>& value) { multiplicative_effect_on_EC50_ = value; }

    private:
        int position_;
        std::vector<std::string> amino_acids_;
        std::vector<double> daily_crs_;
        std::vector<MultiplicativeEffectOnEC50> multiplicative_effect_on_EC50_;
    };

    // Inner class: GeneInfo
    class GeneInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_name() const { return name_; }
        void set_name(const std::string& value) { name_ = value; }

        [[nodiscard]] int get_max_copies() const { return max_copies_; }
        void set_max_copies(int value) { max_copies_ = value; }

        [[nodiscard]] const std::vector<double>& get_cnv_daily_crs() const { return cnv_daily_crs_; }
        void set_cnv_daily_crs(const std::vector<double>& value) { cnv_daily_crs_ = value; }

        [[nodiscard]] const std::vector<MultiplicativeEffectOnEC50>& get_cnv_multiplicative_effect_on_EC50() const { return cnv_multiplicative_effect_on_EC50_; }
        void set_cnv_multiplicative_effect_on_EC50(const std::vector<MultiplicativeEffectOnEC50>& value) { cnv_multiplicative_effect_on_EC50_ = value; }

        [[nodiscard]] const std::vector<AminoAcidPosition>& get_aa_positions() const { return aa_positions_; }
        void set_aa_positions(const std::vector<AminoAcidPosition>& value) { aa_positions_ = value; }

        [[nodiscard]] const std::vector<MultiplicativeEffectOnEC50For2OrMoreMutations>& get_multiplicative_effect_on_ec50_for_2_or_more_mutations() const { return multiplicative_effect_on_ec50_for_2_or_more_mutations_; }
        void set_multiplicative_effect_on_ec50_for_2_or_more_mutations(const std::vector<MultiplicativeEffectOnEC50For2OrMoreMutations>& value) { multiplicative_effect_on_ec50_for_2_or_more_mutations_ = value; }

        [[nodiscard]] double get_average_daily_crs() const { return average_daily_crs_; }
        void set_average_daily_crs(double value) { average_daily_crs_ = value; }

    private:
        std::string name_;
        int max_copies_;
        std::vector<double> cnv_daily_crs_;
        std::vector<MultiplicativeEffectOnEC50> cnv_multiplicative_effect_on_EC50_;
        std::vector<AminoAcidPosition> aa_positions_;
        std::vector<MultiplicativeEffectOnEC50> multiplicative_effect_on_EC50_for_2_or_more_mutations_;
        std::vector<MultiplicativeEffectOnEC50For2OrMoreMutations> multiplicative_effect_on_ec50_for_2_or_more_mutations_;
        double average_daily_crs_;
    };

    // Inner class: ChromosomeInfo
    class ChromosomeInfo {
    public:
        // Getters and Setters
        [[nodiscard]] int get_chromosome() const { return chromosome_; }
        void set_chromosome(int value) { chromosome_ = value; }

        [[nodiscard]] const std::vector<GeneInfo>& get_genes() const { return genes_; }
        void set_genes(const std::vector<GeneInfo>& value) { genes_ = value; }

    private:
        int chromosome_;
        std::vector<GeneInfo> genes_;
    };

    // Inner class: OverrideEC50Pattern
    class OverrideEC50Pattern {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_pattern() const { return pattern_; }
        void set_pattern(const std::string& value) { pattern_ = value; }

        [[nodiscard]] int get_drug_id() const { return drug_id_; }
        void set_drug_id(int value) { drug_id_ = value; }

        [[nodiscard]] double get_ec50() const { return ec50_; }
        void set_ec50(double value) { ec50_ = value; }

    private:
        std::string pattern_;
        int drug_id_;
        double ec50_;
    };

    // Inner class: GenotypeInfo
    class GenotypeInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_aa_sequence() const { return aa_sequence_; }
        void set_aa_sequence(const std::string& value) { aa_sequence_ = value; }

        [[nodiscard]] double get_prevalence() const { return prevalence_; }
        void set_prevalence(double value) { prevalence_ = value; }

    private:
        std::string aa_sequence_;
        double prevalence_;
    };

    // Inner class: InitialGenotypeInfo
    class InitialGenotypeInfo {
    public:
        // Getters and Setters
        [[nodiscard]] int get_location_id() const { return location_id_; }
        void set_location_id(int value) { location_id_ = value; }

        [[nodiscard]] const std::vector<GenotypeInfo>& get_genotype_info() const { return genotype_info_; }
        void set_genotype_info(const std::vector<GenotypeInfo>& value) { genotype_info_ = value; }

    private:
        int location_id_;
        std::vector<GenotypeInfo> genotype_info_;
    };

    // Getters and Setters for GenotypeParameters
    [[nodiscard]] const std::string& get_mutation_mask() const { return mutation_mask_; }
    void set_mutation_mask(const std::string& value) { mutation_mask_ = value; }

    [[nodiscard]] double get_mutation_probability_per_locus() const { return mutation_probability_per_locus_; }
    void set_mutation_probability_per_locus(double value) { mutation_probability_per_locus_ = value; }

    [[nodiscard]] const std::vector<ChromosomeInfo>& get_pf_genotype_info() const { return pf_genotype_info_; }
    void set_pf_genotype_info(const std::vector<ChromosomeInfo>& value) { pf_genotype_info_ = value; }

    [[nodiscard]] const std::vector<OverrideEC50Pattern>& get_override_ec50_patterns() const { return override_ec50_patterns_; }
    void set_override_ec50_patterns(const std::vector<OverrideEC50Pattern>& value) { override_ec50_patterns_ = value; }

    [[nodiscard]] const std::vector<InitialGenotypeInfo>& get_initial_genotype_info() const { return initial_genotype_info_; }
    void set_initial_genotype_info(const std::vector<InitialGenotypeInfo>& value) { initial_genotype_info_ = value; }

private:
    std::string mutation_mask_;
    double mutation_probability_per_locus_;
    std::vector<ChromosomeInfo> pf_genotype_info_;
    std::vector<OverrideEC50Pattern> override_ec50_patterns_;
    std::vector<InitialGenotypeInfo> initial_genotype_info_;
};

namespace YAML {

// GenotypeParameters::MultiplicativeEffectOnEC50For2OrMoreMutations YAML conversion
template<>
struct convert<GenotypeParameters::MultiplicativeEffectOnEC50For2OrMoreMutations> {
    static Node encode(const GenotypeParameters::MultiplicativeEffectOnEC50For2OrMoreMutations& rhs) {
        Node node;
        node["drug_id"] = rhs.get_drug_id();
        node["factor"] = rhs.get_factor();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::MultiplicativeEffectOnEC50For2OrMoreMutations& rhs) {
        if (!node["drug_id"] || !node["factor"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::MultiplicativeEffectOnEC50For2OrMoreMutations");
        }
        rhs.set_drug_id(node["drug_id"].as<int>());
        rhs.set_factor(node["factor"].as<double>());
        return true;
    }
};

// GenotypeParameters::MultiplicativeEffectOnEC50 YAML conversion
template<>
struct convert<GenotypeParameters::MultiplicativeEffectOnEC50> {
    static Node encode(const GenotypeParameters::MultiplicativeEffectOnEC50& rhs) {
        Node node;
        node["drug_id"] = rhs.get_drug_id();
        node["factors"] = rhs.get_factors();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::MultiplicativeEffectOnEC50& rhs) {
        if (!node["drug_id"] || !node["factors"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::MultiplicativeEffectOnEC50");
        }
        rhs.set_drug_id(node["drug_id"].as<int>());
        rhs.set_factors(node["factors"].as<std::vector<double>>());
        return true;
    }
};

// GenotypeParameters::AminoAcidPosition YAML conversion
template<>
struct convert<GenotypeParameters::AminoAcidPosition> {
    static Node encode(const GenotypeParameters::AminoAcidPosition& rhs) {
        Node node;
        node["position"] = rhs.get_position();
        node["amino_acids"] = rhs.get_amino_acids();
        node["daily_crs"] = rhs.get_daily_crs();
        node["multiplicative_effect_on_EC50"] = rhs.get_multiplicative_effect_on_EC50();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::AminoAcidPosition& rhs) {
        if (!node["position"] || !node["amino_acids"] || !node["daily_crs"] || !node["multiplicative_effect_on_EC50"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::AminoAcidPosition");
        }
        rhs.set_position(node["position"].as<int>());
        rhs.set_amino_acids(node["amino_acids"].as<std::vector<std::string>>());
        rhs.set_daily_crs(node["daily_crs"].as<std::vector<double>>());
        rhs.set_multiplicative_effect_on_EC50(node["multiplicative_effect_on_EC50"].as<std::vector<GenotypeParameters::MultiplicativeEffectOnEC50>>());
        return true;
    }
};
template<typename T>
void optional_decode(const YAML::Node& node, const std::string& key, T& value) {
    if (node[key]) {
        value = node[key].as<T>();
    }
}

// GenotypeParameters::GeneInfo YAML conversion
template<>
struct convert<GenotypeParameters::GeneInfo> {
    static Node encode(const GenotypeParameters::GeneInfo& rhs) {
        Node node;
        node["name"] = rhs.get_name();
        node["aa_positions"] = rhs.get_aa_positions();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::GeneInfo& rhs) {
        if (!node["name"] || !node["aa_positions"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::GeneInfo");
        }
        rhs.set_name(node["name"].as<std::string>());
        if(node["aa_positions"])
            rhs.set_aa_positions(node["aa_positions"].as<std::vector<GenotypeParameters::AminoAcidPosition>>());
        if(node["multiplicative_effect_on_EC50_for_2_or_more_mutations"])
            rhs.set_multiplicative_effect_on_ec50_for_2_or_more_mutations(node["multiplicative_effect_on_EC50_for_2_or_more_mutations"].as<std::vector<GenotypeParameters::MultiplicativeEffectOnEC50For2OrMoreMutations>>());
        if(node["max_copies"])
            rhs.set_max_copies(node["max_copies"].as<int>());
        if(node["cnv_daily_crs"])
            rhs.set_cnv_daily_crs(node["cnv_daily_crs"].as<std::vector<double>>());
        if(node["cnv_multiplicative_effect_on_EC50"])
            rhs.set_cnv_multiplicative_effect_on_EC50(node["cnv_multiplicative_effect_on_EC50"].as<std::vector<GenotypeParameters::MultiplicativeEffectOnEC50>>());
        if(node["average_daily_crs"])
            rhs.set_average_daily_crs(node["average_daily_crs"].as<double>());
        return true;
    }
};

// GenotypeParameters::ChromosomeInfo YAML conversion
template<>
struct convert<GenotypeParameters::ChromosomeInfo> {
    static Node encode(const GenotypeParameters::ChromosomeInfo& rhs) {
        Node node;
        node["chromosome"] = rhs.get_chromosome();
        node["genes"] = rhs.get_genes();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::ChromosomeInfo& rhs) {
        if (!node["chromosome"] || !node["genes"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::ChromosomeInfo");
        }
        rhs.set_chromosome(node["chromosome"].as<int>());
        rhs.set_genes(node["genes"].as<std::vector<GenotypeParameters::GeneInfo>>());
        return true;
    }
};

// GenotypeParameters::OverrideEC50Pattern YAML conversion
template<>
struct convert<GenotypeParameters::OverrideEC50Pattern> {
    static Node encode(const GenotypeParameters::OverrideEC50Pattern& rhs) {
        Node node;
        node["pattern"] = rhs.get_pattern();
        node["drug_id"] = rhs.get_drug_id();
        node["ec50"] = rhs.get_ec50();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::OverrideEC50Pattern& rhs) {
        if (!node["pattern"] || !node["drug_id"] || !node["ec50"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::OverrideEC50Pattern");
        }
        rhs.set_pattern(node["pattern"].as<std::string>());
        rhs.set_drug_id(node["drug_id"].as<int>());
        rhs.set_ec50(node["ec50"].as<double>());
        return true;
    }
};

// GenotypeParameters::GenotypeInfo YAML conversion
template<>
struct convert<GenotypeParameters::GenotypeInfo> {
    static Node encode(const GenotypeParameters::GenotypeInfo& rhs) {
        Node node;
        node["aa_sequence"] = rhs.get_aa_sequence();
        node["prevalence"] = rhs.get_prevalence();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::GenotypeInfo& rhs) {
        if (!node["aa_sequence"] || !node["prevalence"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::GenotypeInfo");
        }
        rhs.set_aa_sequence(node["aa_sequence"].as<std::string>());
        rhs.set_prevalence(node["prevalence"].as<double>());
        return true;
    }
};

// GenotypeParameters::InitialGenotypeInfo YAML conversion
template<>
struct convert<GenotypeParameters::InitialGenotypeInfo> {
    static Node encode(const GenotypeParameters::InitialGenotypeInfo& rhs) {
        Node node;
        node["location_id"] = rhs.get_location_id();
        node["genotype_info"] = rhs.get_genotype_info();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters::InitialGenotypeInfo& rhs) {
        if (!node["location_id"] || !node["genotype_info"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters::InitialGenotypeInfo");
        }
        rhs.set_location_id(node["location_id"].as<int>());
        rhs.set_genotype_info(node["genotype_info"].as<std::vector<GenotypeParameters::GenotypeInfo>>());
        return true;
    }
};

// GenotypeParameters YAML conversion
template<>
struct convert<GenotypeParameters> {
    static Node encode(const GenotypeParameters& rhs) {
        Node node;
        node["mutation_mask"] = rhs.get_mutation_mask();
        node["mutation_probability_per_locus"] = rhs.get_mutation_probability_per_locus();
        node["pf_genotype_info"] = rhs.get_pf_genotype_info();
        node["override_ec50_patterns"] = rhs.get_override_ec50_patterns();
        node["initial_genotype_info"] = rhs.get_initial_genotype_info();
        return node;
    }

    static bool decode(const Node& node, GenotypeParameters& rhs) {
        if (!node["mutation_mask"] || !node["mutation_probability_per_locus"] || !node["pf_genotype_info"]
            || !node["override_ec50_patterns"] || !node["initial_genotype_info"]) {
            throw std::runtime_error("Missing fields in GenotypeParameters");
        }
        rhs.set_mutation_mask(node["mutation_mask"].as<std::string>());
        rhs.set_mutation_probability_per_locus(node["mutation_probability_per_locus"].as<double>());
        rhs.set_pf_genotype_info(node["pf_genotype_info"].as<std::vector<GenotypeParameters::ChromosomeInfo>>());
        rhs.set_override_ec50_patterns(node["override_ec50_patterns"].as<std::vector<GenotypeParameters::OverrideEC50Pattern>>());
        rhs.set_initial_genotype_info(node["initial_genotype_info"].as<std::vector<GenotypeParameters::InitialGenotypeInfo>>());
        return true;
    }
};

}  // namespace YAML

#endif //GENOTYPEPARAMETERS_H
