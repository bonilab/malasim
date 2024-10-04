#include "Model.h"

#include <iostream>
#include <stdexcept>

#include "Configuration/Config.h"  // Assuming Config is defined here

// Private constructor: creates the Config instance
Model::Model()
    : config_(std::make_unique<Config>()),
      config_file_path_("config.yml"),
      is_initialized_(false) {
  // Constructor does not load the configuration file
  // Initialization is deferred to the Initialize() method
}

void Model::initialize() {
  if (!config_file_path_.empty()) {
    // Load the configuration file
    config_->load(config_file_path_);

    std::cout << "Model initialized with configuration file: "
              << config_file_path_ << "\n";
    is_initialized_ = true;
  } else {
    throw std::invalid_argument("Configuration file path must be provided.");
  }
}

void Model::run() const {
  if (!is_initialized_) {
    throw std::runtime_error(
        "Model is not initialized. Call Initialize() first.");
  }
  // Simulation run code
}

void Model::finalize() {
  if (!is_initialized_) {
    throw std::runtime_error("Model is not initialized or already finalized.");
  }
  // Cleanup code
  config_.reset();  // Automatically handled by unique_ptr, but explicitly
                    // showing intent
}
