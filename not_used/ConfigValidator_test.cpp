#include "Configuration/ConfigValidator.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

class ConfigValidatorTest : public ::testing::Test {
protected:
  ConfigValidator validator;

  void SetUp() override {
    // Optional: Initialize any shared resources
  }
  void TearDown() override {
    // Optional: Clean up any shared resources
  }
};

TEST_F(ConfigValidatorTest, Validate) {
  YAML::Node node = YAML::Load("{ settings: { pi: 3.14159}, alpha: 0.5 }");
  // YAML::Emitter out;
  // out << node;
  // spdlog::info("node: {}", out.c_str());

  // Create a Lua state and load Sol2's standard libraries
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);

  validator.LoadConfigToLua(lua, node);

  sol::table config = lua["config"];

  sol::table settings = config["settings"];

  EXPECT_DOUBLE_EQ(settings.get<double>("pi"), 3.14159);

  EXPECT_DOUBLE_EQ(config["alpha"], 0.5);
}
