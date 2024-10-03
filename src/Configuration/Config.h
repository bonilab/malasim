// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <date/date.h>
#include <yaml-cpp/yaml.h>

#include <functional>
#include <shared_mutex>
#include <string>
#include <vector>

#include "ConfigData.h"

// Forward declaration
class Model;

// Observer Callback Type
using ConfigObserver = std::function<void(const ConfigData &)>;

class Config {
public:
  // Constructor and Destructor
  Config() = default;
  ~Config() = default;

  // delete copy and move constructors and assign operators
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;
  Config(Config &&) = delete;
  Config &operator=(Config &&) = delete;

  // Load configuration from a YAML file
  void load(const std::string &filename);

  // Reload configuration (useful for dynamic updates)
  void reload();

  // Register an observer for configuration changes
  void register_observer(ConfigObserver observer);

private:
  // Internal Method to Notify Observers
  void notify_observers();

  // Configuration Data
  ConfigData config_data_;

  // Mutex for Thread-Safe Access
  mutable std::shared_mutex mutex_;

  // Observers
  std::vector<ConfigObserver> observers_;

  // Configuration File Path
  std::string config_file_path_;
};

#endif  // CONFIG_H
