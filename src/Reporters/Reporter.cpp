#include "Reporter.h"
// #include "ConsoleReporter.h"
#include "MMCReporter.h"
#include "Simulation/Model.h"
#include "MonthlyReporter.h"
#include "NovelDrugReporter.h"
#include "SQLiteDistrictReporter.h"
#include "SQLitePixelReporter.h"
#include "TACTReporter.h"
#include "ValidationReporter.h"
// #include "MMCReporter.h"
// #include "TACTReporter.h"
// #include "NovelDrugReporter.h"
// #include "ValidationReporter.h"

std::map<std::string, Reporter::ReportType> Reporter::ReportTypeMap{
    {"Console",         CONSOLE},
    {"MonthlyReporter", MONTHLY_REPORTER},
    {"MMC",             MMC_REPORTER},
    {"TACT",            TACT_REPORTER},
    {"NovelDrug",       NOVEL_DRUG_REPOTER},
    {"ValidationReporter",       VALIDATION_REPORTER},
    {"SQLiteDistrictReporter", SQLITE_DISTRICT_REPORTER},
    {"SQLitePixelReporter", SQLITE_PIXEL_REPORTER},
#ifdef ENABLE_TRAVEL_TACKING
        {"TravelTrackingReporter", TRAVEL_TRACKING_REPORTER},
#endif
};

Reporter::Reporter() : model_(nullptr) {
}

Reporter::~Reporter() = default;

Reporter* Reporter::MakeReport(ReportType report_type) {
  switch (report_type) {
  //   case CONSOLE:
  //     return new ConsoleReporter();
  case MONTHLY_REPORTER:
    return new MonthlyReporter();
  case MMC_REPORTER:
    return new MMCReporter();
  case TACT_REPORTER:
    return new TACTReporter();
  case NOVEL_DRUG_REPOTER:
    return new NovelDrugReporter();
  case VALIDATION_REPORTER:
    return new ValidationReporter();
  case SQLITE_DISTRICT_REPORTER:
    return new SQLiteDistrictReporter();
  case SQLITE_PIXEL_REPORTER:
    return new SQLitePixelReporter();
  default:
  return new MonthlyReporter();
  }
}