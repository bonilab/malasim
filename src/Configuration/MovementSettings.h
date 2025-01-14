#include <string>
#include <stdexcept>

#include "Spatial/Movement/BarabasiSM.hxx"
#include "Spatial/Movement/BurkinaFasoSM.hxx"
#include "Spatial/Movement/MarshallSM.hxx"
#include "Spatial/Movement/WesolowskiSM.hxx"
#include "Spatial/Movement/WesolowskiSurfaceSM.hxx"

class MovementSettings : IConfigClass {
public:
    class BarabasiSM {
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

    class WesolowskiSM{
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

    class WesolowskiSurfaceSM{
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

    class MarshallSM {
    public:
      [[nodiscard]] double get_tau() const { return tau_; }
      void set_tau(double value) { tau_ = value; }

      [[nodiscard]] double get_alpha() const { return alpha_; }
      void set_alpha(double value) { alpha_ = value; }

      [[nodiscard]] double get_log_rho() const { return log_rho_; }
      void set_log_rho(double value) { log_rho_ = value; }

    private:
      double tau_;
      double alpha_;
      double log_rho_;
    };

    class BurkinaFasoSM {
    public:
      [[nodiscard]] double get_tau() const { return tau_; }
      void set_tau(double value) { tau_ = value; }

      [[nodiscard]] double get_alpha() const { return alpha_; }
      void set_alpha(double value) { alpha_ = value; }

      [[nodiscard]] double get_log_rho() const { return log_rho_; }
      void set_log_rho(double value) { log_rho_ = value; }

      [[nodiscard]] double get_capital() const { return capital_; }
      void set_capital(double value) { capital_ = value; }

      [[nodiscard]] double get_penalty() const { return penalty_; }
      void set_penalty(double value) { penalty_ = value; }

    private:
      double tau_;
      double alpha_;
      double log_rho_;
      double capital_;
      double penalty_;
    };

  class SpatialModelSettings {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_name() const { return name_; }
        void set_name(const std::string& value) { name_ = value; }

        [[nodiscard]] const BarabasiSM& get_barabasi_sm() const { return barabasi_sm_; }
        void set_barabasi_sm(const BarabasiSM& value) { barabasi_sm_ = value; }

        [[nodiscard]] const WesolowskiSM& get_wesolowski_sm() const { return wesolowski_sm_; }
        void set_wesolowski_sm(const WesolowskiSM& value) { wesolowski_sm_ = value; }

        [[nodiscard]] const MarshallSM& get_marshall_sm() const { return marshall_sm_; }
        void set_marshall_sm(const MarshallSM& value) { marshall_sm_ = value; }

        [[nodiscard]] const BurkinaFasoSM& get_burkina_faso_sm() const { return burkina_faso_sm_; }
        void set_burkina_faso_sm(const BurkinaFasoSM& value) { burkina_faso_sm_ = value; }

        [[nodiscard]] const WesolowskiSurfaceSM& get_wesolowski_surface_sm() const { return wesolowski_surface_sm_; }
        void set_wesolowski_surface_sm(const WesolowskiSurfaceSM& value) { wesolowski_surface_sm_ = value; }

    private:
        std::string name_;
        BarabasiSM barabasi_sm_;
        WesolowskiSM wesolowski_sm_;
        WesolowskiSurfaceSM wesolowski_surface_sm_;
        MarshallSM marshall_sm_;
        BurkinaFasoSM burkina_faso_sm_;
    };

    class MovingLevelDistributionExponential {
    public:
        // Getters and Setters
        [[nodiscard]] double get_mean() const { return mean_; }
        void set_mean(double value) { mean_ = value; }

        [[nodiscard]] double get_sd() const { return sd_; }
        void set_sd(double value) { sd_ = value; }

        [[nodiscard]] double get_scale() const { return scale_; }
        void set_scale(double value) { scale_ = value; }

    private:
        double scale_;
        double mean_;
        double sd_;
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

        [[nodiscard]] const MovingLevelDistributionExponential& get_exponential() const { return exponential_; }
        void set_exponential(const MovingLevelDistributionExponential& value) { exponential_ = value; }

    private:
        std::string distribution_;
        MovingLevelDistributionGamma gamma_;
        MovingLevelDistributionExponential exponential_;
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

    // Getters and Setters
    [[nodiscard]] const SpatialModelSettings& get_spatial_model_settings() const { return spatial_model_settings_; }
    void set_spatial_model_settings(const SpatialModelSettings& value) { spatial_model_settings_ = value; }

    [[nodiscard]] Spatial::SpatialModel* get_spatial_model() const { return spatial_model_; }
    void set_spatial_model(Spatial::SpatialModel* value) { spatial_model_ = value; }

    [[nodiscard]] const CirculationInfo& get_circulation_info() const { return circulation_info_; }
    void set_circulation_info(const CirculationInfo& value) { circulation_info_ = value; }

  void process_config() override {}

  void process_config_using_spatial_settings(
    std::vector<std::vector<double>> spatial_distance_matrix,int number_of_locations) {
    spdlog::info("Processing MovementSettings");
    if(spatial_model_settings_.get_name() == "Barabasi") {
      spdlog::info("Processing BarabasiSM");
      spatial_model_ = new Spatial::BarabasiSM(spatial_model_settings_.get_barabasi_sm().get_r_g_0(),
                                               spatial_model_settings_.get_barabasi_sm().get_beta_r(),
                                               spatial_model_settings_.get_barabasi_sm().get_kappa());
    } else if(spatial_model_settings_.get_name() == "Wesolowski") {
      spdlog::info("Processing WesolowskiSM");
      spatial_model_ = new Spatial::WesolowskiSM(spatial_model_settings_.get_wesolowski_sm().get_kappa(),
                                                 spatial_model_settings_.get_wesolowski_sm().get_alpha(),
                                                 spatial_model_settings_.get_wesolowski_sm().get_beta(),
                                                 spatial_model_settings_.get_wesolowski_sm().get_gamma());
    } else if(spatial_model_settings_.get_name() == "WesolowskiSurface") {
      spdlog::info("Processing WesolowskiSurfaceSM");
      spatial_model_ = new Spatial::WesolowskiSurfaceSM(spatial_model_settings_.get_wesolowski_surface_sm().get_kappa(),
                                                            spatial_model_settings_.get_wesolowski_surface_sm().get_alpha(),
                                                            spatial_model_settings_.get_wesolowski_surface_sm().get_beta(),
                                                            spatial_model_settings_.get_wesolowski_surface_sm().get_gamma(),
                                                            number_of_locations);
    } else if(spatial_model_settings_.get_name() == "Marshall") {
      spdlog::info("Processing MarshallSM");
      spatial_model_ = new Spatial::MarshallSM(spatial_model_settings_.get_marshall_sm().get_tau(),
                                               spatial_model_settings_.get_marshall_sm().get_alpha(),
                                               spatial_model_settings_.get_marshall_sm().get_log_rho(),
                                               number_of_locations,
                                               spatial_distance_matrix);
    } else if(spatial_model_settings_.get_name() == "BurkinaFaso") {
      spdlog::info("Processing BurkinaFasoSM");
      spatial_model_ = new Spatial::BurkinaFasoSM(spatial_model_settings_.get_burkina_faso_sm().get_tau(),
                                                  spatial_model_settings_.get_burkina_faso_sm().get_alpha(),
                                                  spatial_model_settings_.get_burkina_faso_sm().get_log_rho(),
                                                  spatial_model_settings_.get_burkina_faso_sm().get_capital(),
                                                  spatial_model_settings_.get_burkina_faso_sm().get_penalty(),
                                                  number_of_locations,
                                                  spatial_distance_matrix);
    }
  }

private:
    SpatialModelSettings spatial_model_settings_;
    Spatial::SpatialModel* spatial_model_;
    CirculationInfo circulation_info_;
};

namespace YAML {
// BarabasiSM YAML conversion
template<>
struct convert<MovementSettings::BarabasiSM> {
    static Node encode(const MovementSettings::BarabasiSM& rhs) {
        Node node;
        node["r_g_0"] = rhs.get_r_g_0();
        node["beta_r"] = rhs.get_beta_r();
        node["kappa"] = rhs.get_kappa();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::BarabasiSM& rhs) {
        if(!node["r_g_0"] || !node["beta_r"] || !node["kappa"])
            throw std::runtime_error("Missing fields in BarabasiSettings");

        rhs.set_r_g_0(node["r_g_0"].as<double>());
        rhs.set_beta_r(node["beta_r"].as<double>());
        rhs.set_kappa(node["kappa"].as<int>());
        return true;
    }
};


// WesolowskiSM YAML conversion
template<>
struct convert<MovementSettings::WesolowskiSM> {
    static Node encode(const MovementSettings::WesolowskiSM& rhs) {
        Node node;
        node["kappa"] = rhs.get_kappa();
        node["alpha"] = rhs.get_alpha();
        node["beta"] = rhs.get_beta();
        node["gamma"] = rhs.get_gamma();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::WesolowskiSM& rhs) {
        if(!node["kappa"] || !node["alpha"] || !node["beta"] || !node["gamma"])
            throw std::runtime_error("Missing fields in WesolowskiSM");

        rhs.set_kappa(node["kappa"].as<double>());
        rhs.set_alpha(node["alpha"].as<double>());
        rhs.set_beta(node["beta"].as<double>());
        rhs.set_gamma(node["gamma"].as<double>());
        return true;
    }
};

// WesolowskiSurfaceSM YAML conversion
template<>
struct convert<MovementSettings::WesolowskiSurfaceSM> {
  static Node encode(const MovementSettings::WesolowskiSurfaceSM& rhs) {
    Node node;
    node["kappa"] = rhs.get_kappa();
    node["alpha"] = rhs.get_alpha();
    node["beta"] = rhs.get_beta();
    node["gamma"] = rhs.get_gamma();
    return node;
  }

  static bool decode(const Node& node, MovementSettings::WesolowskiSurfaceSM& rhs) {
    if(!node["kappa"] || !node["alpha"] || !node["beta"] || !node["gamma"])
      throw std::runtime_error("Missing fields in WesolowskiSurfaceSM");

    rhs.set_kappa(node["kappa"].as<double>());
    rhs.set_alpha(node["alpha"].as<double>());
    rhs.set_beta(node["beta"].as<double>());
    rhs.set_gamma(node["gamma"].as<double>());
    return true;
  }
};

// MarshallSM YAML conversion
template<>
struct convert<MovementSettings::MarshallSM> {
  static Node encode(const MovementSettings::MarshallSM& rhs) {
    Node node;
    node["tau"] = rhs.get_tau();
    node["alpha"] = rhs.get_alpha();
    node["log_rho"] = rhs.get_log_rho();
    return node;
  }

  static bool decode(const Node& node, MovementSettings::MarshallSM& rhs) {
    if(!node["tau"] || !node["alpha"] || !node["log_rho"])
      throw std::runtime_error("Missing fields in MarshallSM");

    rhs.set_tau(node["tau"].as<double>());
    rhs.set_alpha(node["alpha"].as<double>());
    rhs.set_log_rho(node["log_rho"].as<double>());
    return true;
  }
};

// BurkinaFasoSM YAML conversion
template<>
struct convert<MovementSettings::BurkinaFasoSM> {
  static Node encode(const MovementSettings::BurkinaFasoSM& rhs) {
    Node node;
    node["tau"] = rhs.get_tau();
    node["alpha"] = rhs.get_alpha();
    node["log_rho"] = rhs.get_log_rho();
    node["capital"] = rhs.get_capital();
    node["penalty"] = rhs.get_penalty();
    return node;
  }

  static bool decode(const Node& node, MovementSettings::BurkinaFasoSM& rhs) {
    if(!node["tau"] || !node["alpha"] || !node["log_rho"] || !node["capital"] || !node["penalty"])
      throw std::runtime_error("Missing fields in BurkinaFasoSM");

    rhs.set_tau(node["tau"].as<double>());
    rhs.set_alpha(node["alpha"].as<double>());
    rhs.set_log_rho(node["log_rho"].as<double>());
    rhs.set_capital(node["capital"].as<double>());
    rhs.set_penalty(node["penalty"].as<double>());
    return true;
  }
};

// SpatialModel YAML conversion
template<>
struct convert<MovementSettings::SpatialModelSettings> {
    static Node encode(const MovementSettings::SpatialModelSettings& rhs) {
        Node node;
        node["name"] = rhs.get_name();
        node["Barabasi"] = rhs.get_barabasi_sm();
        node["Wesolowski"] = rhs.get_wesolowski_sm();
        node["Marshall"] = rhs.get_marshall_sm();
        node["BurkinaFaso"] = rhs.get_burkina_faso_sm();
        node["WesolowskiSurface"] = rhs.get_wesolowski_surface_sm();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::SpatialModelSettings& rhs) {
      if(!node["name"]) {
        throw std::runtime_error("Missing fields in SpatialModel");
        }
      rhs.set_name(node["name"].as<std::string>());

      if(node["Barabasi"]) {
        rhs.set_barabasi_sm(node["Barabasi"].as<MovementSettings::BarabasiSM>());
      }
      if(node["Wesolowski"]) {
          rhs.set_wesolowski_sm(node["Wesolowski"].as<MovementSettings::WesolowskiSM>());
      }
      if(node["Marshall"]) {
          rhs.set_marshall_sm(node["Marshall"].as<MovementSettings::MarshallSM>());
      }
      if(node["BurkinaFaso"]) {
          rhs.set_burkina_faso_sm(node["BurkinaFaso"].as<MovementSettings::BurkinaFasoSM>());
      }
      if(node["WesolowskiSurface"]) {
          rhs.set_wesolowski_surface_sm(node["WesolowskiSurface"].as<MovementSettings::WesolowskiSurfaceSM>());
      }
      return true;
    }
};

// ExponentialDistribution YAML conversion
template<>
struct convert<MovementSettings::MovingLevelDistributionExponential> {
    static Node encode(const MovementSettings::MovingLevelDistributionExponential& rhs) {
        Node node;
        node["scale"] = rhs.get_scale();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::MovingLevelDistributionExponential& rhs) {
        if(!node["scale"])
            throw std::runtime_error("Missing fields in ExponentialDistribution");

        rhs.set_scale(node["scale"].as<double>());
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
        node["Exponential"] = rhs.get_exponential();
        return node;
    }

    static bool decode(const Node& node, MovementSettings::MovingLevelDistribution& rhs) {
        if(!node["distribution"] || !node["Gamma"] || !node["Exponential"])
            throw std::runtime_error("Missing fields in MovingLevelDistribution");

        rhs.set_distribution(node["distribution"].as<std::string>());
        rhs.set_gamma(node["Gamma"].as<MovementSettings::MovingLevelDistributionGamma>());
        rhs.set_exponential(node["Exponential"].as<MovementSettings::MovingLevelDistributionExponential>());
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
        node["spatial_model"] = rhs.get_spatial_model_settings();
        node["circulation_info"] = rhs.get_circulation_info();
        return node;
    }

    static bool decode(const Node& node, MovementSettings& rhs) {
        if(!node["spatial_model"] || !node["circulation_info"])
            throw std::runtime_error("Missing fields in MovementSettings");

        rhs.set_spatial_model_settings(node["spatial_model"].as<MovementSettings::SpatialModelSettings>());
        rhs.set_circulation_info(node["circulation_info"].as<MovementSettings::CirculationInfo>());
        return true;
    }
};
}
