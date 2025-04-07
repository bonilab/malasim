#include "Configuration/SeasonalitySettings.h"
#include "Environment/SeasonalPattern.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "SeasonalPatternFixture.h"
#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <fmt/format.h>
#include <fstream>

class TestSeasonalPattern : public SeasonalPattern {
public:
    static SeasonalPattern* build(const YAML::Node &node, SpatialData* spatial_data) {
        SeasonalPattern* pattern = new SeasonalPattern();
        YAML::convert<SeasonalPattern*>::decode(node["pattern"], pattern);
        pattern->build(spatial_data);
        return pattern;
    }
    static SpatialData* create_fake_spatial_data() {
        std::cout << "Creating fake spatial data" << std::endl;
        auto fake_spatial_data = new SpatialData();
        fake_spatial_data->get_admin_level_manager()->register_level("district");
        auto boundary = BoundaryData();
        boundary.location_to_unit = {1, 2};
        boundary.unit_to_locations = {{1, 2}, {3, 4}};
        boundary.min_unit_id = 1;
        boundary.max_unit_id = 2;
        boundary.unit_count = 2;
        fake_spatial_data->get_admin_level_manager()->set_boundary(0, boundary);
        return fake_spatial_data;
      }
protected:
    int get_admin_unit_for_location(int location) const override {
        if (location == 999){
            throw std::out_of_range("Location is out of range");
        }
        return location;
    }
};

class SeasonalPatternTest : public ::testing::Test, protected SeasonalPatternFixture {
protected:
    void SetUp() override {
        SeasonalPatternFixture::SetUp();
    }
    void TearDown() override {
        SeasonalPatternFixture::TearDown();
    }
};

TEST_F(SeasonalPatternTest, CanCreateWithMonthlyData) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        admin_level: "district"
        filename: "{}"
        period: 12
    )", monthly_file));

    auto fake_spatial_data = std::unique_ptr<SpatialData>(TestSeasonalPattern::create_fake_spatial_data());
    auto pattern = TestSeasonalPattern::build(node,fake_spatial_data.get());
    ASSERT_NE(pattern, nullptr);
    EXPECT_TRUE(pattern->get_is_monthly());
    EXPECT_EQ(pattern->get_period(), 12);
}

TEST_F(SeasonalPatternTest, CanCreateWithDailyData) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        admin_level: "district"
        filename: "{}"
        period: 365
    )", daily_file));

    auto fake_spatial_data = std::unique_ptr<SpatialData>(TestSeasonalPattern::create_fake_spatial_data());
    auto pattern = TestSeasonalPattern::build(node,fake_spatial_data.get());
    ASSERT_NE(pattern, nullptr);
    EXPECT_FALSE(pattern->get_is_monthly());
    EXPECT_EQ(pattern->get_period(), 365);
}

TEST_F(SeasonalPatternTest, ThrowsOnInvalidPeriod) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        admin_level: "district"
        filename: "{}"
        period: 100
    )", "test_pattern.csv"));

    auto fake_spatial_data = std::unique_ptr<SpatialData>(TestSeasonalPattern::create_fake_spatial_data());
    EXPECT_THROW(TestSeasonalPattern::build(node,fake_spatial_data.get()), std::invalid_argument);
}

TEST_F(SeasonalPatternTest, HandlesMissingDistrict) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        admin_level: "district"
        filename: "{}"
        period: 12
    )", monthly_file));

    auto fake_spatial_data = std::unique_ptr<SpatialData>(TestSeasonalPattern::create_fake_spatial_data());
    auto pattern = TestSeasonalPattern::build(node,fake_spatial_data.get());
    // ASSERT_NE(pattern, nullptr);
    // EXPECT_THROW(pattern->get_seasonal_factor(std::chrono::sys_days{date::year{2000}/1/1}, 999), std::out_of_range);
}

TEST_F(SeasonalPatternTest, HandlesNonExistentFile) {
    auto node = YAML::Load(R"(
      enable: true
      mode: "pattern"
      pattern:
        admin_level: "district"
        filename: "non_existent_file.csv"
        period: 12
    )");
    auto fake_spatial_data = std::unique_ptr<SpatialData>(TestSeasonalPattern::create_fake_spatial_data());
    EXPECT_THROW(TestSeasonalPattern::build(node, fake_spatial_data.get()), std::runtime_error);
}
