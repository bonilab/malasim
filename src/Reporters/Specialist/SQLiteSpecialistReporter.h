#ifndef SQLITESPECIALISTREPORTER_H
#define SQLITESPECIALISTREPORTER_H


#include "Helpers/SQLiteDatabase.h"
#include "Reporters/Reporter.h"

class SQLiteSpecialistReporter : public Reporter {
  /*
   * For MovementReporter
   */
private:
  const std::string insert_fine_move_query_ =
      "INSERT INTO Movement (ReplicateId, Timestep, IndividualId, Source, Destination) VALUES (?, ?, ?, ?, ?);";

  const std::string insert_coarse_move_query_ =
      "INSERT INTO DistrictMovement (ReplicateId, Timestep, Count, Source, Destination) VALUES (?, ?, ?, ?, ?);";

protected:
  std::unique_ptr<SQLiteDatabase> db;
  void populate_db_schema();

public:
  /*
   * For MovementReporter
   */
  void insert_movement(const std::vector<std::string>& fine_move, bool fine);
  void insert_district_movement(const std::vector<std::vector<std::string>>& coarse_moves);

public:
  // Constructor and destructor
  SQLiteSpecialistReporter() = default;
  SQLiteSpecialistReporter(const SQLiteSpecialistReporter &) = delete;
  SQLiteSpecialistReporter(SQLiteSpecialistReporter &&) = delete;
  SQLiteSpecialistReporter &operator=(const SQLiteSpecialistReporter &) = delete;
  SQLiteSpecialistReporter &operator=(SQLiteSpecialistReporter &&) = delete;
  ~SQLiteSpecialistReporter() override = default;

  // Initialize the reporter with job number and path
  void initialize() override;

  // Basic declarations for before run and begin time step
  void before_run() override {}
  void begin_time_step() override {}

  // Overridden functions for monthly reporting and post-run cleanup
  void monthly_report() override{};
  // With the new Database class, the database connection will automatically be
  // closed when the Database object is destroyed. Therefore, no explicit action
  // is required here. If there are other cleanup actions to perform, they can
  // be added here.
  void after_run() override {}
};



#endif //SQLITESPECIALISTREPORTER_H
