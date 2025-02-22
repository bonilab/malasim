#include "SQLiteSpecialistReporter.h"

#include <filesystem>

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Helpers/StringHelpers.h"
#include "MDC/ModelDataCollector.h"
#include "Parasites/Genotype.h"
#include "Simulation/Model.h"
#include "Population/Population.h"
#include "Utils/Cli.hxx"

// Function to create the database schema
// This sets up the necessary tables in the database
void SQLiteSpecialistReporter::populate_db_schema() {
  /*
   * For MovementReporter
   */
  const std::string createMovement = R""""(
    CREATE TABLE IF NOT EXISTS Movement (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        ReplicateId INTEGER NOT NULL,
        Timestep INTEGER NOT NULL,
        IndividualId INTEGER NOT NULL,
        Source INTEGER NOT NULL,
        Destination INTEGER NOT NULL
    );
  )"""";

  const std::string createDistrictMovement = R""""(
    CREATE TABLE IF NOT EXISTS DistrictMovement (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        ReplicateId INTEGER NOT NULL,
        Timestep INTEGER NOT NULL,
        Count INTEGER NOT NULL,
        Source INTEGER NOT NULL,
        Destination INTEGER NOT NULL
    );
  )"""";

  try {
    db->execute(createMovement);
    db->execute(createDistrictMovement);
  }
  catch (const std::exception &ex) {
      spdlog::error("Error in populate_db_schema: {}", ex.what());
  }
}

// Initialize the reporter
// Sets up the database and prepares it for data entry
void SQLiteSpecialistReporter::initialize() {
  spdlog::info("Base SQLiteSpecialistReporter initialized.\n");

  // Define the database file path
  auto dbPath = fmt::format("{}specialist_data_{}.db",
    utils::Cli::get_instance().get_output_path(),
    utils::Cli::get_instance().get_job_number());

  // Check if the file exists
  if (std::filesystem::exists(dbPath)) {
    // Delete the old database file if it exists
    if (std::remove(dbPath.c_str()) != 0) {
      // Handle the error, if any, when deleting the old file
      spdlog::error("Error deleting old database file.");
    }
  } else {
    // The file doesn't exist, so no need to delete it
    spdlog::info("Database file does not exist. No deletion needed.\n");
  }

  // Open or create the SQLite database file
  db = std::make_unique<SQLiteDatabase>(dbPath);

  populate_db_schema();
}

/*
 * For MovementReporter
 */

void SQLiteSpecialistReporter::insert_movement(const std::vector<std::string>& fine_move, bool fine) {
  std::string const query_template = fine ? insert_fine_move_query_ : insert_coarse_move_query_;
  std::string const query = query_template + StringHelpers::join(fine_move, ",") + ";";
  db->execute(query);
}

void SQLiteSpecialistReporter::insert_district_movement(const std::vector<std::vector<std::string>>& coarse_moves) {
  for (const auto& move : coarse_moves) {
    std::string const query = insert_coarse_move_query_ + StringHelpers::join(move, ",") + ";";
    db->execute(query);
  }
}

