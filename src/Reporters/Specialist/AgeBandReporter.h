/*
* AgeBandReporter.h
 *
 * This reporter is intended to be used during model calibration and validation
 * and reports the age-banded PfPR during the last year of the given simulation
 * to an SQLite database.
 */
#ifndef AGEBANDREPORTER_H
#define AGEBANDREPORTER_H

#include <vector>
#include <sqlite3.h>
#include "Reporters/Reporter.h"

class AgeBandReporter : public Reporter {
public:
  //disallow copy and move
  AgeBandReporter(const AgeBandReporter &) = delete;
  void operator=(const AgeBandReporter &) = delete;
  AgeBandReporter(AgeBandReporter &&) = delete;
  void operator=(AgeBandReporter &&) = delete;

private:
  // When to start logging the data
  int start_recording = -1;

  // Mapping of the locations to their districts
  std::vector<int> district_lookup;

  // SQLite database connection
  sqlite3* db;

public:
  AgeBandReporter() = default;
  ~AgeBandReporter() override;

  void before_run() override {}
  void begin_time_step() override {}
  void initialize(int job_number, const std::string &path) override;
  void after_run() override;
  void monthly_report() override;
};

#endif
