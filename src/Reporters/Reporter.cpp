#include "Reporter.h"
// #include "ConsoleReporter.h"
#include "ConsoleReporter.h"
#include "MMCReporter.h"
#include "Simulation/Model.h"
#include "MonthlyReporter.h"
#include "NovelDrugReporter.h"
#include "SQLiteDistrictReporter.h"
#include "SQLitePixelReporter.h"
#include "TACTReporter.h"
#include "ValidationReporter.h"
#include "Specialist/AgeBandReporter.h"
#include "Specialist/CellularReporter.h"
#include "Specialist/GenotypeCarriersReporter.h"
#include "Specialist/MovementReporter.h"
#include "Specialist/PopulationReporter.h"
#include "Specialist/SeasonalImmunity.h"
#include "Specialist/TherapyRecordReporter.h"

std::map<std::string, Reporter::ReportType> Reporter::ReportTypeMap{
    {"Console",         CONSOLE},
    {"MonthlyReporter", MONTHLY_REPORTER},
    {"MMC",             MMC_REPORTER},
    {"TACT",            TACT_REPORTER},
    {"NovelDrug",       NOVEL_DRUG_REPOTER},
    {"ValidationReporter",       VALIDATION_REPORTER},
    {"MovementReporter", MOVEMENT_REPORTER},
    {"PopulationReporter", POPULATION_REPORTER},
    {"CellularReporter", CELLULAR_REPORTER},
    {"GenotypeCarriers", GENOTYPE_CARRIERS},
    {"SeasonalImmunity", SEASONAL_IMMUNITY},
    {"AgeBand", AGE_BAND_REPORTER},
    {"TherapyRecordReporter", THERAPY_RECORD_REPORTER},
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
  case MOVEMENT_REPORTER:
    return new MovementReporter();
  case POPULATION_REPORTER:
    return new PopulationReporter();
  case CELLULAR_REPORTER:
    return new CellularReporter();
  case GENOTYPE_CARRIERS:
    return new GenotypeCarriersReporter();
  case SEASONAL_IMMUNITY:
    return new SeasonalImmunity();
  case AGE_BAND_REPORTER:
    return new AgeBandReporter();
  case THERAPY_RECORD_REPORTER:
    return new TherapyRecordReporter();
  case SQLITE_DISTRICT_REPORTER:
    return new SQLiteDistrictReporter();
  case SQLITE_PIXEL_REPORTER:
    return new SQLitePixelReporter();
#ifdef ENABLE_TRAVEL_TRACKING
    case TRAVEL_TRACKING_REPORTER:
      return new TravelTrackingReporter();
#endif
  default:
    return new MonthlyReporter();
  }
}