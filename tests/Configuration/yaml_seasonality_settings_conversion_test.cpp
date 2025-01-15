#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/SeasonalitySettings.h"

class SeasonalitySettingsTest : public ::testing::Test {
protected:
    SeasonalitySettings default_settings = SeasonalitySettings();

    void SetUp() override {
        // Initialize default SeasonalitySettings object using setters
        SeasonalitySettings::SeasonalEquation equation;
        equation.base_ = std::vector<double>{2.5};
        equation.A_ = std::vector<double>{0.4};
        equation.B_ = std::vector<double>{0.6};
        equation.phi_ = std::vector<int>{146};
        equation.raster_ = true;

        SeasonalitySettings::SeasonalRainfall rainfall;
        rainfall.set_filename("../../sample_inputs/dev_seasonality.csv");
        rainfall.set_period(365);

        default_settings.set_enable(true);
        default_settings.set_mode("rainfall");
        default_settings.set_seasonal_rainfall(rainfall);
        default_settings.set_seasonal_equation(equation);
    }
};

// Test encoding functionality
TEST_F(SeasonalitySettingsTest, EncodeSeasonalitySettings) {
    YAML::Node node = YAML::convert<SeasonalitySettings>::encode(default_settings);

    EXPECT_EQ(node["enable"].as<bool>(), default_settings.get_enable());
    EXPECT_EQ(node["mode"].as<std::string>(), default_settings.get_mode());

    auto rainfall = default_settings.get_seasonal_rainfall();
    EXPECT_EQ(node["rainfall"]["filename"].as<std::string>(), rainfall.get_filename());
    EXPECT_EQ(node["rainfall"]["period"].as<int>(), rainfall.get_period());

    auto equation = default_settings.get_seasonal_equation();

    EXPECT_EQ(node["equation"]["base"].as<std::vector<double>>(), equation.base_);
    EXPECT_EQ(node["equation"]["a"].as<std::vector<double>>(), equation.A_);
    EXPECT_EQ(node["equation"]["b"].as<std::vector<double>>(), equation.B_);
    EXPECT_EQ(node["equation"]["phi"].as<std::vector<int>>(), equation.phi_);
}

// Test decoding functionality
TEST_F(SeasonalitySettingsTest, DecodeSeasonalitySettings) {
    YAML::Node node;
    node["enable"] = true;
    node["mode"] = "rainfall";

    node["rainfall"]["filename"] = "../../sample_inputs/dev_seasonality.csv";
    node["rainfall"]["period"] = 365;

    node["equation"]["base"] = std::vector<double>{2.5};
    node["equation"]["a"] = std::vector<double>{0.4};
    node["equation"]["b"] = std::vector<double>{0.6};
    node["equation"]["phi"] = std::vector<int>{146};
    node["equation"]["raster"] = true;

    SeasonalitySettings decoded_settings;
    EXPECT_NO_THROW(YAML::convert<SeasonalitySettings>::decode(node, decoded_settings));

    EXPECT_EQ(decoded_settings.get_enable(), true);
    EXPECT_EQ(decoded_settings.get_mode(), "rainfall");

    auto decoded_rainfall = decoded_settings.get_seasonal_rainfall();
    EXPECT_EQ(decoded_rainfall.get_filename(), "../../sample_inputs/dev_seasonality.csv");
    EXPECT_EQ(decoded_rainfall.get_period(), 365);

    auto decoded_equation = decoded_settings.get_seasonal_equation();

    EXPECT_EQ(decoded_equation.base_, std::vector<double>{2.5});
    EXPECT_EQ(decoded_equation.A_, std::vector<double>{0.4});
    EXPECT_EQ(decoded_equation.B_, std::vector<double>{0.6});
    EXPECT_EQ(decoded_equation.phi_, std::vector<int>{146});
}

// Test missing fields during decoding
TEST_F(SeasonalitySettingsTest, DecodeSeasonalitySettingsMissingField) {
    YAML::Node node;
    node["enable"] = true;  // Intentionally omit other fields

    SeasonalitySettings decoded_settings;
    EXPECT_THROW(YAML::convert<SeasonalitySettings>::decode(node, decoded_settings), std::runtime_error);
}
