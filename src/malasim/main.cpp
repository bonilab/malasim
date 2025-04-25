#include "Simulation/Model.h"
#include "Utils/Cli.h"
#include "Utils/Logger.h"
#include "spdlog/spdlog.h"
#include "version.h"

int main(int argc, char** argv) {
  Logger::initialize(spdlog::level::info);
  spdlog::info("Malaria Simulation v{}",VERSION);
  spdlog::info("Starting...");
  utils::Cli::get_instance().parse(argc, argv);
  if (Model::get_instance()->initialize()) {
    Model::get_instance()->run();
    Model::get_instance()->release();
  } else {
    spdlog::get("default_logger")->error("Model initialization failed.");
  }
  return 0;
}
