// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <date/date.h>
#include <yaml-cpp/yaml.h>

#include <functional>
#include <shared_mutex>
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

  // Load configuration from a YAML file
  void Load(const std::string &filename);

  // Reload configuration (useful for dynamic updates)
  void Reload();

  // Register an observer for configuration changes
  void RegisterObserver(ConfigObserver observer);

private:
  // Internal Method to Notify Observers
  void NotifyObservers();

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
