/*
 * AgeBandReporter.cpp
 *
 * Implementation of the AgeBandReporter class using SQLite.
 */
#include "AgeBandReporter.h"

#include "Configuration/Config.h"
#include "MDC/ModelDataCollector.h"
#include "Simulation/Model.h"
#include "sqlite3.h"
#include "Core/Scheduler/Scheduler.h"

void AgeBandReporter::initialize(int job_number, const std::string &path) {
  // Open SQLite database
  std::string db_filename = fmt::format("{}age_band_report_{}.db", path, job_number);
  if (sqlite3_open(db_filename.c_str(), &db) != SQLITE_OK) {
    throw std::runtime_error("Failed to open SQLite database");
  }

  // Create tables
  const char* create_table_query =
      "CREATE TABLE IF NOT EXISTS age_band_report ("
      "model_days INTEGER,"
      "district INTEGER,"
      "age_band INTEGER,"
      "cases INTEGER,"
      "population INTEGER"
      ");";


  char* err_msg;
  if (sqlite3_exec(db, create_table_query, nullptr, nullptr, &err_msg) != SQLITE_OK) {
    sqlite3_free(err_msg);
    throw std::runtime_error("Failed to create database table");
  }

  // Determine when we should start running
  start_recording = Model::get_config()->get_simulation_timeframe().get_total_time() - 366;
}

void AgeBandReporter::monthly_report() {
  auto current_time = Model::get_scheduler()->current_time();
  if (current_time < start_recording) { return; }

  auto age_classes = Model::get_config()->get_population_demographic().get_number_of_age_classes();
  auto districts = SpatialData::get_instance().max_district_id + 1;
  auto first_index = SpatialData::get_instance().get_first_district();
  std::vector<std::vector<int>> population(districts, std::vector<int>(age_classes));
  std::vector<std::vector<int>> cases(districts, std::vector<int>(age_classes));

  for (auto loc = 0; loc < Model::get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    for (auto ac = 0; ac < Model::get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
      auto district = SpatialData::get_instance().get_district(loc);
      population[district][ac] += Model::get_mdc()->popsize_by_location_age_class()[loc][ac];
      cases[district][ac] += Model::get_mdc()->blood_slide_number_by_location_age_group()[loc][ac];
    }
  }

  sqlite3_stmt* stmt;
  const char* insert_query = "INSERT INTO age_band_report VALUES (?, ?, ?, ?, ?);";
  if (sqlite3_prepare_v2(db, insert_query, -1, &stmt, nullptr) != SQLITE_OK) {
    throw std::runtime_error("Failed to prepare SQLite insert statement");
  }

  for (auto district = 0; district < districts; district++) {
    for (auto ac = 0; ac < age_classes; ac++) {
      sqlite3_bind_int(stmt, 1, current_time);
      sqlite3_bind_int(stmt, 2, district + first_index);
      sqlite3_bind_int(stmt, 3, ac);
      sqlite3_bind_int(stmt, 4, cases[district][ac]);
      sqlite3_bind_int(stmt, 5, population[district][ac]);

      if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to insert data into SQLite database");
      }
      sqlite3_reset(stmt);
    }
  }
  sqlite3_finalize(stmt);
}

void AgeBandReporter::after_run() {
  if (db) {
    sqlite3_close(db);
  }
}

AgeBandReporter::~AgeBandReporter() {
  if (db) {
    sqlite3_close(db);
  }
}
