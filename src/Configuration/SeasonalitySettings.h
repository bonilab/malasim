#ifndef SEASONALITYSETTINGS_H
#define SEASONALITYSETTINGS_H

#include <string>
#include <vector>
#include "date/date.h"

// Class to hold the 'rainfall' settings
class RainfallSettings {
public:
  // Getters
  [[nodiscard]] const std::string &get_filename() const {
    return filename_;
  }

  // Setters
  void set_filename(const std::string &value) {
    filename_ = value;
  }

  [[nodiscard]] const date::days &get_period() const {
    return period_;
  }

  void set_period(const date::days &value) {
    if (value.count() <= 0)
      throw std::invalid_argument("Period must be greater than 0");
    period_ = value;
  }

private:
  std::string filename_;
  date::days period_;
};

// Class to hold the 'simple' settings
class SimpleSettings {
public:
  // Getters
  [[nodiscard]] const std::vector<double> &get_a() const {
    return a_;
  }

  // Setters
  void set_a(const std::vector<double> &value) {
    a_ = value;
  }

  [[nodiscard]] const std::vector<int> &get_phi() const {
    return phi_;
  }

  void set_phi(const std::vector<int> &value) {
    phi_ = value;
  }

  [[nodiscard]] const std::vector<double> &get_min_value() const {
    return min_value_;
  }

  void set_min_value(const std::vector<double> &value) {
    min_value_ = value;
  }

  [[nodiscard]] const date::days &get_period() const {
    return period_;
  }

  void set_period(const date::days &value) {
    if (value.count() <= 0)
      throw std::invalid_argument("Period must be greater than 0");
    period_ = value;
  }

private:
  std::vector<double> a_;
  std::vector<int> phi_;
  std::vector<double> min_value_;
  date::days period_;
};

// Class to hold seasonality settings, combining RainfallSettings and SimpleSettings
class SeasonalitySettings {
public:
  // Getters
  [[nodiscard]] bool get_enable() const {
    return enable_;
  }

  // Setters
  void set_enable(bool value) {
    enable_ = value;
  }

  [[nodiscard]] const std::string &get_mode() const {
    return mode_;
  }

  void set_mode(const std::string &value) {
    mode_ = value;
  }

  [[nodiscard]] const RainfallSettings &get_rainfall() const {
    return rainfall_;
  }

  void set_rainfall(const RainfallSettings &value) {
    rainfall_ = value;
  }

  [[nodiscard]] const SimpleSettings &get_simple() const {
    return simple_;
  }

  void set_simple(const SimpleSettings &value) {
    simple_ = value;
  }

private:
  bool enable_;
  std::string mode_;
  RainfallSettings rainfall_;
  SimpleSettings simple_;
};

namespace YAML {
// Convert specialization for RainfallSettings
template <>
struct YAML::convert<RainfallSettings> {
  static Node encode(const RainfallSettings &rhs) {
    Node node;
    node["filename"] = rhs.get_filename();
    node["period"] = static_cast<int>(rhs.get_period().count()); // Storing period as an int (days)
    return node;
  }

  static bool decode(const Node &node, RainfallSettings &rhs) {
    if (!node["filename"] || !node["period"]) {
      throw std::runtime_error("Missing fields in RainfallSettings");
    }
    rhs.set_filename(node["filename"].as<std::string>());
    rhs.set_period(date::days(node["period"].as<int>()));
    return true;
  }
};

// Convert specialization for SimpleSettings
template <>
struct YAML::convert<SimpleSettings> {
  static Node encode(const SimpleSettings &rhs) {
    Node node;
    node["a"] = rhs.get_a();
    node["phi"] = rhs.get_phi();
    node["min_value"] = rhs.get_min_value();
    node["period"] = static_cast<int>(rhs.get_period().count());
    return node;
  }

  static bool decode(const Node &node, SimpleSettings &rhs) {
    if (!node["a"] || !node["phi"] || !node["min_value"] || !node["period"]) {
      throw std::runtime_error("Missing fields in SimpleSettings");
    }
    rhs.set_a(node["a"].as<std::vector<double>>());
    rhs.set_phi(node["phi"].as<std::vector<int>>());
    rhs.set_min_value(node["min_value"].as<std::vector<double>>());
    rhs.set_period(date::days(node["period"].as<int>()));
    return true;
  }
};

// Convert specialization for SeasonalitySettings
template <>
struct YAML::convert<SeasonalitySettings> {
  static Node encode(const SeasonalitySettings &rhs) {
    Node node;
    node["enable"] = rhs.get_enable();
    node["mode"] = rhs.get_mode();
    node["rainfall"] = rhs.get_rainfall();
    node["simple"] = rhs.get_simple();
    return node;
  }

  static bool decode(const Node &node, SeasonalitySettings &rhs) {
    if (!node["enable"] || !node["mode"] || !node["rainfall"] || !node["simple"]) {
      throw std::runtime_error("Missing fields in SeasonalitySettings");
    }

    rhs.set_enable(node["enable"].as<bool>());
    rhs.set_mode(node["mode"].as<std::string>());
    rhs.set_rainfall(node["rainfall"].as<RainfallSettings>());
    rhs.set_simple(node["simple"].as<SimpleSettings>());
    return true;
  }
};
}
#endif // SEASONALITYSETTINGS_H
