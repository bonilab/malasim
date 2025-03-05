#include "MovementReporter.h"

#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Random.h"
#include "Simulation/Model.h"
#include "Utils/Cli.hxx"
#include "spdlog/spdlog.h"

#define COUNT_LIMIT 100

MovementReporter::~MovementReporter() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        spdlog::info("SQLite3 database closed.");
    }
}

void MovementReporter::initialize(int job_number, const std::string &path) {
    db_path = fmt::format("{}/movement_report_{}.db", path, job_number);

    // Open SQLite3 database
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open SQLite database: " + std::string(sqlite3_errmsg(db)));
    }

    // Create tables if they do not exist
    char* err_msg = nullptr;
    if (sqlite3_exec(db, CREATE_FINE_MOVEMENT_TABLE.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK ||
        sqlite3_exec(db, CREATE_COARSE_MOVEMENT_TABLE.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string error_msg = "Failed to create tables: " + std::string(err_msg);
        sqlite3_free(err_msg);
        throw std::runtime_error(error_msg);
    }

    replicate = utils::Cli::get_instance().get_replicate();
    if (replicate == 0) {
        spdlog::error("Replicate value of zero, expected one or higher.");
        throw std::runtime_error("Invalid replicate value");
    }

    division_count = Model::get_config()->get_spatial_settings().get_number_of_locations();
    movement_counts = new int*[division_count];
    for (int i = 0; i < division_count; i++) {
        movement_counts[i] = new int[division_count]{0};
    }

    spdlog::info("MovementReporter initialized with SQLite3 at {}", db_path);
}

void MovementReporter::add_fine_move(int individual, int source, int destination) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, INSERT_FINE_MOVE.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        spdlog::error("Failed to prepare INSERT_FINE_MOVE statement: {}", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, replicate);
    sqlite3_bind_int(stmt, 2, Model::get_scheduler()->current_time());
    sqlite3_bind_int(stmt, 3, individual);
    sqlite3_bind_int(stmt, 4, source);
    sqlite3_bind_int(stmt, 5, destination);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        spdlog::error("Failed to insert fine movement: {}", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);

    count++;
    if (count >= COUNT_LIMIT) {
        fine_report();
    }
}

void MovementReporter::add_coarse_move(int individual, int source, int destination) {
    movement_counts[source][destination]++;
}

void MovementReporter::coarse_report() {
  auto timestep = Model::get_scheduler()->current_time();
  if (timestep == 0) return;

  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, INSERT_COARSE_MOVE.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    spdlog::error("Failed to prepare INSERT_COARSE_MOVE statement: {}", sqlite3_errmsg(db));
    return;
  }

  for (int source = 0; source < division_count; source++) {
    for (int destination = 0; destination < division_count; destination++) {
      if (movement_counts[source][destination] == 0) continue;

      sqlite3_reset(stmt);
      sqlite3_bind_int(stmt, 1, replicate);
      sqlite3_bind_int(stmt, 2, timestep);
      sqlite3_bind_int(stmt, 3, movement_counts[source][destination]);
      sqlite3_bind_int(stmt, 4, source);
      sqlite3_bind_int(stmt, 5, destination);

      if (sqlite3_step(stmt) != SQLITE_DONE) {
        spdlog::error("Failed to insert coarse movement: {}", sqlite3_errmsg(db));
      }

      movement_counts[source][destination] = 0;
    }
  }

  sqlite3_finalize(stmt);
}

void MovementReporter::fine_report() {
  count = 0;  // Reset fine move counter
}

void MovementReporter::monthly_report() {
  if (movement_counts != nullptr) coarse_report();
  if (count > 0) fine_report();
}

void MovementReporter::after_run() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
    spdlog::info("SQLite3 database closed.");
  }

  if (movement_counts) {
    for (int i = 0; i < division_count; i++) {
      delete[] movement_counts[i];
    }
    delete[] movement_counts;
    movement_counts = nullptr;
  }
}