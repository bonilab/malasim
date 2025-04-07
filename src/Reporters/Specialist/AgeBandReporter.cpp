/*
 * AgeBandReporter.cpp
 *
 * Implementation of the AgeBandReporter class using SQLite.
 */
#include "AgeBandReporter.h"
#include "Configuration/Config.h"
#include "MDC/ModelDataCollector.h"
#include "Simulation/Model.h"
#include "Core/Scheduler/Scheduler.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void AgeBandReporter::initialize(int job_number, const std::string &path) {
  // Setup spdlog multi-sink loggers
  auto pfpr_file = fmt::format("{}age_band_pfpr_{}.csv", path, job_number);
  auto cases_file = fmt::format("{}age_band_cases_{}.csv", path, job_number);

  auto pfpr_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(pfpr_file, true);
  auto pfpr_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  pfpr_logger = std::make_shared<spdlog::logger>("age_band_pfpr", spdlog::sinks_init_list{pfpr_file_sink, pfpr_console_sink});
  pfpr_logger->set_pattern("%v");
  spdlog::register_logger(pfpr_logger);

  auto cases_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(cases_file, true);
  auto cases_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  cases_logger = std::make_shared<spdlog::logger>("age_band_cases", spdlog::sinks_init_list{cases_file_sink, cases_console_sink});
  cases_logger->set_pattern("%v");
  spdlog::register_logger(cases_logger);

  // Determine when to start reporting
  start_recording = Model::get_config()->get_simulation_timeframe().get_total_time();
  start_recording -= 366;

  spdlog::info("Logging of age-banded blood slide prevalence will start at model day {}", start_recording);

  // Build district lookup
  for (auto loc = 0; loc < Model::get_config()->number_of_locations(); loc++) {
    district_lookup.emplace_back(
        Model::get_spatial_data()->get_admin_unit("district", loc));
  }

  // Log headers
  pfpr << "ModelDays" << Csv::sep << "District" << Csv::sep;
  cases << "ModelDays" << Csv::sep << "District" << Csv::sep;
  for (auto ac = 0; ac < Model::get_config()->number_of_age_classes(); ac++) {
    pfpr << Model::get_config()->get_population_demographic().get_age_structure()[ac] << Csv::sep;
    auto band = Model::get_config()->get_population_demographic().get_age_structure()[ac];
    cases << "cases_" << band << Csv::sep << "pop_" << band << Csv::sep;
  }
  pfpr << Csv::end_line;
  cases << Csv::end_line;
  pfpr_logger->info(pfpr.str());
  cases_logger->info(cases.str());
  pfpr.str("");
  cases.str("");
}

void AgeBandReporter::monthly_report() {
  auto current_time = Model::get_scheduler()->current_time();
  if (current_time < start_recording) { return; }

  auto age_classes = Model::get_config()->number_of_age_classes();
  auto districts = Model::get_spatial_data()->get_boundary("district")->unit_count;

  std::vector<std::vector<int>> population(districts, std::vector<int>(age_classes));
  std::vector<std::vector<double>> prevalence(districts, std::vector<double>(age_classes));

  for (auto loc = 0; loc < Model::get_config()->number_of_locations(); loc++) {
    auto district = Model::get_spatial_data()->get_boundary("district")->min_unit_id == 0
    ? district_lookup[loc] : district_lookup[loc] - 1;
    for (auto ac = 0; ac < age_classes; ac++) {
      population[district][ac] += Model::get_mdc()->popsize_by_location_age_class()[loc][ac];
      prevalence[district][ac] += Model::get_mdc()->blood_slide_number_by_location_age_group()[loc][ac];
    }
  }

  for (auto district = 0; district < districts; district++) {
    pfpr << current_time << Csv::sep << district << Csv::sep;
    cases << current_time << Csv::sep << district << Csv::sep;
    for (auto ac = 0; ac < age_classes; ac++) {
      pfpr << ((prevalence[district][ac] != 0)
                   ? (prevalence[district][ac] / population[district][ac]) * 100.0
                   : 0)
           << Csv::sep;
      cases << prevalence[district][ac] << Csv::sep << population[district][ac] << Csv::sep;
    }
    pfpr << Csv::end_line;
    cases << Csv::end_line;
  }

  pfpr_logger->info(pfpr.str());
  cases_logger->info(cases.str());
  pfpr.str("");
  cases.str("");
}

