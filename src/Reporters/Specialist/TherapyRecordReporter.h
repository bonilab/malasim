/*
* TherapyRecordReporter.h
 *
 * Define the TherapyRecordReporter class which is used to capture information
 * related to the number of treatments completed each month by location and
 * therapy.
 */
#ifndef THERAPYRECORDREPORTER_H
#define THERAPYRECORDREPORTER_H

#include <sqlite3.h>
#include "SpecialistReporterBase.h"

class TherapyRecordReporter : public SpecialistReporterBase {
private:
  // SQLite database connection
  sqlite3* db;

  const std::string CREATE_TABLE_QUERY =
      "CREATE TABLE IF NOT EXISTS therapy_record ("
      "MonthlyDataId INTEGER,"
      "LocationId INTEGER,"
      "TherapyId INTEGER,"
      "Success INTEGER,"
      "Failure INTEGER,"
      "Completed INTEGER"
      ");";

  const std::string INSERT_THERAPY_QUERY =
      "INSERT INTO therapy_record (MonthlyDataId, LocationId, TherapyId, Success, Failure, Completed)"
      " VALUES (?, ?, ?, ?, ?, ?);";

  void district_report(int id);

public:
  TherapyRecordReporter() = default;
  ~TherapyRecordReporter() override;

  // Basic declarations
  void before_run() override {}
  void after_run() override;
  void begin_time_step() override {}

  // Overrides
  void initialize(int job_number, const std::string &path) override;
  void monthly_report() override;
};

#endif
