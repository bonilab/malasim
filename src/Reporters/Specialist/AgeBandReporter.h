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
#include <memory>
#include <sstream>

#include "Reporters/Reporter.h"

namespace spdlog {
class logger;
}

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

  // String streams to use when working with the loggers
  std::stringstream pfpr;
  std::stringstream cases;

  // Mapping of the locations to their districts
  std::vector<int> district_lookup;

  std::shared_ptr<spdlog::logger> pfpr_logger;
  std::shared_ptr<spdlog::logger> cases_logger;

public:
  AgeBandReporter() = default;

  ~AgeBandReporter() override = default;

  void before_run() override {}

  void begin_time_step() override {}

  void initialize(int job_number, const std::string &path) override;

  void after_run() override {}

  void monthly_report() override;
};

#endif
