
#include "MMCReporter.h"
#include "Simulation/Model.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "MDC/ModelDataCollector.h"
#include "Utils/Random.h"
#include "Utils/Constants.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Treatment/ITreatmentCoverageModel.h"
#include "Utility/ReporterUtils.h"

MMCReporter::MMCReporter() = default;

void MMCReporter::initialize(int job_number, const std::string &path) {
  ReporterUtils::initialize_moi_file_logger();

}

void MMCReporter::before_run() {
  // // std::cout << "MMC Reporter" << std::endl;
  // for (auto genotype : (*Model::get_instance().get_config()->get_genotype_parameters().genotype_db)){
  //   std::cout << *genotype.second << std::endl;
  // }

}

void MMCReporter::begin_time_step() { }


void MMCReporter::print_treatment_failure_rate_by_therapy() {
  for (auto tf_by_therapy : Model::get_instance().get_mdc()->current_tf_by_therapy()) {
    ss << tf_by_therapy << sep;
  }
}

void MMCReporter::print_ntf_by_location() {
  double sum_ntf = 0.0;
  ul pop_size = 0;
  for (auto location = 0; location < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    sum_ntf += Model::get_instance().get_mdc()->cumulative_NTF_by_location()[location];
    pop_size += Model::get_instance().get_mdc()->popsize_by_location()[location];

  }

  ss << (sum_ntf * 100.0 / pop_size) << sep;
}

void MMCReporter::monthly_report() {
  ss << Model::get_instance().get_scheduler()->current_time() << sep;
  ss << std::chrono::system_clock::to_time_t(Model::get_instance().get_scheduler()->calendar_date) << sep;
  ss << date::format("%Y\t%m\t%d", Model::get_instance().get_scheduler()->calendar_date) << sep;
  ss << Model::get_instance().get_config()->get_seasonality_settings().get_seasonal_factor(Model::get_instance().get_scheduler()->calendar_date, 0) << sep;
  ss << Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(0, 1) << sep;
  ss << Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(0, 10) << sep;
  ss << Model::get_instance().get_population()->size() << sep;
  ss << group_sep;

  print_EIR_PfPR_by_location();
  ss << group_sep;
  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    ss << Model::get_instance().get_mdc()->monthly_number_of_treatment_by_location()[loc] << sep;
  }
  ss << group_sep;
  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    ss << Model::get_instance().get_mdc()->monthly_number_of_clinical_episode_by_location()[loc] << sep;
  }
  ss << group_sep;

  ReporterUtils::output_genotype_frequency3(
      ss,
      Model::get_instance().get_config()->get_genotype_parameters().genotype_db->size(),
      Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());

  ss << group_sep;
  print_ntf_by_location();
  ss << group_sep;
  print_treatment_failure_rate_by_therapy();
  ss << Model::get_instance().get_mdc()->current_TF_by_location()[0];
  // CLOG(INFO, "monthly_reporter") << ss.str();
  spdlog::info("monthly_reporter {}", ss.str());
  ss.str("");
}

void MMCReporter::after_run() {
  ss.str("");
  ss << Model::get_instance().get_random()->get_seed() << sep << Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations() << sep;
  ss << Model::get_instance().get_config()->get_spatial_settings().location_db[0].beta << sep;
  ss << Model::get_instance().get_config()->get_spatial_settings().location_db[0].population_size << sep;
  print_EIR_PfPR_by_location();

  ss << group_sep;
  //output last strategy information
  ss << Model::get_instance().get_treatment_strategy()->id << sep;

  //output NTF
  const auto total_time_in_years = (Model::get_instance().get_scheduler()->current_time()
                                    - Model::get_instance().get_config()->get_simulation_timeframe().get_start_of_comparison_period())
                                    /static_cast<double>(Constants::DAYS_IN_YEAR);

  auto sum_ntf = 0.0;
  ul pop_size = 0;
  for (auto location = 0; location < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    sum_ntf += Model::get_instance().get_mdc()->cumulative_NTF_by_location()[location];
    pop_size += Model::get_instance().get_mdc()->popsize_by_location()[location];
  }

  ss << (sum_ntf * 100 / pop_size) / total_time_in_years << sep;
  ss << group_sep;

  ss << Model::get_instance().get_mdc()->cumulative_number_treatments_by_location()[0] << sep;
  ss << Model::get_instance().get_mdc()->cumulative_TF_by_location()[0] << sep;
  ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location()[0] << sep;

  ss << group_sep;
  //print # mutation events of first 10 years
  int number_of_years = Model::get_instance().get_mdc()->number_of_mutation_events_by_year().size() >= 11 ? 11 :
                        Model::get_instance().get_mdc()->number_of_mutation_events_by_year().size();
  for (int i = 0; i < number_of_years; ++i) {
    ss << Model::get_instance().get_mdc()->number_of_mutation_events_by_year()[i] << sep;
  }

  // CLOG(INFO, "summary_reporter") << ss.str();
  spdlog::info("summary_reporter {}", ss.str());
  ss.str("");

  // Report MOI
  ReporterUtils::output_moi(ss, Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());
}

void MMCReporter::print_EIR_PfPR_by_location() {
  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); ++loc) {
    //
    // EIR
    if (Model::get_instance().get_mdc()->EIR_by_location_year()[loc].empty()) {
      ss << 0 << sep;
    } else {
      ss << Model::get_instance().get_mdc()->EIR_by_location_year()[loc].back() << sep;
    }

    //pfpr <5 and all
    ss << Model::get_instance().get_mdc()->get_blood_slide_prevalence(loc, 0, 5) * 100 << sep;
    ss << Model::get_instance().get_mdc()->get_blood_slide_prevalence(loc, 2, 10) * 100 << sep;
    ss << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location()[loc] * 100 << sep;
//    std::cout << Model::get_instance().get_population()->size() << "\t"
//              << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location()[loc] * 100 << std::endl;
  }
}


