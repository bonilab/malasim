#include <date/date.h>
#include <gtest/gtest.h>

#include "Configuration/ModelSettings.h"

class ModelSettingsTest : public ::testing::Test {
protected:
  ModelSettings default_settings;

  void SetUp() override {
    // Initialize default ModelSettings object using setters
    default_settings.set_days_between_stdout_output(10);
    default_settings.set_initial_seed_number(123);
    default_settings.set_record_genome_db(true);
  }
};

// Test encoding functionality
TEST_F(ModelSettingsTest, EncodeModelSettings) {
  YAML::Node node = YAML::convert<ModelSettings>::encode(default_settings);

  EXPECT_EQ(node["days_between_stdout_output"].as<int>(),
            default_settings.get_days_between_stdout_output());
  EXPECT_EQ(node["initial_seed_number"].as<int>(),
            default_settings.get_initial_seed_number());
  EXPECT_EQ(node["record_genome_db"].as<bool>(),
            default_settings.get_record_genome_db());
}

// Test decoding functionality
TEST_F(ModelSettingsTest, DecodeModelSettings) {
  YAML::Node node;
  node["days_between_stdout_output"] = 10;
  node["initial_seed_number"] = 123;
  node["record_genome_db"] = true;

  ModelSettings decoded_settings;
  EXPECT_NO_THROW(YAML::convert<ModelSettings>::decode(node, decoded_settings));

  EXPECT_EQ(decoded_settings.get_days_between_stdout_output(), 10);
  EXPECT_EQ(decoded_settings.get_initial_seed_number(), 123);
  EXPECT_EQ(decoded_settings.get_record_genome_db(), true);
}

// Test missing fields during decoding
TEST_F(ModelSettingsTest, DecodeModelSettingsMissingField) {
  YAML::Node node;
  node["initial_seed_number"] = 123;  // intentionally omit other fields

  ModelSettings decoded_settings;
  EXPECT_THROW(YAML::convert<ModelSettings>::decode(node, decoded_settings),
               std::runtime_error);
}

