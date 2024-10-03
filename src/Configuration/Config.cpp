
#include "Config.h"

#include <yaml-cpp/yaml.h>

#include <mutex>

#include "YAMLConverters.h"

void Config::load(const std::string &filename) {
  std::shared_lock lock(mutex_);
  config_file_path_ = filename;
  YAML::Node config = YAML::LoadFile(filename);
  config_data_.model_settings = config["ModelSettings"].as<ModelSettings>();
  config_data_.transmission_settings =
      config["TransmissionSettings"].as<TransmissionSettings>();
  config_data_.population_demographic =
      config["PopulationDemographic"].as<PopulationDemographic>();
  notify_observers();
}

void Config::reload() { load(config_file_path_); }

void Config::register_observer(ConfigObserver observer) {
  std::unique_lock lock(mutex_);
  observers_.push_back(observer);
}

void Config::notify_observers() {
  for (const auto &observer : observers_) { observer(config_data_); }
}

