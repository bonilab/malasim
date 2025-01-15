#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <string>
#include <Core/Scheduler/Dispatcher.h>
#include <Utils/Random.h>

// Forward declaration
class Config;
class Random;
class Cli;
class Scheduler;
class Population;

class Model {
public:
  // Provides global access to the singleton instance
  static Model &get_instance() {
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
  [[nodiscard]] Config* get_config() {
    if (config_ == nullptr) {
      throw std::runtime_error(
          "Model not initialized. Call Initialize() first.");
    }
    return config_;
  }

  // Access scheduler in a controlled manner
  [[nodiscard]] Scheduler* get_scheduler() const {
    if (scheduler_ == nullptr) {
      throw std::runtime_error(
          "Model not initialized. Call Initialize() first.");
    }
    return scheduler_;
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
  Config* config_;

  Scheduler* scheduler_;

  // Configuration file path with default value
  std::string config_file_path_;

  bool is_initialized_;

  Population* population_;
  utils::Random* random_;
public:
  void begin_time_step();
  void end_time_step();
  void daily_update();
  void monthly_update();
  void yearly_update();
  utils::Random* get_random();

};

#endif  // MODEL_H

