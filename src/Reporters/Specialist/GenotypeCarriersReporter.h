#ifndef GENOTYPECARRIERSREPORTER_H
#define GENOTYPECARRIERSREPORTER_H

#include <sqlite3.h>  // SQLite3 header
#include <memory>
#include <string>
#include "SpecialistReporterBase.h"

class GenotypeCarriersReporter : public SpecialistReporterBase {
private:
  sqlite3* db;  // SQLite3 database connection

  const std::string CREATE_TABLE =
      "CREATE TABLE IF NOT EXISTS monthlysitedata ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "monthlydataid INTEGER, "
      "location INTEGER, "
      "genotypecarriers INTEGER"
      ");";

  const std::string CHECK_COLUMN =
      "PRAGMA table_info(monthlysitedata);";

  const std::string INSERT_GENOTYPE_CARRIERS =
      "INSERT INTO monthlysitedata (monthlydataid, location, genotypecarriers) "
      "VALUES (?, ?, ?) "
      "ON CONFLICT(monthlydataid, location) DO UPDATE SET genotypecarriers = ?;";

  const std::string SELECT_LAST_ID =
      "SELECT last_insert_rowid();";

  // The genotype we are looking for
  const std::string GENOTYPE = "580Y / 561H";
  const int LOCUS = 2;
  const int ALLELE = 1;

public:
  GenotypeCarriersReporter();
  ~GenotypeCarriersReporter() = default;

  // Basic declarations
  void before_run() override {}
  void after_run() override;
  void begin_time_step() override {}

  // Overrides
  void initialize(int job_number, const std::string &path) override;
  void monthly_report() override;
};

#endif  // GENOTYPECARRIERSREPORTER_H
