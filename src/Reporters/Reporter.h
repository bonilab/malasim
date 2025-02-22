#ifndef REPORTER_H
#define REPORTER_H

#include <map>

class Model;

class Reporter {
public:
  //disallow copy and assign and move
  Reporter(const Reporter &orig) = delete;
  void operator=(const Reporter &orig) = delete;
  Reporter(Reporter &&orig) = delete;
  void operator=(Reporter &&orig) = delete;

public:
  Model *model{};
  void set_model(Model *value) { model = value; }

 private:
 Model *model_;

public:
  enum ReportType {
    CONSOLE,
    GUI,
    MONTHLY_REPORTER,
    MMC_REPORTER,
    TACT_REPORTER,
    NOVEL_DRUG_REPOTER,
    VALIDATION_REPORTER
  };



 public:
  static std::map<std::string, ReportType> ReportTypeMap;

 public:
  Reporter();

  //    Reporter(const Reporter& orig);
  virtual ~Reporter();

  virtual void initialize() = 0;

  virtual void before_run() = 0;

  virtual void after_run() = 0;

  virtual void begin_time_step() = 0;

  virtual void monthly_report() = 0;

  static Reporter *MakeReport(ReportType report_type);

 private:

};

#endif /* REPORTER_H */
