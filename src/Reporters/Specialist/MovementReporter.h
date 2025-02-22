/*
 * MovementReporter.h
 *
 * Define the MovementReporter class which is used to insert movement
 * information into the database. When running for large populations over long
 * periods of time, this will be a bit quicker since updates are buffered until
 * the end of the time step.
 */
#ifndef MOVEMENTREPORTER_H
#define MOVEMENTREPORTER_H

#include "Reporters/SQLiteDbReporter.h"

class SQLiteSpecialistReporter;

class MovementReporter : public SQLiteDbReporter {
private:
  const std::string INSERT_FINE_MOVE =
      "INSERT INTO Movement (ReplicateId, Timestep, IndividualId, Source, Destination) VALUES (?, ?, ?, ?, ?);";

  const std::string INSERT_COARSE_MOVE =
      "INSERT INTO DistrictMovement (ReplicateId, Timestep, Count, Source, Destination) VALUES (?, ?, ?, ?, ?);";

  int count = 0;
  int replicate = 0;
  std::string fine_update_query;

  int division_count = 0;
  int** movement_counts = nullptr;

  SQLiteSpecialistReporter* db_reporter = nullptr;  // SQLite database reporter

public:
  MovementReporter() = default;
  ~MovementReporter() override = default;

  void initialize(int job_number, const std::string &path, SQLiteSpecialistReporter* db);
  void monthly_report() override;
  void after_run() override;

  void add_coarse_move(int individual, int source, int destination);
  void add_fine_move(int individual, int source, int destination);
  void coarse_report();
  void fine_report();
};

#endif

