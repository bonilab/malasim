#ifndef SEASONALITYSETTINGS_H
#define SEASONALITYSETTINGS_H

#include <Utils/Helpers/TimeHelpers.h>

#include <string>
#include <vector>
#include <spdlog/spdlog.h>

#include <fstream>

#include "IConfigData.h"
#include "Spatial/GIS/AscFile.h"
#include "Spatial/GIS/SpatialData.h"
#include "Simulation/Model.h"
#include "Utils/Constants.h"
#include "date/date.h"
#include "Utils/TypeDef.hxx"

// Class to hold seasonality settings, combining RainfallSettings and SimpleSettings
class SeasonalitySettings : public IConfigData{
public:
  class ISeasonalInfo {
  public:
    virtual ~ISeasonalInfo() = default;

    // Return the seasonal factor for the given day and location, based upon the
    // loaded configuration.
    virtual double get_seasonal_factor(const date::sys_days &today,
                                       const int &location) {
      throw std::runtime_error("Runtime call to virtual function");
    }
  };

  // Class to hold the 'equation' settings
  class SeasonalEquation : ISeasonalInfo{
  public:
    SeasonalEquation() = default;
    void build(int number_of_locations) {
      // Before doing anything, check to see if there is a raster
      if (raster_) {
        set_from_raster();
      }
      // Warn the user if enough nodes were not provided
      if (A_.size() > 1
          && A_.size() < number_of_locations) {
        spdlog::info(
            "Only {} seasonal  equation settings provided, but {} are needed for "
            "all locations",
            A_.size(), number_of_locations);
      }

      // Set the values from the array and return
      for (auto i = 0ul; i < number_of_locations; i++) {
        auto input_loc =
            A_.size() < number_of_locations ? 0 : i;
        set_seasonal_period(input_loc);
      }
    }

    double get_seasonal_factor(const date::sys_days &today,
                                                 const int &location) override{
      // Note what day of the year it is
      int day = TimeHelpers::day_of_year(today);

      // Seasonal factor is determined by the algorithm:
      //
      // multiplier = base + (a * sin⁺(b * pi * (t - phi) / 365))
      auto multiplier = A_[location]
                        * sin(B_[location] * M_PI * (day - phi_[location])
                              / static_cast<double>(Constants::DAYS_IN_YEAR));
      multiplier = (multiplier < 0) ? 0 : multiplier;
      multiplier += base_[location];

      // Return the multiplier
      return multiplier;
    }

    // Set the values based upon the contents of a raster file.
    void set_from_raster() {
      // Get the raster data and make sure it is valid
      AscFile* raster = SpatialData::get_instance().get_raster(
          SpatialData::SpatialFileType::Ecoclimatic);
      if (raster == nullptr) {
        throw std::invalid_argument(
            "Seasonal equation  raster flag set without eco-climatic raster "
            "loaded.");
      }

      // Prepare to run
      spdlog::info("Setting seasonal equation using raster data.");

      // Load the values based upon the raster data
      auto size = A_.size();
      int index = 0;
      for (int row = 0; row < raster->NROWS; row++) {
        for (int col = 0; col < raster->NCOLS; col++) {
          // Pass if we have no data here
          if (raster->data[row][col] == raster->NODATA_VALUE) { continue; }
          //
          // // Verify the index
          // int index = static_cast<int>(raster->data[row][col]);
          // spdlog::info("Setting seasonal equation for location: {}", index);
          // if (index < 0) {
          //   throw std::out_of_range(fmt::format(
          //       "Raster value at row: {}, col: {} is less than zero.", row, col));
          // }
          // if (index > (size - 1)) {
          //   throw std::out_of_range(fmt::format(
          //       "Raster value at row: {}, col: {} exceeds bounds of {}.", row, col,
          //       size));
          // }

          // Set the seasonal period
          set_seasonal_period(index);
          index++;
        }
      }
    }

    // Set the period for a single location given the index
    void set_seasonal_period(unsigned long index) {
      base_.push_back(raster_base_[index]);
      A_.push_back(raster_A_[index]);
      B_.push_back(raster_B_[index]);
      phi_.push_back(raster_phi_[index]);

      // Update the reference values as well
      reference_base_.push_back(raster_base_[index]);
      reference_A_.push_back(raster_A_[index]);
      reference_B_.push_back(raster_B_[index]);
      reference_phi_.push_back(raster_phi_[index]);
    }

    // Update the seasonality of the equation from the current to the new one.
    void update_seasonality(int from, int to) {
      for (auto ndx = 0; ndx < base_.size(); ndx++) {
        if (base_[ndx] == reference_base_[from] && A_[ndx] == reference_A_[from]
            && B_[ndx] == reference_B_[from] && phi_[ndx] == reference_phi_[from]) {
          base_[ndx] = reference_base_[to];
          A_[ndx] = reference_A_[to];
          B_[ndx] = reference_B_[to];
          phi_[ndx] = reference_phi_[to];
        }
      }
    }

    [[nodiscard]] bool get_raster() const {
      return raster_;
    }

    void set_raster(const bool value) {
      raster_ = value;
    }

    bool raster_;
    std::string ecoclimatic_raster_;
    std::vector<double> raster_base_;
    std::vector<double> raster_A_;
    std::vector<double> raster_B_;
    std::vector<int> raster_phi_;

    std::vector<double> base_;
    std::vector<double> A_;
    std::vector<double> B_;
    std::vector<int> phi_;

    std::vector<double> reference_base_;
    std::vector<double> reference_A_;
    std::vector<double> reference_B_;
    std::vector<int> reference_phi_;
  };

  class SeasonalRainfall : ISeasonalInfo{
  public:
    SeasonalRainfall() = default;
    void build() {
      // Read the adjustments, verify that the period matches the adjustment count
      read(filename_);
      if (adjustments_.size() != period_) {
        throw std::invalid_argument(fmt::format(
            "The number of rainfall data points ({}) should match the period ({}).",
            adjustments_.size(), period_));
      }
    }

    double get_seasonal_factor(const date::sys_days &today,
                                                 const int &location) override {
      // Get the current day of the year
      auto doy = TimeHelpers::day_of_year(today);

      // Shift the day of year to be one index, shift two on the leap day
      doy = (doy == 366) ? doy - 2 : doy - 1;

      // If it's a leap day, return the last day a second time, otherwise just
      // return the value for the day
      return adjustments_[doy];
    }

    void read(const std::string &filename) {
      // Check the validity of the file
      std::ifstream in(filename);
      if (!in.good()) {
        throw std::runtime_error("Error opening the rainfall data file: "
                                 + filename);
      }
      if (in.peek() == std::ifstream::traits_type ::eof()) {
        throw std::runtime_error("EOF encountered at start of rainfall data file: "
                                 + filename);
      }

      // Read and store the data
      double data = 0.0;
      while (in >> data) {
        if (data > 1.0) {
          throw std::runtime_error(
              fmt::format("Rain fall factor exceeded 1.0: {0}", data));
        }
        if (data < 0.0) {
          throw std::runtime_error(
              fmt::format("Rain fall factor less than zero: {0}", data));
        }
        adjustments_.emplace_back(data);
      }
    }

    [[nodiscard]] std::string get_filename() const {
      return filename_;
    }

    void set_filename(const std::string &value) {
      filename_ = value;
    }

    [[nodiscard]] int get_period() const {
      return period_;
    }

    void set_period(int value) {
      period_ = value;
    }

    std::string filename_;
    std::vector<double> adjustments_;
    int period_;
  };

  // Getters
  [[nodiscard]] bool get_enable() const {
    return enable_;
  }

  // Setters
  void set_enable(bool value) {
    enable_ = value;
  }

  [[nodiscard]] std::string get_mode() const {
    return mode_;
  }

  void set_mode(std::string value) {
    mode_ = value;
  }

  [[nodiscard]]  SeasonalRainfall get_seasonal_rainfall() const {
    return seasonal_rainfall_;
  }

  void set_seasonal_rainfall(const SeasonalRainfall &value) {
    seasonal_rainfall_ = value;
  }

  [[nodiscard]]  SeasonalEquation get_seasonal_equation() const {
    return seasonal_equation_;
  }

  void set_seasonal_equation(const SeasonalEquation &value) {
    seasonal_equation_ = value;
  }

  [[nodiscard]] double get_seasonal_factor(const date::sys_days &today,
                                             const int &location) {
    if(enable_) {
      if(mode_=="equation") {
        return get_seasonal_equation().get_seasonal_factor(today, location);
      }
      if(mode_=="rainfall") {
        return get_seasonal_rainfall().get_seasonal_factor(today, location);
      }
    }
    return 1.0;
  }

  void process_config() override {};

  void process_config_using_number_of_locations(int number_of_locations) {
    spdlog::info("Processing SeasonalitySettings");
    if(enable_) {
      spdlog::info("Seasonality enabled");
      if(mode_=="equation") {
        spdlog::info("Equation based");
        // process equation based
        seasonal_equation_.build(number_of_locations);
      }
      else if(mode_=="rainfall") {
        spdlog::info("Rainfall based");
        // process rainfall based
        seasonal_rainfall_.build();
      }
      else {
        throw std::runtime_error("Unknown mode in 'seasonality_settings'.");
      }
    }
    else {
      spdlog::info("Seasonality disabled, using default value of 1.0");
    }
  }

private:
  bool enable_ = false;
  std::string mode_;
  SeasonalEquation seasonal_equation_;
  SeasonalRainfall seasonal_rainfall_;
public:
  DoubleVector A;
  DoubleVector B;
  DoubleVector C;
  DoubleVector phi;
  DoubleVector min_value;
};

namespace YAML {
//convert specialization for SeasonalitySettings::SeasonalEquation
template <>
struct convert<SeasonalitySettings::SeasonalEquation> {
  static Node encode(const SeasonalitySettings::SeasonalEquation &rhs) {
    Node node;
    node["raster"] = rhs.raster_;
    node["base"] = rhs.raster_base_;
    node["a"] = rhs.raster_A_;
    node["b"] = rhs.raster_B_;
    node["phi"] = rhs.raster_phi_;
    return node;
  }

  static bool decode(const Node &node, SeasonalitySettings::SeasonalEquation &rhs) {
    if (!node["base"] || !node["a"] || !node["b"] || !node["phi"] || !node["raster"]) {
      throw std::runtime_error("Missing fields in SeasonalEquation");
    }

    rhs.raster_ = node["raster"].as<bool>();
    rhs.raster_base_ = node["base"].as<std::vector<double>>();
    rhs.raster_A_ = node["a"].as<std::vector<double>>();
    rhs.raster_B_ = node["b"].as<std::vector<double>>();
    rhs.raster_phi_ = node["phi"].as<std::vector<int>>();
    rhs.A_ = rhs.raster_A_;
    rhs.B_ = rhs.raster_B_;
    rhs.base_ = rhs.raster_base_;
    rhs.phi_ = rhs.raster_phi_;
    rhs.reference_A_ = rhs.raster_A_;
    rhs.reference_B_ = rhs.raster_B_;
    rhs.reference_base_ = rhs.raster_base_;
    rhs.reference_phi_ = rhs.raster_phi_;
    return true;
  }
};

// Convert specialization for SeasonalitySettings::SeasonalRainfall
template <>
struct convert<SeasonalitySettings::SeasonalRainfall> {
  static Node encode(const SeasonalitySettings::SeasonalRainfall &rhs) {
    Node node;
    node["filename"] = rhs.filename_;
    node["period"] = rhs.period_;
    return node;
  }

  static bool decode(const Node &node, SeasonalitySettings::SeasonalRainfall &rhs) {
    if (!node["filename"] || !node["period"]) {
      throw std::runtime_error("Missing fields in SeasonalRainfall");
    }

    rhs.filename_ = node["filename"].as<std::string>();
    rhs.period_ = node["period"].as<int>();
    return true;
  }
};


// Convert specialization for SeasonalitySettings
template <>
struct convert<SeasonalitySettings> {
  static Node encode(const SeasonalitySettings &rhs) {
    Node node;
    node["enable"] = rhs.get_enable();
    node["mode"] = rhs.get_mode();
    node["equation"]["base"] = rhs.get_seasonal_equation().base_;
    node["equation"]["a"] = rhs.get_seasonal_equation().A_;
    node["equation"]["b"] = rhs.get_seasonal_equation().B_;
    node["equation"]["phi"] = rhs.get_seasonal_equation().phi_;
    node["rainfall"]["filename"] = rhs.get_seasonal_rainfall().filename_;
    node["rainfall"]["period"] = rhs.get_seasonal_rainfall().period_;
    return node;
  }

  static bool decode(const Node &node, SeasonalitySettings &rhs) {
    if (!node["enable"] || !node["mode"] || !node["rainfall"] || !node["equation"]) {
      throw std::runtime_error("Missing fields in SeasonalitySettings");
    }

    rhs.set_enable(node["enable"].as<bool>());
    rhs.set_mode(node["mode"].as<std::string>());
    if(node["equation"]) {
      rhs.set_seasonal_equation(node["equation"].as<SeasonalitySettings::SeasonalEquation>());
    }
    if(node["rainfall"]) {
      rhs.set_seasonal_rainfall(node["rainfall"].as<SeasonalitySettings::SeasonalRainfall>());
    }
    return true;
  }
};
}
#endif // SEASONALITYSETTINGS_H
