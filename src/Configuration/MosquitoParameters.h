#ifndef MOSQUITOPARAMETERS_H
#define MOSQUITOPARAMETERS_H
#include <string>
#include <vector>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include "IConfigData.h"
#include "Spatial/GIS/AscFile.h"
#include "Spatial/GIS/SpatialData.h"
#include "Spatial/Location/Location.h"
#include "spdlog/spdlog.h"

class MosquitoParameters : public IConfigData {
public:
    // Inner class: GridBased
    class GridBased {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_interrupted_feeding_rate_raster() const { return interrupted_feeding_rate_raster_; }
        void set_interrupted_feeding_rate_raster(const std::string& value) { interrupted_feeding_rate_raster_ = value; }

        [[nodiscard]] const std::string& get_prmc_size_raster() const { return prmc_size_raster_; }
        void set_prmc_size_raster(const std::string& value) { prmc_size_raster_ = value; }

    private:
        std::string interrupted_feeding_rate_raster_;
        std::string prmc_size_raster_;
    };

    // Inner class: LocationBased
    class LocationBased {
    public:
        // Getters and Setters
        [[nodiscard]] const std::vector<double>& get_interrupted_feeding_rate() const { return interrupted_feeding_rate_; }
        void set_interrupted_feeding_rate(const std::vector<double>& value) { interrupted_feeding_rate_ = value; }

        [[nodiscard]] const std::vector<int>& get_prmc_size() const { return prmc_size_; }
        void set_prmc_size(const std::vector<int>& value) { prmc_size_ = value; }

    private:
        std::vector<double> interrupted_feeding_rate_;
        std::vector<int> prmc_size_;
    };

    // Inner class: MosquitoConfig
    class MosquitoConfig {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_mode() const { return mode_; }
        void set_mode(const std::string& value) {
            if (value != "grid_based" && value != "location_based") {
                throw std::invalid_argument("mode must be 'grid_based' or 'location_based'");
            }
            mode_ = value;
        }

        [[nodiscard]] const GridBased& get_grid_based() const { return grid_based_; }
        void set_grid_based(const GridBased& value) { grid_based_ = value; }

        [[nodiscard]] const LocationBased& get_location_based() const { return location_based_; }
        void set_location_based(const LocationBased& value) { location_based_ = value; }

    private:
        std::string mode_;
        GridBased grid_based_;
        LocationBased location_based_;
    };

    // Getters and Setters for MosquitoParameters
    [[nodiscard]] const MosquitoConfig& get_mosquito_config() const { return mosquito_config_; }
    void set_mosquito_config(const MosquitoConfig& value) { mosquito_config_ = value; }

    [[nodiscard]] bool get_within_host_induced_free_recombination() const { return within_host_induced_free_recombination_; }
    void set_within_host_induced_free_recombination(bool value) { within_host_induced_free_recombination_ = value; }

    void process_config() override {};
    void process_config_using_locations(std::vector<Spatial::Location> locations) {
      spdlog::info("Processing MosquitoParameters");
      if(get_mosquito_config().get_mode()=="grid_based") {
        AscFile* raster = SpatialData::get_instance().get_raster(
                  SpatialData::SpatialFileType::Ecoclimatic);
        if (raster == nullptr) {
          throw std::invalid_argument(
              "Mosquito  raster flag set without eco-climatic raster loaded.");
        }
        // Prepare to run
        spdlog::info("Setting seasonal equation using raster data.");
        // Load the values based upon the raster data
        auto size = locations.size();
        int index = 0;
        for (int row = 0; row < raster->NROWS; row++) {
          for (int col = 0; col < raster->NCOLS; col++) {
            // Pass if we have no data here
            if (raster->data[row][col] == raster->NODATA_VALUE) { continue; }
            // Set the seasonal period
            locations[index].mosquito_ifr = locations[index].mosquito_ifr;
            locations[index].mosquito_size = locations[index].mosquito_size;
            index++;
          }
        }
      }
      if(get_mosquito_config().get_mode()=="location_based") {
        LocationBased location_based = get_mosquito_config().get_location_based();
        if(location_based.get_interrupted_feeding_rate().size() == 1) {
          spdlog::info("1 IFR value provided, distributing equally to all locations");
          for(auto &location : locations) {
            location.mosquito_ifr = location_based.get_interrupted_feeding_rate()[0];
            location.mosquito_size = location_based.get_prmc_size()[0];
          }
        }
        if(location_based.get_interrupted_feeding_rate().size() == locations.size()) {
          spdlog::info("IFR values provided for all locations");
          for(auto i=0; i<locations.size(); i++) {
            locations[i].mosquito_ifr = location_based.get_interrupted_feeding_rate()[i];
            locations[i].mosquito_size = location_based.get_prmc_size()[i];
          }
        }
      }
    }

private:
    MosquitoConfig mosquito_config_;
    bool within_host_induced_free_recombination_ = true;

};

namespace YAML {
// MosquitoParameters::GridBased YAML conversion
template<>
struct convert<MosquitoParameters::GridBased> {
    static Node encode(const MosquitoParameters::GridBased& rhs) {
        Node node;
        node["interrupted_feeding_rate_raster"] = rhs.get_interrupted_feeding_rate_raster();
        node["prmc_size_raster"] = rhs.get_prmc_size_raster();
        return node;
    }

    static bool decode(const Node& node, MosquitoParameters::GridBased& rhs) {
        if (!node["interrupted_feeding_rate_raster"] || !node["prmc_size_raster"]) {
            throw std::runtime_error("Missing fields in MosquitoParameters::GridBased");
        }
        rhs.set_interrupted_feeding_rate_raster(node["interrupted_feeding_rate_raster"].as<std::string>());
        rhs.set_prmc_size_raster(node["prmc_size_raster"].as<std::string>());
        return true;
    }
};

// MosquitoParameters::LocationBased YAML conversion
template<>
struct convert<MosquitoParameters::LocationBased> {
    static Node encode(const MosquitoParameters::LocationBased& rhs) {
        Node node;
        node["interrupted_feeding_rate"] = rhs.get_interrupted_feeding_rate();
        node["prmc_size"] = rhs.get_prmc_size();
        return node;
    }

    static bool decode(const Node& node, MosquitoParameters::LocationBased& rhs) {
        if (!node["interrupted_feeding_rate"] || !node["prmc_size"]) {
            throw std::runtime_error("Missing fields in MosquitoParameters::LocationBased");
        }
        rhs.set_interrupted_feeding_rate(node["interrupted_feeding_rate"].as<std::vector<double>>());
        rhs.set_prmc_size(node["prmc_size"].as<std::vector<int>>());
        return true;
    }
};

// MosquitoParameters::MosquitoConfig YAML conversion
template<>
struct convert<MosquitoParameters::MosquitoConfig> {
    static Node encode(const MosquitoParameters::MosquitoConfig& rhs) {
        Node node;
        node["mode"] = rhs.get_mode();
        if (rhs.get_mode() == "grid_based") {
            node["grid_based"] = rhs.get_grid_based();
        } else if (rhs.get_mode() == "location_based") {
            node["location_based"] = rhs.get_location_based();
        }
        return node;
    }

    static bool decode(const Node& node, MosquitoParameters::MosquitoConfig& rhs) {
        if (!node["mode"]) {
            throw std::runtime_error("Missing 'mode' field in MosquitoParameters::MosquitoConfig");
        }
        std::string mode = node["mode"].as<std::string>();
        rhs.set_mode(mode);

        if (mode == "grid_based") {
            rhs.set_grid_based(node["grid_based"].as<MosquitoParameters::GridBased>());
        } else if (mode == "location_based") {
            rhs.set_location_based(node["location_based"].as<MosquitoParameters::LocationBased>());
        } else {
            throw std::runtime_error("Invalid mode in MosquitoParameters::MosquitoConfig");
        }
        return true;
    }
};

// MosquitoParameters YAML conversion
template<>
struct convert<MosquitoParameters> {
    static Node encode(const MosquitoParameters& rhs) {
        Node node;
        node["mosquito_config"] = rhs.get_mosquito_config();
        node["within_host_induced_free_recombination"] = rhs.get_within_host_induced_free_recombination();
        return node;
    }

    static bool decode(const Node& node, MosquitoParameters& rhs) {
        if (!node["mosquito_config"] || !node["within_host_induced_free_recombination"]) {
            throw std::runtime_error("Missing 'mosquito_config' field in MosquitoParameters");
        }
        rhs.set_mosquito_config(node["mosquito_config"].as<MosquitoParameters::MosquitoConfig>());
        rhs.set_within_host_induced_free_recombination(node["within_host_induced_free_recombination"].as<bool>());
        return true;
    }
};

}  // namespace YAML


#endif //MOSQUITOPARAMETERS_H
