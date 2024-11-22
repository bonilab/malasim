#include <iostream>
#include <stdexcept>
#include "Model.h"
#include <Core/Scheduler/Scheduler.h>
#include "Configuration/Config.h"  // Assuming Config is defined here
#include "Utils/Cli.h"

// Private constructor: creates the Config instance
Model::Model()
    : config_(std::make_unique<Config>()),
      scheduler_(std::make_unique<Scheduler>(this)), config_file_path_("input.yml"),
      is_initialized_(false) {
}

bool Model::initialize() {
  if (!config_file_path_.empty()) {
    config_file_path_ = utils::Cli::get_instance().get_input_file();
    // Load the configuration file
    if(config_->load(config_file_path_)) {
      std::cout << "Model initialized with configuration file: "
                << config_file_path_ << "\n";
      is_initialized_ = true;
    }
    else {
      throw std::runtime_error("Failed to load configuration file: " + config_file_path_);
    }
    return is_initialized_;
  }
  throw std::invalid_argument("Configuration file path must be provided.");
}

void Model::run() const {
  if (!is_initialized_) {
    throw std::runtime_error(
        "Model is not initialized. Call Initialize() first.");
  }
  scheduler_->run();
}

void Model::finalize() {
  if (!is_initialized_) {
    throw std::runtime_error("Model is not initialized or already finalized.");
  }
  // Cleanup code
  config_.reset();  // Automatically handled by unique_ptr, but explicitly
                    // showing intent
}

void Model::begin_time_step() {
  // spdlog::info("\t\t\tBegin time step");
}/**/

void Model::end_time_step() {
  // spdlog::info("\t\t\tEnd time step");
}

void Model::daily_update() {
  // spdlog::info("\tDaily update");
}

void Model::monthly_update() {
  // spdlog::info("\t\tMonthly update");
}

void Model::yearly_update() {
  // spdlog::info("\tYearly update");
}

