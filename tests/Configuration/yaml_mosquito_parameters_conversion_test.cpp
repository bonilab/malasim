#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/MosquitoParameters.h"

class MosquitoParametersTest : public ::testing::Test {
protected:
    MosquitoParameters mosquito_parameters;

    void SetUp() override {
        // Set up grid-based mosquito parameters
        MosquitoParameters::GridBased grid_based;
        grid_based.set_interrupted_feeding_rate_raster("../input/dev_mosquito_ifr.asc");
        grid_based.set_prmc_size_raster("../input/dev_mosquito_size.asc");

        // Set up location-based mosquito parameters
        MosquitoParameters::LocationBased location_based;
        location_based.set_interrupted_feeding_rate({0.19});
        location_based.set_prmc_size({100});

        // Set up mosquito config
        MosquitoParameters::MosquitoConfig config;
        config.set_mode("grid_based");
        config.set_grid_based(grid_based);
        config.set_location_based(location_based);

        mosquito_parameters.set_mosquito_config(config);
        mosquito_parameters.set_within_host_induced_free_recombination(true);
    }
};

// Test encoding functionality for MosquitoParameters
TEST_F(MosquitoParametersTest, EncodeMosquitoParameters) {
    YAML::Node node = YAML::convert<MosquitoParameters>::encode(mosquito_parameters);

    // Validate encoding of mosquito_config and grid-based mode
    EXPECT_EQ(node["mosquito_config"]["mode"].as<std::string>(), "grid_based");
    EXPECT_EQ(node["mosquito_config"]["grid_based"]["interrupted_feeding_rate_raster"].as<std::string>(), "../input/dev_mosquito_ifr.asc");
    EXPECT_EQ(node["mosquito_config"]["grid_based"]["prmc_size_raster"].as<std::string>(), "../input/dev_mosquito_size.asc");
}

// Test decoding functionality for MosquitoParameters (grid-based)
TEST_F(MosquitoParametersTest, DecodeMosquitoParametersGridBased) {
    YAML::Node node;
    node["mosquito_config"]["mode"] = "grid_based";
    node["mosquito_config"]["grid_based"]["interrupted_feeding_rate_raster"] = "../input/dev_mosquito_ifr.asc";
    node["mosquito_config"]["grid_based"]["prmc_size_raster"] = "../input/dev_mosquito_size.asc";
    node["within_host_induced_free_recombination"] = true;

    MosquitoParameters decoded_parameters;
    EXPECT_NO_THROW(YAML::convert<MosquitoParameters>::decode(node, decoded_parameters));

    // Validate decoding of grid-based mode
    EXPECT_EQ(decoded_parameters.get_mosquito_config().get_mode(), "grid_based");
    EXPECT_EQ(decoded_parameters.get_mosquito_config().get_grid_based().get_interrupted_feeding_rate_raster(), "../input/dev_mosquito_ifr.asc");
    EXPECT_EQ(decoded_parameters.get_mosquito_config().get_grid_based().get_prmc_size_raster(), "../input/dev_mosquito_size.asc");
}

// Test decoding functionality for MosquitoParameters (location-based)
TEST_F(MosquitoParametersTest, DecodeMosquitoParametersLocationBased) {
    YAML::Node node;
    node["mosquito_config"]["mode"] = "location_based";
    node["mosquito_config"]["location_based"]["interrupted_feeding_rate"] = std::vector<double>{0.19};
    node["mosquito_config"]["location_based"]["prmc_size"] = std::vector<int>{100};
    node["within_host_induced_free_recombination"] = true;

    MosquitoParameters decoded_parameters;
    EXPECT_NO_THROW(YAML::convert<MosquitoParameters>::decode(node, decoded_parameters));

    // Validate decoding of location-based mode
    EXPECT_EQ(decoded_parameters.get_mosquito_config().get_mode(), "location_based");
    EXPECT_EQ(decoded_parameters.get_mosquito_config().get_location_based().get_interrupted_feeding_rate()[0], 0.19);
    EXPECT_EQ(decoded_parameters.get_mosquito_config().get_location_based().get_prmc_size()[0], 100);
}

// Test for decoding with missing fields
TEST_F(MosquitoParametersTest, DecodeMosquitoParametersMissingField) {
    YAML::Node node;
    node["mosquito_config"]["mode"] = "grid_based";  // Missing grid-based data

    MosquitoParameters decoded_parameters;
    EXPECT_THROW(YAML::convert<MosquitoParameters>::decode(node, decoded_parameters), std::runtime_error);
}
