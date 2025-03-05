/*
 * TherapyRecordReporter.cpp
 *
 * Implement the TherapyRecordReporter class using SQLite.
 */
#include "TherapyRecordReporter.h"

#include "Configuration/Config.h"
#include "MDC/ModelDataCollector.h"
#include "Simulation/Model.h"
#include "sqlite3.h"
#include "Core/Scheduler/Scheduler.h"

void TherapyRecordReporter::initialize(int job_number, const std::string &path) {
  // Open SQLite database
  std::string db_filename = fmt::format("{}therapy_record_{}.db", path, job_number);
  if (sqlite3_open(db_filename.c_str(), &db) != SQLITE_OK) {
    throw std::runtime_error("Failed to open SQLite database");
  }

  // Create table
  char* err_msg;
  if (sqlite3_exec(db, CREATE_TABLE_QUERY.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
    sqlite3_free(err_msg);
    throw std::runtime_error("Failed to create database table");
  }
}

void TherapyRecordReporter::monthly_report() {
  if (!Model::get_mdc()->recording_data()) { return; }

  // Get the monthly data ID
  sqlite3_stmt* stmt;
  const char* select_query = "SELECT id FROM monthly_data ORDER BY dayselapsed DESC LIMIT 1;";
  if (sqlite3_prepare_v2(db, select_query, -1, &stmt, nullptr) != SQLITE_OK) {
    throw std::runtime_error("Failed to prepare SQLite select statement");
  }

  int monthly_data_id = -1;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    monthly_data_id = sqlite3_column_int(stmt, 0);
  }
  sqlite3_finalize(stmt);
  if (monthly_data_id == -1) { return; }

  // Insert treatment records
  if (sqlite3_prepare_v2(db, INSERT_THERAPY_QUERY.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    throw std::runtime_error("Failed to prepare SQLite insert statement");
  }

  auto locations = Model::get_config()->get_spatial_settings().get_number_of_locations();
  auto therapies = Model::get_config()->get_therapy_parameters().therapy_db.size();

  for (int location = 0; location < locations; location++) {
    for (int therapy = 0; therapy < therapies; therapy++) {
      sqlite3_bind_int(stmt, 1, monthly_data_id);
      sqlite3_bind_int(stmt, 2, location);
      sqlite3_bind_int(stmt, 3, therapy);
      sqlite3_bind_int(stmt, 4, Model::get_mdc()->monthly_treatment_success_by_location_therapy()[location][therapy]);
      sqlite3_bind_int(stmt, 5, Model::get_mdc()->monthly_treatment_failure_by_location_therapy()[location][therapy]);
      sqlite3_bind_int(stmt, 6, Model::get_mdc()->monthly_treatment_complete_by_location_therapy()[location][therapy]);

      if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to insert data into SQLite database");
      }
      sqlite3_reset(stmt);
    }
  }
  sqlite3_finalize(stmt);
}

void TherapyRecordReporter::after_run() {
  if (db) {
    sqlite3_close(db);
  }
}

TherapyRecordReporter::~TherapyRecordReporter() {
  if (db) {
    sqlite3_close(db);
  }
}
