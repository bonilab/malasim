#include <iostream>
#include <stdexcept>
#include "Model.h"
#include <Core/Scheduler/Scheduler.h>
#include <Population/Population.h>
#include <Utils/Random.h>

#include "Configuration/Config.h"  // Assuming Config is defined here
#include "Helpers/ObjectHelpers.h"
#include "MDC/ModelDataCollector.h"
#include "Mosquito/Mosquito.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Population/ImmuneSystem/ImmunityClearanceUpdateFunction.h"
#include "Reporters/Reporter.h"
#include "Treatment/SteadyTCM.h"
#include "Utils/Cli.hxx"

// Private constructor: creates the Config instance
Model::Model(const int &object_pool_size){
  // initialize_object_pool(object_pool_size);

  random_ = new utils::Random();
  config_ = new Config();
  scheduler_ = new Scheduler(this);
  population_ = new Population(this);
  mdc_ = new ModelDataCollector(this);
  mosquito_ = new Mosquito(this);

  progress_to_clinical_update_function_ = new ClinicalUpdateFunction(this);
  immunity_clearance_update_function_ = new ImmunityClearanceUpdateFunction(this);
  having_drug_update_function_ = new ImmunityClearanceUpdateFunction(this);
  clinical_update_function_ = new ImmunityClearanceUpdateFunction(this);
}

void Model::set_treatment_strategy(const int& strategy_id) {
  treatment_strategy_ = strategy_id == -1 ? nullptr : config_->get_strategy_parameters().strategy_db[strategy_id];
  treatment_strategy_->adjust_started_time_point(Model::get_instance().get_scheduler()->current_time());
}

void Model::set_treatment_coverage(ITreatmentCoverageModel* tcm) {
  if (treatment_coverage_ != tcm) {
    if (tcm->p_treatment_less_than_5.empty() || tcm->p_treatment_more_than_5.empty()) {
      // copy current value
      tcm->p_treatment_less_than_5 = treatment_coverage_->p_treatment_less_than_5;
      tcm->p_treatment_more_than_5 = treatment_coverage_->p_treatment_more_than_5;
    }
    ObjectHelpers::delete_pointer<ITreatmentCoverageModel>(treatment_coverage_);
  }
  treatment_coverage_ = tcm;
}

void Model::build_initial_treatment_coverage() {
  auto* tcm = new SteadyTCM();
  for (auto& location : config_->get_spatial_settings().location_db) {
    tcm->p_treatment_less_than_5.push_back(location.p_treatment_under_5);
    tcm->p_treatment_more_than_5.push_back(location.p_treatment_over_5);
  }
  set_treatment_coverage(tcm);
}

bool Model::initialize() {
  if (!utils::Cli::get_instance().get_input_path().empty()) {
    if(config_->load(utils::Cli::get_instance().get_input_path())) {
      if(config_->get_model_settings().get_initial_seed_number() <= 0) {
        random_->set_seed(std::chrono::system_clock::now().time_since_epoch().count());
      } else {
        random_->set_seed(config_->get_model_settings().get_initial_seed_number());
      }

      if (utils::Cli::get_instance().get_output_path().empty()) {
        utils::Cli::get_instance().set_output_path("./");
      }

      // add reporter here
      if (utils::Cli::get_instance().get_reporter().empty()) {
        add_reporter(Reporter::MakeReport(Reporter::MONTHLY_REPORTER));
      } else {
        if (Reporter::ReportTypeMap.find(utils::Cli::get_instance().get_reporter()) != Reporter::ReportTypeMap.end()) {
          add_reporter(Reporter::MakeReport(Reporter::ReportTypeMap[utils::Cli::get_instance().get_reporter()]));
        }
      }

      // initialize reporters
      for (auto* reporter : reporters_) {
        reporter->initialize();
      }

      scheduler_->initialize(config_->get_simulation_timeframe().get_starting_date(),
                             config_->get_simulation_timeframe().get_ending_date());

      set_treatment_strategy(config_->get_strategy_parameters().get_initial_strategy_id());

      build_initial_treatment_coverage();

      mdc_->initialize();

      population_->initialize();

      mosquito_->initialize(config_);

      // population_->introduce_initial_cases();

      // for (auto* event : config_->get_preconfig_population_events().get_events()) {
      //   scheduler_->schedule_population_event(event);
      // }

      is_initialized_ = true;
    }
    else {
      spdlog::error("Failed to load configuration file: " + utils::Cli::get_instance().get_input_path());
    }
  }
  return is_initialized_;
}

void Model::run() const {
  if (!is_initialized_) {
    throw std::runtime_error(
        "Model is not initialized. Call Initialize() first.");
  }
  scheduler_->run();
}

void Model::finalize() {
  if (!is_initialized_) {
    throw std::runtime_error("Model is not initialized or already finalized.");
  }
  // Cleanup code
  delete population_;
  delete scheduler_;
  delete config_;
  delete random_;
}

void Model::begin_time_step() {
  // spdlog::info("\t\t\tBegin time step");
}

void Model::end_time_step() {
  // spdlog::info("\t\t\tEnd time step");
}

void Model::daily_update() {
  spdlog::info("\tDaily update {}", scheduler_->current_time());
  // population_->update(scheduler_->current_time());
}

void Model::monthly_update() {
  // spdlog::info("\t\tMonthly update");
}

void Model::yearly_update() {
  // spdlog::info("\tYearly update");
}

Model* Model::get_model() {
  return this;
}

utils::Random* Model::get_random() {
  return random_;
}

ModelDataCollector* Model::get_mdc() {
  return mdc_;
}

Population* Model::get_population() {
  return population_;
}

Mosquito* Model::get_mosquito() {
  return mosquito_;
}

IStrategy* Model::get_treatment_strategy() {
  return treatment_strategy_;
}

ITreatmentCoverageModel* Model::get_treatment_coverage() {
  return treatment_coverage_;
}

void Model::monthly_report() {
  mdc_->perform_population_statistic();

  for (auto* reporter : reporters_) {
    reporter->monthly_report();
  }
}

void Model::report_begin_of_time_step() {
  for (auto* reporter : reporters_) {
    reporter->begin_time_step();
  }
}

void Model::add_reporter(Reporter* reporter) {
  reporters_.push_back(reporter);
  reporter->set_model(this);
}



