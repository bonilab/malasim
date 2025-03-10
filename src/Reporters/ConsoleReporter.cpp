#include <iostream>
#include "fmt/printf.h"
#include "ConsoleReporter.h"
#include "Simulation/Model.h"
#include "Utils/Random.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "MDC/ModelDataCollector.h"
#include "Utils/Constants.h"
#include "Utility/ReporterUtils.h"

ConsoleReporter::ConsoleReporter() {
}

ConsoleReporter::~ConsoleReporter() {
}

void ConsoleReporter::before_run() {
  std::cout << "Seed:" << Model::get_random()->get_seed() << std::endl;

}

void report_number_by_state(const int &location, PersonIndexByLocationStateAgeClass* pi) {
  //    std::cout << std::setw(10) << std::setprecision(3);
  for (int hs = 0; hs < Person::NUMBER_OF_STATE - 1; hs++) {
    //        int sum = 0;
    //        for (int ac = 0; ac < Model::get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
    //            sum += pi->vPerson()[location][hs][ac].size();
    //        }
    double v = Model::get_mdc()->popsize_by_location_hoststate()[location][hs] * 100 /
               (double) Model::get_mdc()->popsize_by_location()[location];
    //        double v = sum;

    fmt::printf("%.3f\t", v);

  }

}

void ConsoleReporter::after_run() {
  std::cout << "==========================================================================" << std::endl;

  //total time
  double total_time_in_years = (Model::get_scheduler()->current_time()
      - Model::get_config()->get_simulation_timeframe().get_start_collect_data_day())
      / static_cast<double>(Constants::DAYS_IN_YEAR);
  std::cout << "Total time (from equilibrium) : " << total_time_in_years << " years" << std::endl;

  //report EIR
  std::cout << "EIR by location:" << std::endl;
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    std::cout << Model::get_mdc()->EIR_by_location()[location] << "\t";
  }
  std::cout << std::endl;

  //total number of bites
  std::cout << "Number of infectious bites:" << std::endl;
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    std::cout << Model::get_mdc()->total_number_of_bites_by_location()[location] << "\t";
  }
  std::cout << std::endl;

  std::cout << "Number of clinical episodes:" << std::endl;
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    std::cout << Model::get_mdc()->cumulative_clinical_episodes_by_location()[location] << "\t";
  }
  std::cout << std::endl;

  std::cout << "Percentage of bites on top 20% bitten" << std::endl;
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    std::cout << Model::get_mdc()->percentage_bites_on_top_20_by_location()[location] * 100 << "%" << "\t";
  }
  std::cout << std::endl;

  std::cout << "NTF by location: " << std::endl;
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    double location_NTF = Model::get_mdc()->cumulative_NTF_by_location()[location] * 100 /
                          (double) Model::get_mdc()->popsize_by_location()[location];
    location_NTF /= total_time_in_years;

    std::cout << location_NTF << "\t";
  }
  std::cout << std::endl;

  std::cout << "Number of mutations by location: " << std::endl;
  for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    std::cout << Model::get_mdc()->cumulative_mutants_by_location()[location] << "\t";
  }
  std::cout << std::endl;

  for (int t_id = 0; t_id < Model::get_config()->get_therapy_parameters().therapy_db.size(); t_id++) {

    int nTreaments = Model::get_mdc()->number_of_treatments_with_therapy_ID()[t_id];
    int nSuccess = Model::get_mdc()->number_of_treatments_success_with_therapy_ID()[t_id];
    int nFail = Model::get_mdc()->number_of_treatments_fail_with_therapy_ID()[t_id];
    double pSuccess = (nTreaments == 0) ? 0 : nSuccess * 100.0 / nTreaments;

    std::cout << "Number of patients (with non-resistant parasite) treated with therapy " << t_id
              << " (% success) = "
              << nTreaments << " (" << pSuccess << "%)" << std::endl;
    std::cout << "Number Failed: " << nFail << "-" << nSuccess << "-" << nTreaments << std::endl;

  }

  std::cout << "Strategy UTL: " << Model::get_mdc()->current_utl_duration() << std::endl;

  std::cout << "AMU per parasite population: " << Model::get_mdc()->AMU_per_parasite_pop() << std::endl;
  std::cout << "AMU per per: " << Model::get_mdc()->AMU_per_person() << std::endl;
  std::cout << "EAMU count only clinical caused parasite: "
            << Model::get_mdc()->AMU_for_clinical_caused_parasite()
            << std::endl;

}

void ConsoleReporter::begin_time_step() {
}

void ConsoleReporter::monthly_report() {

  if (Model::get_scheduler()->current_time() % Model::get_config()->get_model_settings().get_days_between_stdout_output() == 0) {
//        Model::get_mdc()->perform_population_statistic();

    std::cout << Model::get_scheduler()->current_time() << "\t";

    auto* pi = Model::get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();

    for (int location = 0; location < Model::get_config()->get_spatial_settings().get_number_of_locations(); location++) {
      std::cout << "||\t";
      report_number_by_state(location, pi);
      std::cout << Model::get_mdc()->blood_slide_prevalence_by_location()[location] * 100 << "\t";
      std::cout << Model::get_mdc()->total_immune_by_location()[location] / Model::get_population()->size(location)
                << "\t";
      std::cout << Model::get_mdc()->current_RITF_by_location()[location] << "-"
                << Model::get_mdc()->current_TF_by_location()[location] << "\t";
    }
    std::cout << std::endl;
  }
}