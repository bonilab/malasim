//
// Created by Nguyen Tran on 3/5/2018.
//

#include "MonthlyReporter.h"

#include <date/date.h>

#include "Utils/Constants.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Random.h"
#include "MDC/ModelDataCollector.h"
#include "Parasites/Genotype.h"
#include "Simulation/Model.h"
#include "Population/Population.h"
#include "Treatment/ITreatmentCoverageModel.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Utility/ReporterUtils.h"
#include "Treatment/Strategies/IStrategy.h"
#include "Utils/Cli.hxx"

MonthlyReporter::MonthlyReporter() = default;

MonthlyReporter::~MonthlyReporter() = default;

void MonthlyReporter::initialize(int job_number, const std::string &path) {
  gene_freq_file.open(fmt::format("{}/gene_freq_{}.txt", path, job_number));
  monthly_data_file.open(fmt::format("{}/monthly_data_{}.txt", path, job_number));
  summary_data_file.open(fmt::format("{}/summary_{}.txt", path, job_number));
  gene_db_file.open(fmt::format("{}/gene_db_{}.txt", path, job_number));
}

void MonthlyReporter::before_run() {}

void MonthlyReporter::begin_time_step() {}

void MonthlyReporter::monthly_report() {
    std::stringstream ss;

    ss << Model::get_instance().get_scheduler()->current_time() << sep;
    ss << std::chrono::system_clock::to_time_t(Model::get_instance().get_scheduler()->calendar_date) << sep;
    ss << date::format("%Y\t%m\t%d", Model::get_instance().get_scheduler()->calendar_date) << sep;
    ss << Model::get_instance().get_config()->get_seasonality_settings().get_seasonal_factor(Model::get_instance().get_scheduler()->calendar_date, 0) << sep;
    ss << Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(0, 1) << sep;
    ss << Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(0, 10) << sep;
    ss << Model::get_instance().get_population()->size() << sep;
    ss << group_sep;

    print_EIR_PfPR_by_location(ss);
    ss << group_sep;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    ss << Model::get_instance().get_mdc()->monthly_number_of_new_infections_by_location()[loc] << sep;
    }
    ss << group_sep;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    ss << Model::get_instance().get_mdc()->monthly_number_of_treatment_by_location()[loc] << sep;
    }
    ss << group_sep;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    ss << Model::get_instance().get_mdc()->monthly_number_of_clinical_episode_by_location()[loc] << sep;
    }
    ss << group_sep;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_NTF_by_location()[loc] << sep;
    }
    ss << group_sep;

  // including total number of positive individuals
  //  ReporterUtils::output_genotype_frequency3(ss, Model::get_instance().get_config()->get_genotype_parameters().genotype_db.size(),
  //                                            Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());

  std::stringstream gene_freq_ss;
  ReporterUtils::output_genotype_frequency3(gene_freq_ss, Model::get_instance().get_config()->get_genotype_parameters().genotype_db.size(),
                                            Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());

  gene_freq_file << gene_freq_ss.str() << std::endl;

  monthly_data_file << ss.str() << std::endl;
}

void MonthlyReporter::after_run() {
  std::stringstream ss;

  ss.str("");
  ss << Model::get_instance().get_random()->get_seed() << sep << Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations() << sep;
  ss << Model::get_instance().get_config()->get_spatial_settings().location_db[0].beta << sep;
  ss << Model::get_instance().get_config()->get_spatial_settings().location_db[0].population_size << sep;
  print_EIR_PfPR_by_location(ss);

  ss << group_sep;
  // output last strategy information
  ss << Model::get_instance().get_treatment_strategy()->id << sep;

  // output NTF
  const auto total_time_in_years = (Model::get_instance().get_scheduler()->current_time() - Model::get_instance().get_config()->get_simulation_timeframe().get_start_of_comparison_period())
                                   / static_cast<double>(Constants::DAYS_IN_YEAR);

  auto sum_ntf = 0.0;
  ul pop_size = 0;
  for (auto location = 0; location < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    sum_ntf += Model::get_instance().get_mdc()->cumulative_NTF_by_location()[location];
    pop_size += Model::get_instance().get_mdc()->popsize_by_location()[location];
  }
  ss << (sum_ntf * 100 / pop_size) / total_time_in_years << sep;

  ss << group_sep;
  for (auto location = 0; location < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); location++)
  {
    for (auto age = 0; age < 60; age++){
      ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location_age()[location][age]/total_time_in_years/Model::get_instance().get_mdc()->popsize_by_location_age()[location][age] << sep;
    }
  }
  summary_data_file << ss.str() << std::endl;

  for (auto [g_id, genotype] : Model::get_instance().get_config()->get_genotype_parameters().genotype_db) {
    gene_db_file << g_id << sep << genotype->aa_sequence << std::endl;
  }

  gene_freq_file.close();
  monthly_data_file.close();
  summary_data_file.close();
  gene_db_file.close();
}

void MonthlyReporter::print_EIR_PfPR_by_location(std::stringstream& ss) {
  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); ++loc) {
    //
    // EIR
    if (Model::get_instance().get_mdc()->EIR_by_location_year()[loc].empty()) {
      ss << 0 << sep;
    } else {
      ss << Model::get_instance().get_mdc()->EIR_by_location_year()[loc].back() << sep;
    }
    ss << group_sep;
    // pfpr <5 , 2-10 and all
    ss << Model::get_instance().get_mdc()->get_blood_slide_prevalence(loc, 2, 10) * 100 << sep;
    ss << Model::get_instance().get_mdc()->get_blood_slide_prevalence(loc, 0, 5) * 100 << sep;
    ss << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location()[loc] * 100 << sep;
  }
}
