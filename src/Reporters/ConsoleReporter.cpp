#include "ConsoleReporter.h"

#include <iostream>

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "MDC/ModelDataCollector.h"
#include "Population/Population.h"
#include "Simulation/Model.h"
#include "Utility/ReporterUtils.h"
#include "Utils/Constants.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Utils/Random.h"
#include "fmt/printf.h"

ConsoleReporter::ConsoleReporter() = default;

ConsoleReporter::~ConsoleReporter() = default;

void ConsoleReporter::before_run() {
  std::cout << "Seed:" << Model::get_random()->get_seed() << '\n';
}

void report_number_by_state(const int &location, PersonIndexByLocationStateAgeClass* pi) {
  //    std::cout << std::setw(10) << std::setprecision(3);
  for (int hs = 0; hs < Person::NUMBER_OF_STATE - 1; hs++) {
    //        int sum = 0;
    //        for (int ac = 0; ac < Model::get_config()->number_of_age_classes(); ac++) {
    //            sum += pi->vPerson()[location][hs][ac].size();
    //        }
    double fraction = Model::get_mdc()->popsize_by_location_hoststate()[location][hs] * 100
                      / static_cast<double>(Model::get_mdc()->popsize_by_location()[location]);
    //        double v = sum;

    fmt::printf("%.3f\t", fraction);
  }
}

void ConsoleReporter::after_run() {
  std::cout << "==========================================================================" << '\n';

  // total time
  double total_time_in_years =
      (Model::get_scheduler()->current_time()
       - Model::get_config()->get_simulation_timeframe().get_start_collect_data_day())
      / static_cast<double>(Constants::DAYS_IN_YEAR);
  std::cout << "Total time (from equilibrium) : " << total_time_in_years << " years" << '\n';

  // report EIR
  std::cout << "EIR by location:" << '\n';
  for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
    std::cout << Model::get_mdc()->EIR_by_location()[location] << "\t";
  }
  std::cout << '\n';

  // total number of bites
  std::cout << "Number of infectious bites:" << '\n';
  for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
    std::cout << Model::get_mdc()->total_number_of_bites_by_location()[location] << "\t";
  }
  std::cout << '\n';

  std::cout << "Number of clinical episodes:" << '\n';
  for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
    std::cout << Model::get_mdc()->cumulative_clinical_episodes_by_location()[location] << "\t";
  }
  std::cout << '\n';

  std::cout << "Percentage of bites on top 20% bitten" << '\n';
  for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
    std::cout << Model::get_mdc()->percentage_bites_on_top_20_by_location()[location] * 100 << "%"
              << "\t";
  }
  std::cout << '\n';

  std::cout << "NTF by location: " << '\n';
  for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
    double location_ntf = Model::get_mdc()->cumulative_NTF_by_location()[location] * 100
                          / static_cast<double>(Model::get_mdc()->popsize_by_location()[location]);
    location_ntf /= total_time_in_years;

    std::cout << location_ntf << "\t";
  }
  std::cout << '\n';

  std::cout << "Number of mutations by location: " << '\n';
  for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
    std::cout << Model::get_mdc()->cumulative_mutants_by_location()[location] << "\t";
  }
  std::cout << '\n';

  for (int t_id = 0; t_id < Model::get_therapy_db().size(); t_id++) {
    int n_treaments = Model::get_mdc()->number_of_treatments_with_therapy_ID()[t_id];
    int n_success = Model::get_mdc()->number_of_treatments_success_with_therapy_ID()[t_id];
    int n_fail = Model::get_mdc()->number_of_treatments_fail_with_therapy_ID()[t_id];
    double p_success = (n_treaments == 0) ? 0 : n_success * 100.0 / n_treaments;

    std::cout << "Number of patients (with non-resistant parasite) treated with therapy " << t_id
              << " (% success) = " << n_treaments << " (" << p_success << "%)" << '\n';
    std::cout << "Number Failed: " << n_fail << "-" << n_success << "-" << n_treaments << '\n';
  }

  std::cout << "Strategy UTL: " << Model::get_mdc()->current_utl_duration() << '\n';

  std::cout << "AMU per parasite population: " << Model::get_mdc()->AMU_per_parasite_pop() << '\n';
  std::cout << "AMU per per: " << Model::get_mdc()->AMU_per_person() << '\n';
  std::cout << "EAMU count only clinical caused parasite: "
            << Model::get_mdc()->AMU_for_clinical_caused_parasite() << '\n';
}

void ConsoleReporter::begin_time_step() {}

void ConsoleReporter::monthly_report() {
  if (Model::get_scheduler()->current_time()
          % Model::get_config()->get_model_settings().get_days_between_stdout_output()
      == 0) {
    //        Model::get_mdc()->perform_population_statistic();

    std::cout << Model::get_scheduler()->current_time() << "\t";

    auto* pi = Model::get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();

    for (int location = 0; location < Model::get_config()->number_of_locations(); location++) {
      std::cout << "||\t";
      report_number_by_state(location, pi);
      std::cout << Model::get_mdc()->blood_slide_prevalence_by_location()[location] * 100 << "\t";
      std::cout << Model::get_mdc()->total_immune_by_location()[location]
                       / static_cast<double>(Model::get_population()->size(location))
                << "\t";
      std::cout << Model::get_mdc()->current_RITF_by_location()[location] << "-"
                << Model::get_mdc()->current_TF_by_location()[location] << "\t";
    }
    std::cout << '\n';
  }
}

