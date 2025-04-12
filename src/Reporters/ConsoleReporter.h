#ifndef CONSOLEREPORTER_H
#define    CONSOLEREPORTER_H

#include "Reporter.h"

class ConsoleReporter : public Reporter {
public:
  // Disallow copy
  ConsoleReporter(const ConsoleReporter&) = delete;
  ConsoleReporter& operator=(const ConsoleReporter&) = delete;

  // Disallow move
  ConsoleReporter(ConsoleReporter&&) = delete;
  ConsoleReporter& operator=(ConsoleReporter&&) = delete;

 public:
  ConsoleReporter();

  //    ConsoleReporter(const ConsoleReporter& orig);
  virtual ~ConsoleReporter();

  virtual void initialize(int job_number, const std::string &path) {}

  virtual void before_run();

  virtual void after_run();

  virtual void begin_time_step();

  virtual void monthly_report();

 private:

};

#endif    /* CONSOLEREPORTER_H */

