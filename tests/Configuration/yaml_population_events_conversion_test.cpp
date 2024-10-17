#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/PopulationEvents.h"

class PopulationEventsTest : public ::testing::Test {
protected:
    PopulationEvents population_events;

    void SetUp() override {
        // Setup ParasiteCaseInfo
        PopulationEvents::ParasiteCaseInfo case_info;
        case_info.set_date(date::year_month_day{date::year(2024), date::month(1), date::day(1)});
        case_info.set_genotype_id(1);
        case_info.set_number_of_cases(100);

        // Setup PeriodicParasiteIntroductionInfo
        PopulationEvents::PeriodicParasiteIntroductionInfo parasite_intro;
        parasite_intro.set_location(1);
        parasite_intro.set_duration(30);
        parasite_intro.set_number_of_cases(200);
        parasite_intro.set_start_date(date::year_month_day{date::year(2024), date::month(1), date::day(1)});
        parasite_intro.set_end_date(date::year_month_day{date::year(2024), date::month(1), date::day(30)});

        // Setup EventInfo
        PopulationEvents::EventInfo event_info;
        event_info.set_date(date::year_month_day{date::year(2024), date::month(1), date::day(1)});
        event_info.set_mutation_status(true);
        event_info.set_parasite_fraction(0.2);
        event_info.set_location_id(1);
        event_info.set_parasite_cases({case_info});
        event_info.set_periodic_parasite_info(parasite_intro);
        event_info.set_treatment_strategy_id(5);
        event_info.set_interrupted_feeding_rate(0.1);
        event_info.set_mutation_probability_per_locus(0.01);

        // Setup Event
        PopulationEvents::Event event;
        event.set_event_name("ParasiteIntroduction");
        event.set_event_info({event_info});

        // Set PopulationEvents
        std::map<std::string, PopulationEvents::Event> event_list;
        event_list["ParasiteIntroduction"] = event;
        population_events.set_event_list(event_list);
    }
};

// Test encoding functionality
TEST_F(PopulationEventsTest, EncodePopulationEvents) {
    YAML::Node node = YAML::convert<PopulationEvents>::encode(population_events);

    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_name"].as<std::string>(), "ParasiteIntroduction");
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["day"].as<date::year_month_day>().year(),date::year(2024));
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["day"].as<date::year_month_day>().month(),date::month(1));
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["day"].as<date::year_month_day>().day(),date::day(1));
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["mutation_status"].as<bool>(), true);
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["parasite_fraction"].as<double>(), 0.2);
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["parasite_cases"][0]["day"].as<std::string>(), "2024-01-01");
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["parasite_cases"][0]["genotype_id"].as<int>(), 1);
    EXPECT_EQ(node["event_list"]["ParasiteIntroduction"]["event_info"][0]["parasite_cases"][0]["number_of_cases"].as<int>(), 100);
}

// Test decoding functionality
TEST_F(PopulationEventsTest, DecodePopulationEvents) {
}

// Test for decoding with missing fields
TEST_F(PopulationEventsTest, DecodePopulationEventsMissingField) {
    // YAML::Node node;
    // node["event_list"]["ParasiteIntroduction"]["event_name"] = "ParasiteIntroduction";  // Missing event_info
    //
    // PopulationEvents decoded_population_events;
    // EXPECT_THROW(YAML::convert<PopulationEvents>::decode(node, decoded_population_events), std::runtime_error);
}
