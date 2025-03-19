#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/MovementSettings.h"

class MovementSettingsTest : public ::testing::Test {
protected:
    MovementSettings movement_settings;

    void SetUp() override {
        MovementSettings::BarabasiSM barabasi;
        barabasi.set_r_g_0(1.0);
        barabasi.set_beta_r(0.5);
        barabasi.set_kappa(3);

        MovementSettings::WesolowskiSM wesolowski;
        wesolowski.set_kappa(2.0);
        wesolowski.set_alpha(0.1);
        wesolowski.set_beta(0.2);
        wesolowski.set_gamma(0.3);

        // Use the SpatialModelSettings class from MovementSettings
        MovementSettings::SpatialModelSettings spatial_model_settings;
        spatial_model_settings.set_name("TestModel");
        spatial_model_settings.set_barabasi_sm(barabasi);
        spatial_model_settings.set_wesolowski_sm(wesolowski);

        MovementSettings::MovingLevelDistributionGamma gamma;
        gamma.set_mean(5.0);
        gamma.set_sd(2.0);


        MovementSettings::LengthOfStay length_of_stay;
        length_of_stay.set_mean(5.0);
        length_of_stay.set_sd(2.0);


        MovementSettings::MovingLevelDistributionExponential exponential;
        exponential.set_scale( 0.17);

        MovementSettings::MovingLevelDistribution moving_level_dist;
        moving_level_dist.set_distribution("Gamma");
        moving_level_dist.set_gamma(gamma);
        moving_level_dist.set_exponential(exponential);

        MovementSettings::CirculationInfo circulation_info;
        circulation_info.set_max_relative_moving_value(10);
        circulation_info.set_number_of_moving_levels(3);
        circulation_info.set_moving_level_distribution(moving_level_dist);
        circulation_info.set_circulation_percent(50.0);
        circulation_info.set_length_of_stay(length_of_stay);

        // Set the updated objects in MovementSettings
        movement_settings.set_spatial_model_settings(spatial_model_settings);
        movement_settings.set_circulation_info(circulation_info);
    }
};

// Test for encoding MovementSettings
TEST_F(MovementSettingsTest, EncodeMovementSettings) {
    YAML::Node node = YAML::convert<MovementSettings>::encode(movement_settings);

    // Check if spatial_model_settings fields are encoded correctly
    EXPECT_EQ(node["spatial_model"]["name"].as<std::string>(), "TestModel");
    EXPECT_EQ(node["spatial_model"]["Barabasi"]["r_g_0"].as<double>(), 1.0);
    EXPECT_EQ(node["spatial_model"]["Barabasi"]["beta_r"].as<double>(), 0.5);
    EXPECT_EQ(node["spatial_model"]["Barabasi"]["kappa"].as<int>(), 3);
    EXPECT_EQ(node["spatial_model"]["Wesolowski"]["kappa"].as<double>(), 2.0);
    EXPECT_EQ(node["spatial_model"]["Wesolowski"]["alpha"].as<double>(), 0.1);
    EXPECT_EQ(node["spatial_model"]["Wesolowski"]["beta"].as<double>(), 0.2);
    EXPECT_EQ(node["spatial_model"]["Wesolowski"]["gamma"].as<double>(), 0.3);

    // Check if circulation_info fields are encoded correctly
    EXPECT_EQ(node["circulation_info"]["max_relative_moving_value"].as<int>(), 10);
    EXPECT_EQ(node["circulation_info"]["number_of_moving_levels"].as<int>(), 3);
    EXPECT_EQ(node["circulation_info"]["moving_level_distribution"]["distribution"].as<std::string>(), "Gamma");
    EXPECT_EQ(node["circulation_info"]["moving_level_distribution"]["Gamma"]["mean"].as<double>(), 5.0);
    EXPECT_EQ(node["circulation_info"]["moving_level_distribution"]["Gamma"]["sd"].as<double>(), 2.0);
    EXPECT_EQ(node["circulation_info"]["moving_level_distribution"]["Exponential"]["scale"].as<double>(), 0.17);
    EXPECT_EQ(node["circulation_info"]["circulation_percent"].as<double>(), 50.0);
    EXPECT_EQ(node["circulation_info"]["length_of_stay"]["mean"].as<double>(), 5.0);
    EXPECT_EQ(node["circulation_info"]["length_of_stay"]["sd"].as<double>(), 2.0);
}

// Test for decoding MovementSettings
TEST_F(MovementSettingsTest, DecodeMovementSettings) {
    YAML::Node node;
    node["spatial_model"]["name"] = "TestModel";
    node["spatial_model"]["Barabasi"]["r_g_0"] = 1.0;
    node["spatial_model"]["Barabasi"]["beta_r"] = 0.5;
    node["spatial_model"]["Barabasi"]["kappa"] = 3;
    node["spatial_model"]["Wesolowski"]["kappa"] = 2.0;
    node["spatial_model"]["Wesolowski"]["alpha"] = 0.1;
    node["spatial_model"]["Wesolowski"]["beta"] = 0.2;
    node["spatial_model"]["Wesolowski"]["gamma"] = 0.3;

    node["circulation_info"]["max_relative_moving_value"] = 10;
    node["circulation_info"]["number_of_moving_levels"] = 3;
    node["circulation_info"]["moving_level_distribution"]["distribution"] = "Gamma";
    node["circulation_info"]["moving_level_distribution"]["Gamma"]["mean"] = 5.0;
    node["circulation_info"]["moving_level_distribution"]["Gamma"]["sd"] = 2.0;
    node["circulation_info"]["moving_level_distribution"]["Exponential"]["scale"] = 0.17;
    node["circulation_info"]["circulation_percent"] = 50.0;
    node["circulation_info"]["length_of_stay"]["mean"] = 5.0;
    node["circulation_info"]["length_of_stay"]["sd"] = 2.0;

    MovementSettings decoded_settings;
    EXPECT_NO_THROW(YAML::convert<MovementSettings>::decode(node, decoded_settings));

    // Validate decoded settings
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_name(), "TestModel");
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_barabasi_sm().get_r_g_0(), 1.0);
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_barabasi_sm().get_beta_r(), 0.5);
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_barabasi_sm().get_kappa(), 3);

    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_wesolowski_sm().get_kappa(), 2.0);
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_wesolowski_sm().get_alpha(), 0.1);
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_wesolowski_sm().get_beta(), 0.2);
    EXPECT_EQ(decoded_settings.get_spatial_model_settings().get_wesolowski_sm().get_gamma(), 0.3);

    EXPECT_EQ(decoded_settings.get_circulation_info().get_max_relative_moving_value(), 10);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_number_of_moving_levels(), 3);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_circulation_percent(), 50.0);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_moving_level_distribution().get_distribution(), "Gamma");
    EXPECT_EQ(decoded_settings.get_circulation_info().get_moving_level_distribution().get_gamma().get_mean(), 5.0);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_moving_level_distribution().get_gamma().get_sd(), 2.0);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_moving_level_distribution().get_exponential().get_scale(), 0.17);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_length_of_stay().get_mean(), 5.0);
    EXPECT_EQ(decoded_settings.get_circulation_info().get_length_of_stay().get_sd(), 2.0);
}

// Test for decoding with missing fields
TEST_F(MovementSettingsTest, DecodeMovementSettingsMissingField) {
    YAML::Node node;
    node["spatial_model"]["name"] = "TestModel";
    // Intentionally missing many required fields under "spatial_model" and "circulation_info"
    node["circulation_info"]["circulation_percent"] = 50.0;

    MovementSettings decoded_settings;
    EXPECT_THROW(YAML::convert<MovementSettings>::decode(node, decoded_settings), std::runtime_error);
}
