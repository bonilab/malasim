// Logger.cpp
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

void Logger::Initialize(spdlog::level::level_enum log_level) {
  try {
    // Default logger
    auto default_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto default_logger =
        std::make_shared<spdlog::logger>("default_logger", default_sink);
    spdlog::set_default_logger(default_logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
    spdlog::info("Default logger initialized.");

    // ConfigValidator logger
    auto config_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto config_logger =
        std::make_shared<spdlog::logger>("config_validator", config_sink);
    spdlog::register_logger(config_logger);
    config_logger->set_level(spdlog::level::info);
    config_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [ConfigValidator] %v");
    config_logger->debug("ConfigValidator logger initialized.");

    // Network logger
    auto network_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto network_logger =
        std::make_shared<spdlog::logger>("network", network_sink);
    spdlog::register_logger(network_logger);
    network_logger->set_level(spdlog::level::info);
    network_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [Network] %v");
    network_logger->info("Network logger initialized.");

  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
  }
}

