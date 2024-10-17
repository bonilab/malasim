#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/StrategyParameters.h"

class StrategyParametersTest : public ::testing::Test {
protected:
    StrategyParameters strategy_parameters;

    void SetUp() override {
        // Setup StrategyInfo
        StrategyParameters::StrategyInfo strategy_info;
        strategy_info.set_name("SP-AQ-CQ-AL-MFTStrategy");
        strategy_info.set_type("MFT");
        strategy_info.set_therapy_ids({5, 2, 12, 6});
        strategy_info.set_distribution({0.3, 0.3, 0.3, 0.1});

        // Setup MassDrugAdministration
        StrategyParameters::MassDrugAdministration mda_info;
        mda_info.set_enable(false);
        mda_info.set_mda_therapy_id(8);
        mda_info.set_age_bracket_prob_individual_present_at_mda({10, 40});
        mda_info.set_mean_prob_individual_present_at_mda({0.85, 0.75, 0.85});
        mda_info.set_sd_prob_individual_present_at_mda({0.3, 0.3, 0.3});

        // Set StrategyParameters
        strategy_parameters.set_strategy_db({strategy_info});
        strategy_parameters.set_initial_strategy_id(15);
        strategy_parameters.set_recurrent_therapy_id(-1);
        strategy_parameters.set_mass_drug_administration(mda_info);
    }
};

// Test encoding functionality
TEST_F(StrategyParametersTest, EncodeStrategyParameters) {
    YAML::Node node = YAML::convert<StrategyParameters>::encode(strategy_parameters);

    EXPECT_EQ(node["initial_strategy_id"].as<int>(), 15);
    EXPECT_EQ(node["recurrent_therapy_id"].as<int>(), -1);
    EXPECT_EQ(node["strategy_db"][0]["name"].as<std::string>(), "SP-AQ-CQ-AL-MFTStrategy");
    EXPECT_EQ(node["strategy_db"][0]["therapy_ids"].as<std::vector<int>>(), std::vector<int>({5, 2, 12, 6}));
    EXPECT_EQ(node["mass_drug_administration"]["enable"].as<bool>(), false);
    EXPECT_EQ(node["mass_drug_administration"]["age_bracket_prob_individual_present_at_mda"].as<std::vector<int>>(), std::vector<int>({10, 40}));
    EXPECT_EQ(node["mass_drug_administration"]["mean_prob_individual_present_at_mda"].as<std::vector<double>>(), std::vector<double>({0.85, 0.75, 0.85}));
}

// Test decoding functionality
TEST_F(StrategyParametersTest, DecodeStrategyParameters) {
    YAML::Node node;
    node["initial_strategy_id"] = 15;
    node["recurrent_therapy_id"] = -1;

    node["strategy_db"][0]["name"] = "SP-AQ-CQ-AL-MFTStrategy";
    node["strategy_db"][0]["type"] = "MFT";
    node["strategy_db"][0]["therapy_ids"] = std::vector<int>{5, 2, 12, 6};
    node["strategy_db"][0]["distribution"] = std::vector<double>{0.3, 0.3, 0.3, 0.1};

    node["mass_drug_administration"]["enable"] = false;
    node["mass_drug_administration"]["mda_therapy_id"] = 8;
    node["mass_drug_administration"]["age_bracket_prob_individual_present_at_mda"] = std::vector<int>{10, 40};
    node["mass_drug_administration"]["mean_prob_individual_present_at_mda"] = std::vector<double>{0.85, 0.75, 0.85};
    node["mass_drug_administration"]["sd_prob_individual_present_at_mda"] = std::vector<double>{0.3, 0.3, 0.3};

    StrategyParameters decoded_parameters;
    EXPECT_NO_THROW(YAML::convert<StrategyParameters>::decode(node, decoded_parameters));

    EXPECT_EQ(decoded_parameters.get_initial_strategy_id(), 15);
    EXPECT_EQ(decoded_parameters.get_recurrent_therapy_id(), -1);
    EXPECT_EQ(decoded_parameters.get_strategy_db()[0].get_name(), "SP-AQ-CQ-AL-MFTStrategy");
    EXPECT_EQ(decoded_parameters.get_strategy_db()[0].get_therapy_ids(), std::vector<int>({5, 2, 12, 6}));
    EXPECT_EQ(decoded_parameters.get_mass_drug_administration().get_enable(), false);
    EXPECT_EQ(decoded_parameters.get_mass_drug_administration().get_age_bracket_prob_individual_present_at_mda(), std::vector<int>({10, 40}));
    EXPECT_EQ(decoded_parameters.get_mass_drug_administration().get_mean_prob_individual_present_at_mda(), std::vector<double>({0.85, 0.75, 0.85}));
}

// Test for decoding with missing fields
TEST_F(StrategyParametersTest, DecodeStrategyParametersMissingField) {
    YAML::Node node;
    node["initial_strategy_id"] = 15;  // Missing other fields

    StrategyParameters decoded_parameters;
    EXPECT_THROW(YAML::convert<StrategyParameters>::decode(node, decoded_parameters), std::runtime_error);
}
