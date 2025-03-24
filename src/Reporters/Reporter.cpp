#include "Reporter.h"
// #include "ConsoleReporter.h"
#include "ConsoleReporter.h"
#include "MMCReporter.h"
#include "Simulation/Model.h"
#include "MonthlyReporter.h"
#include "NovelDrugReporter.h"
#include "SQLiteMonthlyReporter.h"
#include "TACTReporter.h"
#include "ValidationReporter.h"
#include "Configuration/Config.h"
#include "Specialist/AgeBandReporter.h"
#include "Specialist/CellularReporter.h"
#include "Specialist/PopulationReporter.h"
#include "Specialist/SeasonalImmunity.h"

std::map<std::string, Reporter::ReportType> Reporter::ReportTypeMap{
    {"Console",         CONSOLE},
    {"MonthlyReporter", MONTHLY_REPORTER},
    {"MMC",             MMC_REPORTER},
    {"TACT",            TACT_REPORTER},
    {"NovelDrug",       NOVEL_DRUG_REPOTER},
    {"ValidationReporter",       VALIDATION_REPORTER},
    {"PopulationReporter", POPULATION_REPORTER},
    {"CellularReporter", CELLULAR_REPORTER},
    {"SeasonalImmunity", SEASONAL_IMMUNITY},
    {"AgeBand", AGE_BAND_REPORTER},
    {"SQLiteMonthlyReporter", SQLITE_MONTHLY_REPORTER},
#ifdef ENABLE_TRAVEL_TACKING
        {"TravelTrackingReporter", TRAVEL_TRACKING_REPORTER},
#endif
};

Reporter::Reporter() : model_(nullptr) {
}

Reporter::~Reporter() = default;

Reporter* Reporter::MakeReport(ReportType report_type) {
  switch (report_type) {
  case CONSOLE:
    return new ConsoleReporter();
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
  case POPULATION_REPORTER:
    return new PopulationReporter();
  case CELLULAR_REPORTER:
    return new CellularReporter();
  case SEASONAL_IMMUNITY:
    return new SeasonalImmunity();
  case AGE_BAND_REPORTER:
    return new AgeBandReporter();
  case SQLITE_MONTHLY_REPORTER: {
    auto cell_level_reporting = Model::get_config()->get_model_settings().get_cell_level_reporting();
    return new SQLiteMonthlyReporter(cell_level_reporting);
  }
#ifdef ENABLE_TRAVEL_TRACKING
    case TRAVEL_TRACKING_REPORTER:
      return new TravelTrackingReporter();
#endif
  default:
    return new MonthlyReporter();
  }
}