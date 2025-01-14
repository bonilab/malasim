#include <iostream>
#include <stdexcept>
#include "Model.h"
#include <Core/Scheduler/Scheduler.h>
#include <Population/Population.h>
#include <Utils/Random.h>

#include "Configuration/Config.h"  // Assuming Config is defined here
#include "Utils/Cli.hxx"

// Private constructor: creates the Config instance
Model::Model()
    : config_(new Config()),
      scheduler_(new Scheduler(this)), config_file_path_("input.yml"),
      population_(new Population(this)),
      random_(new utils::Random()),
      is_initialized_(false) {
}

bool Model::initialize() {
  if (!config_file_path_.empty()) {
    config_file_path_ = utils::Cli::get_instance().get_input_file();
    // Load the configuration file
    if(config_->load(config_file_path_)) {
      spdlog::info("Configuration file loaded successfully: " + config_file_path_);

      if(config_->get_model_settings().get_initial_seed_number() <= 0) {
        random_->set_seed(std::chrono::system_clock::now().time_since_epoch().count());
      } else {
        random_->set_seed(config_->get_model_settings().get_initial_seed_number());
      }
      scheduler_->initialize(config_->get_simulation_timeframe().get_starting_date(),
                             config_->get_simulation_timeframe().get_ending_date());
      population_->initialize();

      is_initialized_ = true;
    }
    else {
      spdlog::error("Failed to load configuration file: " + config_file_path_);
    }
  }
  return is_initialized_;
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
  delete population_;
  delete scheduler_;
  delete config_;
  delete random_;
}

void Model::begin_time_step() {
  // spdlog::info("\t\t\tBegin time step");
}

void Model::end_time_step() {
  // spdlog::info("\t\t\tEnd time step");
}

void Model::daily_update() {
  // spdlog::info("\tDaily update");
  population_->update(scheduler_->current_time());
}

void Model::monthly_update() {
  // spdlog::info("\t\tMonthly update");
}

void Model::yearly_update() {
  // spdlog::info("\tYearly update");
}

utils::Random* Model::get_random() {
  return random_;
}

