#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <date/date.h>
#include <sstream>
#include "Configuration/SimulationTimeframe.h"

// Test fixture class for SimulationTimeframe tests
class SimulationTimeframeTest : public ::testing::Test {
protected:
    SimulationTimeframe default_timeframe;

    // Set up default values for the tests
    void SetUp() override {
        default_timeframe.set_starting_date(date::year_month_day{date::year{2000}, date::month{1}, date::day{1}});
        default_timeframe.set_start_of_comparison_period_date(date::year_month_day{date::year{2010}, date::month{1}, date::day{1}});
        default_timeframe.set_ending_date(date::year_month_day{date::year{2030}, date::month{1}, date::day{1}});
        default_timeframe.set_start_collect_data_day(0);
    }
};

// Test encoding functionality
TEST_F(SimulationTimeframeTest, EncodeSimulationTimeframe) {
    YAML::Node node = YAML::convert<SimulationTimeframe>::encode(default_timeframe);

    EXPECT_EQ(node["starting_date"].as<std::string>(), "2000/01/01");
    EXPECT_EQ(node["start_of_comparison_period"].as<std::string>(), "2010/01/01");
    EXPECT_EQ(node["ending_date"].as<std::string>(), "2030/01/01");
    EXPECT_EQ(node["start_collect_data_day"].as<int>(), 0);
}

// Test decoding functionality
TEST_F(SimulationTimeframeTest, DecodeSimulationTimeframe) {
    YAML::Node node;
    node["starting_date"] = "2000/01/01";
    node["start_of_comparison_period"] = "2010/01/01";
    node["ending_date"] = "2030/01/01";
    node["start_collect_data_day"] = 0;

    SimulationTimeframe decoded_timeframe;
    EXPECT_NO_THROW(YAML::convert<SimulationTimeframe>::decode(node, decoded_timeframe));

    // Wrap braced initializer lists in parentheses
    EXPECT_EQ(decoded_timeframe.get_starting_date(), (date::year_month_day{date::year{2000}, date::month{1}, date::day{1}}));
    EXPECT_EQ(decoded_timeframe.get_start_of_comparison_period_date(), (date::year_month_day{date::year{2010}, date::month{1}, date::day{1}}));
    EXPECT_EQ(decoded_timeframe.get_ending_date(), (date::year_month_day{date::year{2030}, date::month{1}, date::day{1}}));
    EXPECT_EQ(decoded_timeframe.get_start_collect_data_day(), 0);
}

// Test decoding with missing fields
TEST_F(SimulationTimeframeTest, DecodeSimulationTimeframeMissingField) {
    YAML::Node node;
    node["start_of_comparison_period"] = "2010/01/01";
    node["ending_date"] = "2030/01/01";
    node["start_collect_data_day"] = 0; // Intentionally missing starting_date

    SimulationTimeframe decoded_timeframe;
    EXPECT_THROW(YAML::convert<SimulationTimeframe>::decode(node, decoded_timeframe), std::runtime_error);
}

// Test invalid dates during decoding
TEST_F(SimulationTimeframeTest, InvalidDateThrowsError) {
    YAML::Node node;
    node["starting_date"] = "invalid_date";
    node["start_of_comparison_period"] = "2010/01/01";
    node["ending_date"] = "2030/01/01";
    node["start_collect_data_day"] = 0;

    SimulationTimeframe decoded_timeframe;
    EXPECT_THROW(YAML::convert<SimulationTimeframe>::decode(node, decoded_timeframe), std::runtime_error);
}
