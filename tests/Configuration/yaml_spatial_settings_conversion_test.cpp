#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/SpatialSettings.h"

// Helper function to compare std::array<double, 21>
template <typename T, size_t N>
void CompareArrays(const std::array<T, N>& expected, const std::array<T, N>& actual) {
    for (size_t i = 0; i < N; ++i) {
        EXPECT_EQ(expected[i], actual[i]);
    }
}

class SpatialSettingsTest : public ::testing::Test {
protected:
    SpatialSettings default_settings;

    void SetUp() override {
        default_settings.set_mode("grid_based");

        GridBased grid;
        grid.set_population_raster("../sample_inputs/kag_init_pop.asc");
        grid.set_district_raster("../sample_inputs/kag_district.asc");
        grid.set_p_treatment_under_5_raster("../sample_inputs/kag_treatment.asc");
        grid.set_p_treatment_over_5_raster("../sample_inputs/kag_treatment.asc");
        grid.set_p_treatment_under_5_by_location({-1});
        grid.set_p_treatment_over_5_by_location({-1});
        grid.set_beta_raster("../sample_inputs/kag_beta_r1.asc");
        grid.set_beta_by_location({-1});
        grid.set_cell_size(5.0);
        grid.set_age_distribution_by_location({{0.0378, 0.0378, 0.0378, 0.0378, 0.0282, 0.0282, 0.0282, 0.0282, 0.0282, 0.029, 0.029, 0.029, 0.029, 0.029, 0.169, 0.134, 0.106, 0.066, 0.053, 0.035, 0.0}});
        default_settings.set_grid_based(grid);
    }
};

// Test encoding functionality for SpatialSettings
TEST_F(SpatialSettingsTest, EncodeSpatialSettings) {
    YAML::Node node = YAML::convert<SpatialSettings>::encode(default_settings);

    EXPECT_EQ(node["mode"].as<std::string>(), default_settings.get_mode());

    const auto &grid = default_settings.get_grid_based();
    EXPECT_EQ(node["grid_based"]["population_raster"].as<std::string>(), grid.get_population_raster());
    EXPECT_EQ(node["grid_based"]["district_raster"].as<std::string>(), grid.get_district_raster());
    EXPECT_EQ(node["grid_based"]["p_treatment_under_5_raster"].as<std::string>(), grid.get_p_treatment_under_5_raster());
    EXPECT_EQ(node["grid_based"]["p_treatment_over_5_raster"].as<std::string>(), grid.get_p_treatment_over_5_raster());

    // Use helper function for comparing arrays
    for (size_t i = 0; i < grid.get_age_distribution_by_location().size(); ++i) {
        CompareArrays(grid.get_age_distribution_by_location()[i],
                      node["grid_based"]["age_distribution_by_location"].as<std::vector<std::array<double, 21>>>()[i]);
    }
}

// Test decoding functionality for SpatialSettings
TEST_F(SpatialSettingsTest, DecodeSpatialSettings) {
    YAML::Node node;
    node["mode"] = "grid_based";
    node["grid_based"]["population_raster"] = "../sample_inputs/kag_init_pop.asc";
    node["grid_based"]["district_raster"] = "../sample_inputs/kag_district.asc";
    node["grid_based"]["p_treatment_under_5_raster"] = "../sample_inputs/kag_treatment.asc";
    node["grid_based"]["p_treatment_over_5_raster"] = "../sample_inputs/kag_treatment.asc";
    node["grid_based"]["p_treatment_under_5_by_location"] = std::vector<double>{-1};
    node["grid_based"]["p_treatment_over_5_by_location"] = std::vector<double>{-1};
    node["grid_based"]["beta_raster"] = "../sample_inputs/kag_beta_r1.asc";
    node["grid_based"]["beta_by_location"] = std::vector<double>{-1};
    node["grid_based"]["cell_size"] = 5.0;
    node["grid_based"]["age_distribution_by_location"] = std::vector<std::array<double, 21>>{
        {0.0378, 0.0378, 0.0378, 0.0378, 0.0282, 0.0282, 0.0282, 0.0282, 0.0282, 0.029, 0.029, 0.029, 0.029, 0.029, 0.169, 0.134, 0.106, 0.066, 0.053, 0.035, 0.0}};

    SpatialSettings decoded_settings;
    EXPECT_NO_THROW(YAML::convert<SpatialSettings>::decode(node, decoded_settings));

    EXPECT_EQ(decoded_settings.get_mode(), "grid_based");

    const auto &grid = decoded_settings.get_grid_based();
    EXPECT_EQ(grid.get_population_raster(), "../sample_inputs/kag_init_pop.asc");
    EXPECT_EQ(grid.get_district_raster(), "../sample_inputs/kag_district.asc");
    EXPECT_EQ(grid.get_p_treatment_under_5_raster(), "../sample_inputs/kag_treatment.asc");
    EXPECT_EQ(grid.get_p_treatment_over_5_raster(), "../sample_inputs/kag_treatment.asc");

    for (size_t i = 0; i < grid.get_age_distribution_by_location().size(); ++i) {
        CompareArrays(grid.get_age_distribution_by_location()[i], 
                      std::vector<std::array<double, 21>>{{0.0378, 0.0378, 0.0378, 0.0378, 0.0282, 0.0282, 0.0282, 0.0282, 0.0282, 0.029, 0.029, 0.029, 0.029, 0.029, 0.169, 0.134, 0.106, 0.066, 0.053, 0.035, 0.0}}[i]);
    }
}

// Test for decoding with missing fields
TEST_F(SpatialSettingsTest, DecodeSpatialSettingsMissingField) {
  YAML::Node node;
  node["mode"] = "grid_based";  // Only set mode, leaving other fields empty

  SpatialSettings decoded_settings;
  EXPECT_THROW(YAML::convert<SpatialSettings>::decode(node, decoded_settings), std::runtime_error);
}
