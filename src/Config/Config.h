// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <yaml-cpp/yaml.h>

#include <functional>
#include <shared_mutex>
// #include <memory>
// #include <string>
// #include <vector>

// Forward declaration
class Model;

// Define configuration sections
struct SimulationConfig {
  double timestep;
  double duration;
};

struct PhysicsConfig {
  double gravity;
  double air_resistance;
};

struct OutputConfig {
  std::string log_level;
  std::string file_path;
};

// Aggregated Config Structure
struct ConfigData {
  SimulationConfig simulation;
  PhysicsConfig physics;
  OutputConfig output;
};

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

  // Getters for configuration sections
  SimulationConfig GetSimulationConfig() const;
  PhysicsConfig GetPhysicsConfig() const;
  OutputConfig GetOutputConfig() const;

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
