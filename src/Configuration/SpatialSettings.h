#ifndef SPATIALSETTINGS_H
#define SPATIALSETTINGS_H
#include <string>
#include <vector>
#include <date/date.h>
#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <spdlog/spdlog.h>

#include "IConfigClass.h"
#include "Spatial/Location/Location.h"
#include "Spatial/GIS/SpatialData.h"

// Class for SpatialSettings
class SpatialSettings : IConfigClass{
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

            // Getters and Setters for beta_raster
            [[nodiscard]] const std::string &get_beta_raster() const {
                return beta_raster_;
            }
            void set_beta_raster(const std::string &value) {
                beta_raster_ = value;
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

            void set_number_of_location(int value) {
                number_of_location_ = value;
            }

            [[nodiscard]] int get_number_of_location() const {
                return number_of_location_;
            }

            void set_locations(const std::vector<Spatial::Location> &value) {
                locations_ = value;
            }

            [[nodiscard]] const std::vector<Spatial::Location> &get_locations() const {
                return locations_;
            }

            [[nodiscard]] const std::string &get_ecoclimatic_raster() const {
                return ecoclimatic_raster_;
            }

            void set_ecoclimatic_raster(const std::string &value) {
                ecoclimatic_raster_ = value;
            }

        private:
            std::vector<Spatial::Location> locations_;
            std::string population_raster_;
            std::string district_raster_;
            std::string p_treatment_under_5_raster_;
            std::string p_treatment_over_5_raster_;
            std::string beta_raster_;
            std::string ecoclimatic_raster_;
            double cell_size_ = 0.0;
            std::vector<std::vector<double>> age_distribution_by_location_;
            int number_of_location_ = 0;
        };

    // Class for LocationBased settings
    class LocationBased {
    public:
        // Getters and Setters for location_info
        [[nodiscard]] const std::vector<Spatial::Location> &get_location_info() const {
            return location_info_;
        }
        void set_location_info(const std::vector<Spatial::Location> &value) {
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

        void set_number_of_locations(int value) {
            number_of_locations_ = value;
        }

        [[nodiscard]] int get_number_of_locations() const {
            return number_of_locations_;
        }

    private:
        std::vector<Spatial::Location> location_info_;
        std::vector<std::vector<double>> age_distribution_by_location_;
        std::vector<double> p_treatment_under_5_by_location_;
        std::vector<double> p_treatment_over_5_by_location_;
        std::vector<double> beta_by_location_;
        std::vector<int> population_size_by_location_;
        int number_of_locations_ = 0;
    };

    void set_age_distribution_by_location(const std::vector<std::vector<double>> &value) {
        age_distribution_by_location_ = value;
    }

    [[nodiscard]] const std::vector<std::vector<double>> &get_age_distribution_by_location() const {
        return age_distribution_by_location_;
    }

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

    void set_locations(const std::vector<Spatial::Location> &value) {
      locations_ = value;
    }

    [[nodiscard]] std::vector<Spatial::Location> &get_locations() {
      return locations_;
    }

    void set_spatial_distance_matrix(const std::vector<std::vector<double>> &value) {
      spatial_distance_matrix_ = value;
    }

    [[nodiscard]] std::vector<std::vector<double>> &get_spatial_distance_matrix() {
      return spatial_distance_matrix_;
    }

    void set_number_of_locations(int value) {
      number_of_location_ = value;
    }

    [[nodiscard]] int get_number_of_locations() const {
      return number_of_location_;
    }

    void set_node(const YAML::Node &value) {
      node_ = value;
    }

    [[nodiscard]] YAML::Node &get_node() {
      return node_;
    }

    void process_config() override {
      spdlog::info("Processing SpatialSettings");
        int number_of_location = 0;
        if(mode_=="grid_based") {
          spdlog::info("Grid based");
          /* Number of location, locations and spatial matrix are calculated in SpatialData class */
          SpatialData::get_instance().parse(node_);
        }
        else if(mode_=="location_based") {
          spdlog::info("Location based");
          // process location_based
          location_based_.set_number_of_locations(static_cast<int>(location_based_.get_population_size_by_location().size()));
          number_of_location = location_based_.get_number_of_locations();
          spatial_distance_matrix_ = std::vector<std::vector<double>>(static_cast<unsigned long long int>(number_of_location));
          for (auto from_location = 0ul; from_location < number_of_location; from_location++) {
              spatial_distance_matrix_[from_location].resize(static_cast<unsigned long long int>(number_of_location));
              for (auto to_location = 0ul; to_location < number_of_location; to_location++) {
                  spatial_distance_matrix_[from_location][to_location] = Spatial::Coordinate::calculate_distance_in_km(
                      *this->get_location_based().get_location_info()[from_location].coordinate,
                      *this->get_location_based().get_location_info()[to_location].coordinate);
              }
          }
          locations_ = location_based_.get_location_info();
        }
        else {
          throw std::runtime_error("Unknown mode in 'spatial_settings'.");
        }
    }

private:
    std::string mode_;  // "grid_based" or "location_based"
    GridBased grid_based_;
    LocationBased location_based_;
    std::vector<std::vector<double>> spatial_distance_matrix_;
    std::vector<Spatial::Location> locations_;
    std::vector<std::vector<double>> age_distribution_by_location_;
    int number_of_location_ = 0;
    YAML::Node node_;
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
        node["beta_raster"] = rhs.get_beta_raster();
        node["ecoclimatic_raster"] = rhs.get_ecoclimatic_raster();
        node["cell_size"] = rhs.get_cell_size();
        node["age_distribution_by_location"] = rhs.get_age_distribution_by_location();
        return node;
    }

    static bool decode(const Node &node, SpatialSettings::GridBased &rhs) {
        if (!node["population_raster"] || !node["district_raster"] || !node["p_treatment_under_5_raster"] ||
            !node["p_treatment_over_5_raster"] || !node["beta_raster"] || !node["cell_size"] ||
            !node["age_distribution_by_location"]) {
            throw std::runtime_error("Missing required fields in grid-based settings.");
        }

        rhs.set_population_raster(node["population_raster"].as<std::string>());
        rhs.set_district_raster(node["district_raster"].as<std::string>());
        rhs.set_p_treatment_under_5_raster(node["p_treatment_under_5_raster"].as<std::string>());
        rhs.set_p_treatment_over_5_raster(node["p_treatment_over_5_raster"].as<std::string>());
        rhs.set_beta_raster(node["beta_raster"].as<std::string>());
        rhs.set_cell_size(node["cell_size"].as<double>());
        if(node["ecoclimatic_raster"]) {
            rhs.set_ecoclimatic_raster(node["ecoclimatic_raster"].as<std::string>());
        }

        /* use one age distribution for all locations */
        rhs.set_age_distribution_by_location(node["age_distribution_by_location"].as<std::vector<std::vector<double>>>());
        return true;
    }
};

// Conversion for Location class
template <>
struct convert<Spatial::Location> {
    static Node encode(const Spatial::Location &rhs) {
        Node node;
        node.push_back(rhs.id);
        node.push_back(rhs.coordinate->latitude);
        node.push_back(rhs.coordinate->longitude);
        return node;
    }

    static bool decode(const Node &node, Spatial::Location &rhs) {
        if (!node.IsSequence() || node.size() != 3) {
            throw std::runtime_error("Invalid location info.");
        }
        rhs.id = node[0].as<int>();
        rhs.coordinate = std::make_unique<Spatial::Coordinate>(node[1].as<float>(), node[2].as<float>());
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

        rhs.set_location_info(node["location_info"].as<std::vector<Spatial::Location>>());
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

        /* Here the configuration for spatial settings are parsed based on mode
         * The GridBased and LocationBased classes are used to store the configuration
         * for validation. The actual processing of the configuration is done in the
         * SpatialData class for GridBased. For LocationBased, the processing is done
         * in the process_config method of SpatialSettings class.
         */
        if (rhs.get_mode() == "grid_based") {
            if (!node["grid_based"]) {
                throw std::runtime_error("Missing 'grid_based' settings.");
            }
            rhs.set_grid_based(node["grid_based"].as<SpatialSettings::GridBased>());
            rhs.set_node(node["grid_based"]);
        } else if (rhs.get_mode() == "location_based") {
            if (!node["location_based"]) {
                throw std::runtime_error("Missing 'location_based' settings.");
            }
            rhs.set_location_based(node["location_based"].as<SpatialSettings::LocationBased>());
            rhs.set_node(node["location_based"]);
        } else {
            throw std::runtime_error("Unknown mode in 'spatial_settings'.");
        }

        return true;
    }
};
}

#endif //SPATIALSETTINGS_H
