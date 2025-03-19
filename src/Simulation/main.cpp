#include <Core/Scheduler/Scheduler.h>

#include "Utils/Cli.hxx"
#include "Model.h"
#include "Utils/Logger.h"


int main(int argc, char** argv) {
  Logger::Instance().Initialize(spdlog::level::info);
  utils::Cli::get_instance().parse(argc, argv);
  if(Model::get_instance().initialize()) {
    Model::get_instance().run();
    Model::get_instance().release();
  }
  else {
    spdlog::get("default_logger")->error("Model initialization failed.");
  }
  return 0;
}
