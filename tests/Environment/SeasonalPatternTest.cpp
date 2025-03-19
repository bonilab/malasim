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
    static SeasonalPattern* build(const YAML::Node &node) {
        SeasonalPattern* pattern = new SeasonalPattern();
        YAML::convert<SeasonalPattern*>::decode(node["pattern"], pattern);
        pattern->build();
        return pattern;
    }
protected:
    int get_district_for_location(int location) const override {
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
        filename: "{}"
        period: 12
    )", monthly_file));

    auto pattern = TestSeasonalPattern::build(node);
    ASSERT_NE(pattern, nullptr);
    EXPECT_TRUE(pattern->get_is_monthly());
    EXPECT_EQ(pattern->get_period(), 12);
}

TEST_F(SeasonalPatternTest, CanCreateWithDailyData) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        filename: "{}"
        period: 365
    )", daily_file));

    auto pattern = TestSeasonalPattern::build(node);
    ASSERT_NE(pattern, nullptr);
    EXPECT_FALSE(pattern->get_is_monthly());
    EXPECT_EQ(pattern->get_period(), 365);
}

TEST_F(SeasonalPatternTest, ThrowsOnInvalidPeriod) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        filename: "{}"
        period: 100
    )", "test_pattern.csv"));

    EXPECT_THROW(TestSeasonalPattern::build(node), std::invalid_argument);
}

TEST_F(SeasonalPatternTest, HandlesMissingDistrict) {
    auto node = YAML::Load(fmt::format(R"(
      enable: true
      mode: "pattern"
      pattern:
        filename: "{}"
        period: 12
    )", monthly_file));

    auto pattern = TestSeasonalPattern::build(node);
    ASSERT_NE(pattern, nullptr);
    EXPECT_THROW(pattern->get_seasonal_factor({date::year{2000}/1/1}, 999), std::out_of_range);
}

TEST_F(SeasonalPatternTest, HandlesNonExistentFile) {
    auto node = YAML::Load(R"(
      enable: true
      mode: "pattern"
      pattern:
        filename: "non_existent_file.csv"
        period: 12
    )");
    EXPECT_THROW(TestSeasonalPattern::build(node), std::runtime_error);
}
