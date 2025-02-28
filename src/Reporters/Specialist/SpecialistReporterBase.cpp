/*
 * SpecialistReporterBase.cpp
 *
 * Implement the functionality of the extended SpecialistReporterBase class.
 */
#include "SpecialistReporterBase.h"

#include <sqlite3.h>
#include "Configuration/Config.h"
#include "Simulation/Model.h"
#include "Utils/Cli.hxx"
#include "spdlog/spdlog.h"

void SpecialistReporterBase::build_lookup(sqlite3* db) {
  // Start by first checking to make sure we have a valid replicate
  int replicate = utils::Cli::get_instance().get_replicate();
  if (replicate == 0) {
    spdlog::error("Database replicate number does not appear to be set, unable to determine aggregation level");
    throw std::runtime_error("Missing database replicate number");
  }

  // Next check what the current aggregation level is
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, SELECT_AGGREGATION.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    spdlog::error("Failed to prepare aggregation level query");
    throw std::runtime_error("Failed to prepare SQL statement");
  }

  sqlite3_bind_int(stmt, 1, replicate);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    aggregation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  } else {
    spdlog::error("Database did not return the aggregation level for the study");
    throw std::runtime_error("Aggregation level not returned");
  }
  sqlite3_finalize(stmt);

  if (aggregation == "D") {
    // District reporting requires mapping each cell to the district ID
    auto offset = SpatialData::get_instance().get_first_district();
    for (auto loc = 0; loc < Model::get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
      lookup.emplace_back(SpatialData::get_instance().get_raster_district(loc) - offset);
    }
    lookup_allocation = SpatialData::get_instance().get_district_count();
  } else if (aggregation == "C") {
    // Cellular count, loaded from the database
    if (sqlite3_prepare_v2(db, SELECT_LOCATION.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      spdlog::error("Failed to prepare location query");
      throw std::runtime_error("Failed to prepare SQL statement");
    }

    sqlite3_bind_int(stmt, 1, replicate);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      lookup.emplace_back(sqlite3_column_int(stmt, 0));
    }
    lookup_allocation = static_cast<int>(lookup.size());
    sqlite3_finalize(stmt);
  } else {
    spdlog::error("Unknown aggregation level code: {}", aggregation);
    throw std::runtime_error("Unknown aggregation code returned");
  }
}
