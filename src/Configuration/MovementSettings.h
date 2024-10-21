#include <string>
#include <stdexcept>
#include "date/date.h"

class MovementSettings {
public:
    class BarabasiSettings {
    public:
        // Getters and Setters
        [[nodiscard]] double get_r_g_0() const { return r_g_0_; }
        void set_r_g_0(double value) { r_g_0_ = value; }

        [[nodiscard]] double get_beta_r() const { return beta_r_; }
        void set_beta_r(double value) { beta_r_ = value; }

        [[nodiscard]] int get_kappa() const { return kappa_; }
        void set_kappa(int value) {
            if (value <= 0) throw std::invalid_argument("kappa must be positive");
            kappa_ = value;
        }

    private:
        double r_g_0_;
        double beta_r_;
        int kappa_;
    };

    class WesolowskiSettings {
    public:
        // Getters and Setters
        [[nodiscard]] double get_kappa() const { return kappa_; }
        void set_kappa(double value) { kappa_ = value; }

        [[nodiscard]] double get_alpha() const { return alpha_; }
        void set_alpha(double value) { alpha_ = value; }

        [[nodiscard]] double get_beta() const { return beta_; }
        void set_beta(double value) { beta_ = value; }

        [[nodiscard]] double get_gamma() const { return gamma_; }
        void set_gamma(double value) { gamma_ = value; }

    private:
        double kappa_;
        double alpha_;
        double beta_;
        double gamma_;
    };

    class SpatialModel {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_name() const { return name_; }
        void set_name(const std::string& value) { name_ = value; }

        [[nodiscard]] const BarabasiSettings& get_barabasi() const { return barabasi_; }
        void set_barabasi(const BarabasiSettings& value) { barabasi_ = value; }

        [[nodiscard]] const WesolowskiSettings& get_wesolowski() const { return wesolowski_; }
        void set_wesolowski(const WesolowskiSettings& value) { wesolowski_ = value; }

    private:
        std::string name_;
        BarabasiSettings barabasi_;
        WesolowskiSettings wesolowski_;
    };

    class MovingLevelDistributionGamma {
    public:
        // Getters and Setters
        [[nodiscard]] double get_mean() const { return mean_; }
        void set_mean(double value) { mean_ = value; }

        [[nodiscard]] double get_sd() const { return sd_; }
        void set_sd(double value) { sd_ = value; }

    private:
        double mean_;
        double sd_;
    };

    class MovingLevelDistribution {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_distribution() const { return distribution_; }
        void set_distribution(const std::string& value) { distribution_ = value; }

        [[nodiscard]] const MovingLevelDistributionGamma& get_gamma() const { return gamma_; }
        void set_gamma(const MovingLevelDistributionGamma& value) { gamma_ = value; }

    private:
        std::string distribution_;
        MovingLevelDistributionGamma gamma_;
    };

    class CirculationInfo {
    public:
        // Getters and Setters
        [[nodiscard]] int get_max_relative_moving_value() const { return max_relative_moving_value_; }
        void set_max_relative_moving_value(int value) {
            if (value < 0) throw std::invalid_argument("max_relative_moving_value must be non-negative");
            max_relative_moving_value_ = value;
        }

        [[nodiscard]] int get_number_of_moving_levels() const { return number_of_moving_levels_; }
        void set_number_of_moving_levels(int value) {
            if (value <= 0) throw std::invalid_argument("number_of_moving_levels must be positive");
            number_of_moving_levels_ = value;
        }

        [[nodiscard]] const MovingLevelDistribution& get_moving_level_distribution() const { return moving_level_distribution_; }
        void set_moving_level_distribution(const MovingLevelDistribution& value) { moving_level_distribution_ = value; }

        [[nodiscard]] double get_circulation_percent() const { return circulation_percent_; }
        void set_circulation_percent(double value) {
            if (value < 0 || value > 100) throw std::invalid_argument("circulation_percent must be between 0 and 100");
            circulation_percent_ = value;
        }

        [[nodiscard]] const MovingLevelDistributionGamma& get_length_of_stay() const { return length_of_stay_; }
        void set_length_of_stay(const MovingLevelDistributionGamma& value) { length_of_stay_ = value; }

    private:
        int max_relative_moving_value_;
        int number_of_moving_levels_;
        MovingLevelDistribution moving_level_distribution_;
        double circulation_percent_;
        MovingLevelDistributionGamma length_of_stay_;
    };
public:
    // Getters and Setters
    [[nodiscard]] const SpatialModel& get_spatial_model() const { return spatial_model_; }
    void set_spatial_model(const SpatialModel& value) { spatial_model_ = value; }

    [[nodiscard]] const CirculationInfo& get_circulation_info() const { return circulation_info_; }
    void set_circulation_info(const CirculationInfo& value) { circulation_info_ = value; }

private:
    SpatialModel spatial_model_;
    CirculationInfo circulation_info_;
};

namespace YAML {
// BarabasiSettings YAML conversion
template<>
struct convert<MovementSettings::BarabasiSettings> {
    static Node encode(const MovementSettings::BarabasiSettings& rhs) {
        Node node;
        node["r_g_0"] = rhs.get_r_g_0();
        node["beta_r"] = rhs.get_beta_r();
        node["kappa"] = rhs.get_kappa();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::BarabasiSettings& rhs) {
        if(!node["r_g_0"] || !node["beta_r"] || !node["kappa"])
            throw std::runtime_error("Missing fields in BarabasiSettings");

        rhs.set_r_g_0(node["r_g_0"].as<double>());
        rhs.set_beta_r(node["beta_r"].as<double>());
        rhs.set_kappa(node["kappa"].as<int>());
        return true;
    }
};

// WesolowskiSettings YAML conversion
template<>
struct convert<MovementSettings::WesolowskiSettings> {
    static Node encode(const MovementSettings::WesolowskiSettings& rhs) {
        Node node;
        node["kappa"] = rhs.get_kappa();
        node["alpha"] = rhs.get_alpha();
        node["beta"] = rhs.get_beta();
        node["gamma"] = rhs.get_gamma();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::WesolowskiSettings& rhs) {
        if(!node["kappa"] || !node["alpha"] || !node["beta"] || !node["gamma"])
            throw std::runtime_error("Missing fields in WesolowskiSettings");

        rhs.set_kappa(node["kappa"].as<double>());
        rhs.set_alpha(node["alpha"].as<double>());
        rhs.set_beta(node["beta"].as<double>());
        rhs.set_gamma(node["gamma"].as<double>());
        return true;
    }
};

// SpatialModel YAML conversion
template<>
struct convert<MovementSettings::SpatialModel> {
    static Node encode(const MovementSettings::SpatialModel& rhs) {
        Node node;
        node["name"] = rhs.get_name();
        node["Barabasi"] = rhs.get_barabasi();
        node["Wesolowski"] = rhs.get_wesolowski();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::SpatialModel& rhs) {
        if(!node["name"] || !node["Barabasi"] || !node["Wesolowski"])
            throw std::runtime_error("Missing fields in MovementSettings/SpatialModel");

        rhs.set_name(node["name"].as<std::string>());
        rhs.set_barabasi(node["Barabasi"].as<MovementSettings::BarabasiSettings>());
        rhs.set_wesolowski(node["Wesolowski"].as<MovementSettings::WesolowskiSettings>());
        return true;
    }
};

// GammaDistribution YAML conversion
template<>
struct convert<MovementSettings::MovingLevelDistributionGamma> {
    static Node encode(const MovementSettings::MovingLevelDistributionGamma& rhs) {
        Node node;
        node["mean"] = rhs.get_mean();
        node["sd"] = rhs.get_sd();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::MovingLevelDistributionGamma& rhs) {
        if(!node["mean"] || !node["sd"])
            throw std::runtime_error("Missing fields in GammaDistribution");

        rhs.set_mean(node["mean"].as<double>());
        rhs.set_sd(node["sd"].as<double>());
        return true;
    }
};

// MovingLevelDistribution YAML conversion
template<>
struct convert<MovementSettings::MovingLevelDistribution> {
    static Node encode(const MovementSettings::MovingLevelDistribution& rhs) {
        Node node;
        node["distribution"] = rhs.get_distribution();
        node["Gamma"] = rhs.get_gamma();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::MovingLevelDistribution& rhs) {
        if(!node["distribution"] || !node["Gamma"])
            throw std::runtime_error("Missing fields in MovingLevelDistribution");

        rhs.set_distribution(node["distribution"].as<std::string>());
        rhs.set_gamma(node["Gamma"].as<MovementSettings::MovingLevelDistributionGamma>());
        return true;
    }
};

// CirculationInfo YAML conversion
template<>
struct convert<MovementSettings::CirculationInfo> {
    static Node encode(const MovementSettings::CirculationInfo& rhs) {
        Node node;
        node["max_relative_moving_value"] = rhs.get_max_relative_moving_value();
        node["number_of_moving_levels"] = rhs.get_number_of_moving_levels();
        node["moving_level_distribution"] = rhs.get_moving_level_distribution();
        node["circulation_percent"] = rhs.get_circulation_percent();
        node["length_of_stay"] = rhs.get_length_of_stay();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::CirculationInfo& rhs) {
        if(!node["max_relative_moving_value"] || !node["number_of_moving_levels"]
            || !node["moving_level_distribution"] || !node["circulation_percent"]
            || !node["length_of_stay"])
            throw std::runtime_error("Missing fields in CirculationInfo");

        rhs.set_max_relative_moving_value(node["max_relative_moving_value"].as<int>());
        rhs.set_number_of_moving_levels(node["number_of_moving_levels"].as<int>());
        rhs.set_moving_level_distribution(node["moving_level_distribution"].as<MovementSettings::MovingLevelDistribution>());
        rhs.set_circulation_percent(node["circulation_percent"].as<double>());
        rhs.set_length_of_stay(node["length_of_stay"].as<MovementSettings::MovingLevelDistributionGamma>());
        return true;
    }
};

// MovementSettings YAML conversion
template<>
struct convert<MovementSettings> {
    static Node encode(const MovementSettings& rhs) {
        Node node;
        node["spatial_model"] = rhs.get_spatial_model();
        node["circulation_info"] = rhs.get_circulation_info();
        return node;
    }

    static bool decode(const Node& node, MovementSettings& rhs) {
        if(!node["spatial_model"] || !node["circulation_info"])
            throw std::runtime_error("Missing fields in MovementSettings");

        rhs.set_spatial_model(node["spatial_model"].as<MovementSettings::SpatialModel>());
        rhs.set_circulation_info(node["circulation_info"].as<MovementSettings::CirculationInfo>());
        return true;
    }
};
}
