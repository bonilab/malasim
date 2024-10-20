#include "Utils/Logger.h"
#include "Configuration/Config.h"

int main() {
  Logger::Instance().Initialize(spdlog::level::info);
  spdlog::get("default_logger")->info("Hello, World!");

  Config *config = new Config();
  config->load("../../sample_inputs/input.yml");
  return 0;
}
