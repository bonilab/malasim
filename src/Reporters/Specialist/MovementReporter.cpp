/*
 * MovementReporter.cpp
 *
 * Implementation of the MovementReporter class.
 *
 * NOTE Since we assume this class will mostly be used for testing purposes,
 * there is an assumed dependency on DbReporter having already initialized the
 * database.
 *
 * NOTE This code could be significantly optimized; however, since it is only
 * used for the initial model validation it hasn't been tuned that much.
 */
#include "MovementReporter.h"
#include "MovementReporter.h"

#include "SQLiteSpecialistReporter.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Utils/Cli.hxx"
#include "Spatial/GIS/SpatialData.h"

#define COUNT_LIMIT 100

void MovementReporter::initialize(int job_number, const std::string &path, SQLiteSpecialistReporter* db) {
  db_reporter = db;

  replicate = utils::Cli::get_instance().get_replicate();
  if (replicate == 0) {
    spdlog::error("Replicate value of zero, expected one or higher.");
    throw std::runtime_error("Invalid replicate value");
  }

  if (utils::Cli::get_instance().get_record_district_movement()) {
    division_count = SpatialData::get_instance().get_district_count();
    if (division_count == -1) {
      spdlog::debug("District not loaded");
      return;
    }
    division_count++;
  } else {
    division_count = static_cast<int>(Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations());
  }

  movement_counts = new int*[division_count];
  for (auto ndx = 0; ndx < division_count; ndx++) {
    movement_counts[ndx] = new int[division_count]();
  }

  spdlog::info("MovementReporter initialized with replicate {}", replicate);
}

void MovementReporter::add_fine_move(int individual, int source, int destination) {
  std::vector<std::string> fine_move = {
      std::to_string(replicate),
      std::to_string(Model::get_instance().get_scheduler()->current_time()),
      std::to_string(individual),
      std::to_string(source),
      std::to_string(destination)
  };

  db_reporter->insert_movement(fine_move, true);

  count++;
  if (count >= COUNT_LIMIT) { fine_report(); }
}

void MovementReporter::add_coarse_move(int individual, int source, int destination) {
  movement_counts[source][destination]++;
}

void MovementReporter::coarse_report() {
  auto timestep = Model::get_instance().get_scheduler()->current_time();
  if (timestep == 0) return;

  std::vector<std::vector<std::string>> coarse_moves;

  for (auto source = 0; source < division_count; source++) {
    for (auto destination = 0; destination < division_count; destination++) {
      if (movement_counts[source][destination] == 0) continue;

      coarse_moves.push_back({
          std::to_string(replicate),
          std::to_string(timestep),
          std::to_string(movement_counts[source][destination]),
          std::to_string(source),
          std::to_string(destination)
      });

      movement_counts[source][destination] = 0;
    }
  }

  if (!coarse_moves.empty()) {
    db_reporter->insert_district_movement(coarse_moves);
  } else {
    spdlog::warn("No movement recorded.");
  }
}

void MovementReporter::fine_report() {
  count = 0;
}

void MovementReporter::monthly_report() {
  coarse_report();
}

void MovementReporter::after_run() {
  for (int i = 0; i < division_count; i++) {
    delete[] movement_counts[i];
  }
  delete[] movement_counts;
}

