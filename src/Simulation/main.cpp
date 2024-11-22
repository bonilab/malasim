#include "Utils/Cli.h"
#include "Model.h"
#include "Utils/Logger.h"
#include "Configuration/Config.h"

int main(int argc, char** argv) {
  Logger::Instance().Initialize(spdlog::level::info);
  utils::Cli::get_instance().parse(argc, argv);
  if(Model::instance().initialize()) {
    Model::instance().run();
    Model::instance().finalize();
  }
  else {
    spdlog::get("default_logger")->error("Model initialization failed.");
  }
  return 0;
}
