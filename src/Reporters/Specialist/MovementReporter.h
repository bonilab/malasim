#ifndef MOVEMENTREPORTER_H
#define MOVEMENTREPORTER_H

#include <sqlite3.h>
#include <vector>
#include <string>
#include "Reporters/Reporter.h"

class MovementReporter : public Reporter {
private:
  sqlite3* db = nullptr;

  const std::string CREATE_FINE_MOVEMENT_TABLE =
      "CREATE TABLE IF NOT EXISTS Movement ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "ReplicateId INTEGER, "
      "Timestep INTEGER, "
      "IndividualId INTEGER, "
      "Source INTEGER, "
      "Destination INTEGER"
      ");";

  const std::string CREATE_COARSE_MOVEMENT_TABLE =
      "CREATE TABLE IF NOT EXISTS DistrictMovement ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "ReplicateId INTEGER, "
      "Timestep INTEGER, "
      "Count INTEGER, "
      "Source INTEGER, "
      "Destination INTEGER"
      ");";

  const std::string INSERT_FINE_MOVE =
      "INSERT INTO Movement (ReplicateId, Timestep, IndividualId, Source, Destination) "
      "VALUES (?, ?, ?, ?, ?);";

  const std::string INSERT_COARSE_MOVE =
      "INSERT INTO DistrictMovement (ReplicateId, Timestep, Count, Source, Destination) "
      "VALUES (?, ?, ?, ?, ?);";

  int count = 0;
  int replicate = 0;
  std::string db_path;

  int division_count = 0;
  int** movement_counts = nullptr;

public:
  MovementReporter() = default;
  ~MovementReporter() override;

  void initialize(int job_number, const std::string &path) override;
  void before_run() override {}
  void begin_time_step() override {}
  void monthly_report() override;
  void after_run() override;

  void coarse_report();
  void add_coarse_move(int individual, int source, int destination);
  void add_fine_move(int individual, int source, int destination);
  void fine_report();
};

#endif  // MOVEMENTREPORTER_H
