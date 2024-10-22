#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <memory>

class Logger {
public:
    // Retrieves the singleton instance of Logger
    static Logger& Instance() {
        static Logger instance;
        return instance;
    }

    // Initializes the loggers with a specified log level
    void Initialize(spdlog::level::level_enum log_level = spdlog::level::info);

    // Prevent copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    // Private constructor to prevent instantiation
    Logger() = default;

    // Private destructor
    ~Logger() = default;
};

#endif  // LOGGER_H
