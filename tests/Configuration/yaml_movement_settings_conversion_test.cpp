#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/MovementSettings.h"

class MovementSettingsTest : public ::testing::Test {
protected:
    MovementSettings movement_settings;

    void SetUp() override {
        // Initialize the movement settings object
        BarabasiSettings barabasi;
        barabasi.set_r_g_0(1.0);
        barabasi.set_beta_r(0.5);
        barabasi.set_kappa(3);

        WesolowskiSettings wesolowski;
        wesolowski.set_kappa(2.0);
        wesolowski.set_alpha(0.1);
        wesolowski.set_beta(0.2);
        wesolowski.set_gamma(0.3);

        SpatialModel spatial_model;
        spatial_model.set_name("TestModel");
        spatial_model.set_barabasi(barabasi);
        spatial_model.set_wesolowski(wesolowski);

        MovingLevelDistributionGamma gamma;
        gamma.set_mean(5.0);
        gamma.set_sd(2.0);

        MovingLevelDistribution moving_level_dist;
        moving_level_dist.set_distribution("gamma");
        moving_level_dist.set_gamma(gamma);

        CirculationInfo circulation_info;
        circulation_info.set_max_relative_moving_value(10);
        circulation_info.set_number_of_moving_levels(3);
        circulation_info.set_moving_level_distribution(moving_level_dist);
        circulation_info.set_circulation_percent(50.0);
        circulation_info.set_length_of_stay(gamma);

        movement_settings.set_spatial_model(spatial_model);
        movement_settings.set_circulation_info(circulation_info);
    }
};

// Test for encoding MovementSettings
TEST_F(MovementSettingsTest, EncodeMovementSettings) {
    YAML::Node node = YAML::convert<MovementSettings>::encode(movement_settings);

    // Check if spatial_model fields are encoded correctly
    EXPECT_EQ(node["spatial_model"]["name"].as<std::string>(), "TestModel");
    EXPECT_EQ(node["spatial_model"]["barabasi"]["r_g_0"].as<double>(), 1.0);
    EXPECT_EQ(node["spatial_model"]["wesolowski"]["kappa"].as<double>(), 2.0);

    // Check if circulation_info fields are encoded correctly
    EXPECT_EQ(node["circulation_info"]["max_relative_moving_value"].as<int>(), 10);
    EXPECT_EQ(node["circulation_info"]["moving_level_distribution"]["distribution"].as<std::string>(), "gamma");
    EXPECT_EQ(node["circulation_info"]["circulation_percent"].as<double>(), 50.0);
}

// Test for decoding MovementSettings
TEST_F(MovementSettingsTest, DecodeMovementSettings) {
    YAML::Node node;
    node["spatial_model"]["name"] = "TestModel";
    node["spatial_model"]["barabasi"]["r_g_0"] = 1.0;
    node["spatial_model"]["barabasi"]["beta_r"] = 0.5;
    node["spatial_model"]["barabasi"]["kappa"] = 3;
    node["spatial_model"]["wesolowski"]["kappa"] = 2.0;
    node["spatial_model"]["wesolowski"]["alpha"] = 0.1;
    node["spatial_model"]["wesolowski"]["beta"] = 0.2;
    node["spatial_model"]["wesolowski"]["gamma"] = 0.3;
    node["circulation_info"]["max_relative_moving_value"] = 10;
    node["circulation_info"]["number_of_moving_levels"] = 3;
    node["circulation_info"]["moving_level_distribution"]["distribution"] = "gamma";
    node["circulation_info"]["moving_level_distribution"]["gamma"]["mean"] = 5.0;
    node["circulation_info"]["moving_level_distribution"]["gamma"]["sd"] = 2.0;
    node["circulation_info"]["circulation_percent"] = 50.0;
    node["circulation_info"]["length_of_stay"]["mean"] = 5.0;
    node["circulation_info"]["length_of_stay"]["sd"] = 2.0;

    MovementSettings decoded_settings;
    EXPECT_NO_THROW(YAML::convert<MovementSettings>::decode(node, decoded_settings));

    EXPECT_EQ(decoded_settings.get_spatial_model().get_name(), "TestModel");
    EXPECT_EQ(decoded_settings.get_spatial_model().get_barabasi().get_r_g_0(), 1.0);
    EXPECT_EQ(decoded_settings.get_spatial_model().get_wesolowski().get_kappa(), 2.0);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_max_relative_moving_value(), 10);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_circulation_percent(), 50.0);
}

// Test for decoding with missing fields
TEST_F(MovementSettingsTest, DecodeMovementSettingsMissingField) {
    YAML::Node node;
    node["spatial_model"]["name"] = "TestModel";
    node["circulation_info"]["circulation_percent"] = 50.0;  // Missing most fields

    MovementSettings decoded_settings;
    EXPECT_THROW(YAML::convert<MovementSettings>::decode(node, decoded_settings), std::runtime_error);
}
