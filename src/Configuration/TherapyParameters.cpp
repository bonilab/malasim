#include "TherapyParameters.h"

#include "Simulation/Model.h"

void TherapyParameters::process_config() {
  spdlog::info("Processing TherapyParameters");

  for (std::size_t i = 0; i < node_.size(); i++) {
    auto therapy = read_therapy(node_, static_cast<int>(i));
    Model::get_therapy_db().push_back(std::move(therapy));
  }
}

