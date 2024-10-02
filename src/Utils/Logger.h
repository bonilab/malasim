#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

class Logger {
public:
  // Initializes the loggers with a specified log level
  static void Initialize(
      spdlog::level::level_enum log_level = spdlog::level::info);

  // Retrieves a logger by name; creates it if it doesn't exist
};

#endif  // LOGGER_H
