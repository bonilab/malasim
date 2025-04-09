
#include <gtest/gtest.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Utils/Logger.h"

// Define a test environment to initialize spdlog•¶
class SpdlogEnvironment : public ::testing::Environment {
public:
  void SetUp() override { Logger::instance().initialize(spdlog::level::info); }

  void TearDown() override { spdlog::shutdown(); }
};

// Register the test environment Gtest will take the ownership of the environment
::testing::Environment* const global_env =
    ::testing::AddGlobalTestEnvironment(new SpdlogEnvironment());

