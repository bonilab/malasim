#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/SeasonalitySettings.h"

class SeasonalitySettingsTest : public ::testing::Test {
protected:
    SeasonalitySettings default_settings;

    void SetUp() override {
        // Initialize default SeasonalitySettings object using setters
        SeasonalitySettings::RainfallSettings rainfall;
        rainfall.set_filename("test.csv");
        rainfall.set_period(date::days{365});

        SeasonalitySettings::SimpleSettings simple;
        simple.set_a({1.0});
        simple.set_phi({250});
        simple.set_min_value({0.1});
        simple.set_period(date::days{365});

        default_settings.set_enable(true);
        default_settings.set_mode("rainfall");
        default_settings.set_rainfall(rainfall);
        default_settings.set_simple(simple);
    }
};

// Test encoding functionality
TEST_F(SeasonalitySettingsTest, EncodeSeasonalitySettings) {
    YAML::Node node = YAML::convert<SeasonalitySettings>::encode(default_settings);

    EXPECT_EQ(node["enable"].as<bool>(), default_settings.get_enable());
    EXPECT_EQ(node["mode"].as<std::string>(), default_settings.get_mode());

    auto rainfall = default_settings.get_rainfall();
    EXPECT_EQ(node["rainfall"]["filename"].as<std::string>(), rainfall.get_filename());
    EXPECT_EQ(node["rainfall"]["period"].as<int>(), rainfall.get_period().count());

    auto simple = default_settings.get_simple();
    EXPECT_EQ(node["simple"]["a"].as<std::vector<double>>(), simple.get_a());
    EXPECT_EQ(node["simple"]["phi"].as<std::vector<int>>(), simple.get_phi());
    EXPECT_EQ(node["simple"]["min_value"].as<std::vector<double>>(), simple.get_min_value());
    EXPECT_EQ(node["simple"]["period"].as<int>(), simple.get_period().count());
}

// Test decoding functionality
TEST_F(SeasonalitySettingsTest, DecodeSeasonalitySettings) {
    YAML::Node node;
    node["enable"] = true;
    node["mode"] = "rainfall";

    YAML::Node rainfall_node;
    rainfall_node["filename"] = "test.csv";
    rainfall_node["period"] = 365;

    YAML::Node simple_node;
    simple_node["a"] = std::vector<double>{1.0};
    simple_node["phi"] = std::vector<int>{250};
    simple_node["min_value"] = std::vector<double>{0.1};
    simple_node["period"] = 365;

    node["rainfall"] = rainfall_node;
    node["simple"] = simple_node;

    SeasonalitySettings decoded_settings;
    EXPECT_NO_THROW(YAML::convert<SeasonalitySettings>::decode(node, decoded_settings));

    EXPECT_EQ(decoded_settings.get_enable(), true);
    EXPECT_EQ(decoded_settings.get_mode(), "rainfall");

    auto decoded_rainfall = decoded_settings.get_rainfall();
    EXPECT_EQ(decoded_rainfall.get_filename(), "test.csv");
    EXPECT_EQ(decoded_rainfall.get_period().count(), 365);

    auto decoded_simple = decoded_settings.get_simple();
    EXPECT_EQ(decoded_simple.get_a(), std::vector<double>{1.0});
    EXPECT_EQ(decoded_simple.get_phi(), std::vector<int>{250});
    EXPECT_EQ(decoded_simple.get_min_value(), std::vector<double>{0.1});
    EXPECT_EQ(decoded_simple.get_period().count(), 365);
}

// Test missing fields during decoding
TEST_F(SeasonalitySettingsTest, DecodeSeasonalitySettingsMissingField) {
    YAML::Node node;
    node["enable"] = true;  // Intentionally omit other fields

    SeasonalitySettings decoded_settings;
    EXPECT_THROW(YAML::convert<SeasonalitySettings>::decode(node, decoded_settings), std::runtime_error);
}
