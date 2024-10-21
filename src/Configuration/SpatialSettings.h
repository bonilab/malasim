#ifndef SPATIALSETTINGS_H
#define SPATIALSETTINGS_H
#include <string>
#include <vector>
#include <array>
#include <date/date.h>
#include <yaml-cpp/yaml.h>
#include "SpatialSettings.h"
#include <stdexcept>

// Class for SpatialSettings
class SpatialSettings {
public:
    // Class for GridBased settings
    class GridBased {
        public:
            // Getters and Setters for population_raster
            [[nodiscard]] const std::string &get_population_raster() const {
                return population_raster_;
            }
            void set_population_raster(const std::string &value) {
                population_raster_ = value;
            }

            // Getters and Setters for district_raster
            [[nodiscard]] const std::string &get_district_raster() const {
                return district_raster_;
            }
            void set_district_raster(const std::string &value) {
                district_raster_ = value;
            }

            // Getters and Setters for p_treatment_under_5_raster
            [[nodiscard]] const std::string &get_p_treatment_under_5_raster() const {
                return p_treatment_under_5_raster_;
            }
            void set_p_treatment_under_5_raster(const std::string &value) {
                p_treatment_under_5_raster_ = value;
            }

            // Getters and Setters for p_treatment_over_5_raster
            [[nodiscard]] const std::string &get_p_treatment_over_5_raster() const {
                return p_treatment_over_5_raster_;
            }
            void set_p_treatment_over_5_raster(const std::string &value) {
                p_treatment_over_5_raster_ = value;
            }

            // Getters and Setters for p_treatment_under_5_by_location
            [[nodiscard]] const std::vector<double> &get_p_treatment_under_5_by_location() const {
                return p_treatment_under_5_by_location_;
            }
            void set_p_treatment_under_5_by_location(const std::vector<double> &value) {
                p_treatment_under_5_by_location_ = value;
            }

            // Getters and Setters for p_treatment_over_5_by_location
            [[nodiscard]] const std::vector<double> &get_p_treatment_over_5_by_location() const {
                return p_treatment_over_5_by_location_;
            }
            void set_p_treatment_over_5_by_location(const std::vector<double> &value) {
                p_treatment_over_5_by_location_ = value;
            }

            // Getters and Setters for beta_raster
            [[nodiscard]] const std::string &get_beta_raster() const {
                return beta_raster_;
            }
            void set_beta_raster(const std::string &value) {
                beta_raster_ = value;
            }

            // Getters and Setters for beta_by_location
            [[nodiscard]] const std::vector<double> &get_beta_by_location() const {
                return beta_by_location_;
            }
            void set_beta_by_location(const std::vector<double> &value) {
                beta_by_location_ = value;
            }

            // Getters and Setters for cell_size
            [[nodiscard]] double get_cell_size() const {
                return cell_size_;
            }
            void set_cell_size(double value) {
                if (value <= 0)
                    throw std::invalid_argument("cell_size must be greater than 0");
                cell_size_ = value;
            }

            // Getters and Setters for age_distribution_by_location
            [[nodiscard]] const std::vector<std::vector<double>> &get_age_distribution_by_location() const {
                return age_distribution_by_location_;
            }
            void set_age_distribution_by_location(const std::vector<std::vector<double>> &value) {
                age_distribution_by_location_ = value;
            }

        private:
            std::string population_raster_;
            std::string district_raster_;
            std::string p_treatment_under_5_raster_;
            std::string p_treatment_over_5_raster_;
            std::vector<double> p_treatment_under_5_by_location_;
            std::vector<double> p_treatment_over_5_by_location_;
            std::string beta_raster_;
            std::vector<double> beta_by_location_;
            double cell_size_;
            std::vector<std::vector<double>> age_distribution_by_location_;
        };

        // Class for LocationInfo
        class LocationInfo {
        public:
            // Getters and Setters for id
            [[nodiscard]] int get_id() const {
                return id_;
            }
            void set_id(int value) {
                id_ = value;
            }

            // Getters and Setters for latitude
            [[nodiscard]] double get_latitude() const {
                return latitude_;
            }
            void set_latitude(double value) {
                latitude_ = value;
            }

            // Getters and Setters for longitude
            [[nodiscard]] double get_longitude() const {
                return longitude_;
            }
            void set_longitude(double value) {
                longitude_ = value;
            }

        private:
            int id_;
            double latitude_;
            double longitude_;
        };

        // Class for LocationBased settings
        class LocationBased {
        public:
            // Getters and Setters for location_info
            [[nodiscard]] const std::vector<LocationInfo> &get_location_info() const {
                return location_info_;
            }
            void set_location_info(const std::vector<LocationInfo> &value) {
                location_info_ = value;
            }

            // Getters and Setters for age_distribution_by_location
            [[nodiscard]] const std::vector<std::vector<double>> &get_age_distribution_by_location() const {
                return age_distribution_by_location_;
            }
            void set_age_distribution_by_location(const std::vector<std::vector<double>> &value) {
                age_distribution_by_location_ = value;
            }

            // Getters and Setters for p_treatment_under_5_by_location
            [[nodiscard]] const std::vector<double> &get_p_treatment_under_5_by_location() const {
                return p_treatment_under_5_by_location_;
            }
            void set_p_treatment_under_5_by_location(const std::vector<double> &value) {
                p_treatment_under_5_by_location_ = value;
            }

            // Getters and Setters for p_treatment_over_5_by_location
            [[nodiscard]] const std::vector<double> &get_p_treatment_over_5_by_location() const {
                return p_treatment_over_5_by_location_;
            }
            void set_p_treatment_over_5_by_location(const std::vector<double> &value) {
                p_treatment_over_5_by_location_ = value;
            }

            // Getters and Setters for beta_by_location
            [[nodiscard]] const std::vector<double> &get_beta_by_location() const {
                return beta_by_location_;
            }
            void set_beta_by_location(const std::vector<double> &value) {
                beta_by_location_ = value;
            }

            // Getters and Setters for population_size_by_location
            [[nodiscard]] const std::vector<int> &get_population_size_by_location() const {
                return population_size_by_location_;
            }
            void set_population_size_by_location(const std::vector<int> &value) {
                population_size_by_location_ = value;
            }

        private:
            std::vector<LocationInfo> location_info_;
            std::vector<std::vector<double>> age_distribution_by_location_;
            std::vector<double> p_treatment_under_5_by_location_;
            std::vector<double> p_treatment_over_5_by_location_;
            std::vector<double> beta_by_location_;
            std::vector<int> population_size_by_location_;
        };
public:
    // Getters and Setters for mode
    [[nodiscard]] const std::string &get_mode() const {
        return mode_;
    }
    void set_mode(const std::string &value) {
        mode_ = value;
    }

    // Getters and Setters for grid_based
    [[nodiscard]] const GridBased &get_grid_based() const {
        return grid_based_;
    }
    void set_grid_based(const GridBased &value) {
        grid_based_ = value;
    }

    // Getters and Setters for location_based
    [[nodiscard]] const LocationBased &get_location_based() const {
        return location_based_;
    }
    void set_location_based(const LocationBased &value) {
        location_based_ = value;
    }

private:
    std::string mode_;  // "grid_based" or "location_based"
    GridBased grid_based_;
    LocationBased location_based_;
};

namespace YAML {
// Conversion for GridBased class
template <>
struct convert<SpatialSettings::GridBased> {
    static Node encode(const SpatialSettings::GridBased &rhs) {
        Node node;
        node["population_raster"] = rhs.get_population_raster();
        node["district_raster"] = rhs.get_district_raster();
        node["p_treatment_under_5_raster"] = rhs.get_p_treatment_under_5_raster();
        node["p_treatment_over_5_raster"] = rhs.get_p_treatment_over_5_raster();
        node["p_treatment_under_5_by_location"] = rhs.get_p_treatment_under_5_by_location();
        node["p_treatment_over_5_by_location"] = rhs.get_p_treatment_over_5_by_location();
        node["beta_raster"] = rhs.get_beta_raster();
        node["beta_by_location"] = rhs.get_beta_by_location();
        node["cell_size"] = rhs.get_cell_size();
        node["age_distribution_by_location"] = rhs.get_age_distribution_by_location();
        return node;
    }

    static bool decode(const Node &node, SpatialSettings::GridBased &rhs) {
        if (!node["population_raster"] || !node["district_raster"] || !node["p_treatment_under_5_raster"] ||
            !node["p_treatment_over_5_raster"] || !node["beta_raster"] || !node["cell_size"]) {
            throw std::runtime_error("Missing required fields in grid-based settings.");
        }

        rhs.set_population_raster(node["population_raster"].as<std::string>());
        rhs.set_district_raster(node["district_raster"].as<std::string>());
        rhs.set_p_treatment_under_5_raster(node["p_treatment_under_5_raster"].as<std::string>());
        rhs.set_p_treatment_over_5_raster(node["p_treatment_over_5_raster"].as<std::string>());
        rhs.set_p_treatment_under_5_by_location(node["p_treatment_under_5_by_location"].as<std::vector<double>>());
        rhs.set_p_treatment_over_5_by_location(node["p_treatment_over_5_by_location"].as<std::vector<double>>());
        rhs.set_beta_raster(node["beta_raster"].as<std::string>());
        rhs.set_beta_by_location(node["beta_by_location"].as<std::vector<double>>());
        rhs.set_cell_size(node["cell_size"].as<double>());
        rhs.set_age_distribution_by_location(node["age_distribution_by_location"].as<std::vector<std::vector<double>>>());
        return true;
    }
};

// Conversion for LocationInfo class
template <>
struct convert<SpatialSettings::LocationInfo> {
    static Node encode(const SpatialSettings::LocationInfo &rhs) {
        Node node;
        node.push_back(rhs.get_id());
        node.push_back(rhs.get_latitude());
        node.push_back(rhs.get_longitude());
        return node;
    }

    static bool decode(const Node &node, SpatialSettings::LocationInfo &rhs) {
        if (!node.IsSequence() || node.size() != 3) {
            throw std::runtime_error("Invalid location info.");
        }
        rhs.set_id(node[0].as<int>());
        rhs.set_latitude(node[1].as<double>());
        rhs.set_longitude(node[2].as<double>());
        return true;
    }
};

// Conversion for LocationBased class
template <>
struct convert<SpatialSettings::LocationBased> {
    static Node encode(const SpatialSettings::LocationBased &rhs) {
        Node node;
        node["location_info"] = rhs.get_location_info();
        node["age_distribution_by_location"] = rhs.get_age_distribution_by_location();
        node["p_treatment_under_5_by_location"] = rhs.get_p_treatment_under_5_by_location();
        node["p_treatment_over_5_by_location"] = rhs.get_p_treatment_over_5_by_location();
        node["beta_by_location"] = rhs.get_beta_by_location();
        node["population_size_by_location"] = rhs.get_population_size_by_location();
        return node;
    }

    static bool decode(const Node &node, SpatialSettings::LocationBased &rhs) {
        if (!node["location_info"] || !node["age_distribution_by_location"] ||
            !node["p_treatment_under_5_by_location"] || !node["p_treatment_over_5_by_location"] ||
            !node["beta_by_location"] || !node["population_size_by_location"]) {
            throw std::runtime_error("Missing required fields in location-based settings.");
        }

        rhs.set_location_info(node["location_info"].as<std::vector<SpatialSettings::LocationInfo>>());
        rhs.set_age_distribution_by_location(node["age_distribution_by_location"].as<std::vector<std::vector<double>>>());
        rhs.set_p_treatment_under_5_by_location(node["p_treatment_under_5_by_location"].as<std::vector<double>>());
        rhs.set_p_treatment_over_5_by_location(node["p_treatment_over_5_by_location"].as<std::vector<double>>());
        rhs.set_beta_by_location(node["beta_by_location"].as<std::vector<double>>());
        rhs.set_population_size_by_location(node["population_size_by_location"].as<std::vector<int>>());
        return true;
    }
};

// Conversion for SpatialSettings class
template <>
struct convert<SpatialSettings> {
    static Node encode(const SpatialSettings &rhs) {
        Node node;
        node["mode"] = rhs.get_mode();
        node["grid_based"] = rhs.get_grid_based();
        node["location_based"] = rhs.get_location_based();
        return node;
    }

    static bool decode(const Node &node, SpatialSettings &rhs) {
        if (!node["mode"]) {
            throw std::runtime_error("Missing 'mode' field.");
        }

        rhs.set_mode(node["mode"].as<std::string>());

        if (rhs.get_mode() == "grid_based") {
            if (!node["grid_based"]) {
                throw std::runtime_error("Missing 'grid_based' settings.");
            }
            rhs.set_grid_based(node["grid_based"].as<SpatialSettings::GridBased>());
        } else if (rhs.get_mode() == "location_based") {
            if (!node["location_based"]) {
                throw std::runtime_error("Missing 'location_based' settings.");
            }
            rhs.set_location_based(node["location_based"].as<SpatialSettings::LocationBased>());
        } else {
            throw std::runtime_error("Unknown mode in 'spatial_settings'.");
        }

        return true;
    }
};
}

#endif //SPATIALSETTINGS_H
