#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

void Logger::initialize(spdlog::level::level_enum log_level) {
  try {
    // Default logger
    auto default_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto default_logger = std::make_shared<spdlog::logger>("default_logger", default_sink);
    spdlog::set_default_logger(default_logger);
    spdlog::set_level(log_level);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
    spdlog::info("Default logger initialized.");

  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Logger initialization failed: " << ex.what() << '\n';
  }
}
