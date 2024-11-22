#ifndef MODEL_H
#define MODEL_H

#include <string>

// Forward declaration
class Config;
class Random;
class Cli;
class Scheduler;

class Model {
public:
  // Provides global access to the singleton instance
  static Model &instance() {
    static Model instance;
    return instance;
  }

  // Initialize the model
  bool initialize();

  // Run the simulation
  void run() const;

  // Finalize and clean up resources
  void finalize();

  // Access configuration in a controlled manner
  [[nodiscard]] const Config* get_config() const {
    if (!config_) {
      throw std::runtime_error(
          "Model not initialized. Call Initialize() first.");
    }
    return config_.get();
  }

  // Access scheduler in a controlled manner
  [[nodiscard]] const Scheduler* get_scheduler() const {
    if (!scheduler_) {
      throw std::runtime_error(
          "Model not initialized. Call Initialize() first.");
    }
    return scheduler_.get();
  }

  // Prevent copying and moving
  Model(const Model &) = delete;
  Model(Model &&) = delete;
  Model &operator=(const Model &) = delete;
  Model &operator=(Model &&) = delete;

private:
  // Private constructor and destructor
  Model();
  ~Model() = default;

  // Configuration managed by a smart pointer
  std::shared_ptr<Config> config_;

  std::shared_ptr<Scheduler> scheduler_;

  // Configuration file path with default value
  std::string config_file_path_;

  bool is_initialized_;
public:
  void begin_time_step();
  void end_time_step();
  void daily_update();
  void monthly_update();
  void yearly_update();

};

#endif  // MODEL_H

