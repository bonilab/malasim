#include "Model.h"

#include <Core/Scheduler/Scheduler.h>
#include <Population/Population.h>
#include <Utils/Random.h>

#include <iostream>
#include <stdexcept>

#include "Configuration/Config.h"
#include "MDC/ModelDataCollector.h"
#include "Mosquito/Mosquito.h"
#include "Population/ClinicalUpdateFunction.h"
#include "Population/ImmuneSystem/ImmunityClearanceUpdateFunction.h"
#include "Reporters/Reporter.h"
#include "Treatment/SteadyTCM.h"
#include "Utils/Cli.hxx"
#include "Utils/Helpers/ObjectHelpers.h"

Model* Model::instance = nullptr;

// Private constructor: creates the Config instance
Model::Model(const int &object_pool_size)
    : is_initialized_(false),
      config_(nullptr),
      scheduler_(nullptr),
      random_(nullptr),
      population_(nullptr),
      mdc_(nullptr),
      mosquito_(nullptr),
      treatment_strategy_(nullptr),
      treatment_coverage_(nullptr) {
  // initialize_object_pool(object_pool_size);
}

void Model::set_treatment_strategy(const int &strategy_id) {
  treatment_strategy_ =
      strategy_id == -1
          ? nullptr
          : config_->get_strategy_parameters().strategy_db[strategy_id];
  treatment_strategy_->adjust_started_time_point(
      Model::get_scheduler()->current_time());
}

void Model::set_treatment_coverage(ITreatmentCoverageModel* tcm) {
  if (treatment_coverage_ != tcm) {
    if (tcm->p_treatment_under_5.empty() || tcm->p_treatment_over_5.empty()) {
      // copy current value
      tcm->p_treatment_under_5 = treatment_coverage_->p_treatment_under_5;
      tcm->p_treatment_over_5 = treatment_coverage_->p_treatment_over_5;
    }
    ObjectHelpers::delete_pointer<ITreatmentCoverageModel>(treatment_coverage_);
  }
  treatment_coverage_ = tcm;
}

void Model::build_initial_treatment_coverage() {
  auto* tcm = new SteadyTCM();
  for (auto &location : config_->get_spatial_settings().location_db) {
    tcm->p_treatment_under_5.push_back(location.p_treatment_under_5);
    tcm->p_treatment_over_5.push_back(location.p_treatment_over_5);
  }
  set_treatment_coverage(tcm);
}

bool Model::initialize() {
  config_ = std::make_unique<Config>();
  random_ = std::make_unique<utils::Random>(nullptr, -1);
  scheduler_ = std::make_unique<Scheduler>(this);
  population_ = std::make_unique<Population>(this);

  mdc_ = new ModelDataCollector(this);
  mosquito_ = new Mosquito(this);

  progress_to_clinical_update_function_ = new ClinicalUpdateFunction(this);
  immunity_clearance_update_function_ =
      new ImmunityClearanceUpdateFunction(this);
  having_drug_update_function_ = new ImmunityClearanceUpdateFunction(this);
  clinical_update_function_ = new ImmunityClearanceUpdateFunction(this);

  if (utils::Cli::get_instance().get_input_path().empty()) {
    // spdlog::error("Input path is empty. Please provide a valid input path.");
    // return false;
    spdlog::warn("Input path is empty. Model initialized without configuration file.");
    return true;
  }

  // if input path is not empty, load configuration file
  spdlog::info("Loading configuration file: " + utils::Cli::get_instance().get_input_path());
  if (config_->load(utils::Cli::get_instance().get_input_path())) {
    if (config_->get_model_settings().get_initial_seed_number() <= 0) {
      random_->set_seed(
          std::chrono::system_clock::now().time_since_epoch().count());
    } else {
      random_->set_seed(
          config_->get_model_settings().get_initial_seed_number());
    }

    if (utils::Cli::get_instance().get_output_path().empty()) {
      utils::Cli::get_instance().set_output_path("./");
    }

    spdlog::info("Model initialized with seed: "
                  + std::to_string(random_->get_seed()));
    // add reporter here
    if (utils::Cli::get_instance().get_reporter().empty()) {
      add_reporter(Reporter::MakeReport(Reporter::SQLITE_MONTHLY_REPORTER));
    } else {
      if (Reporter::ReportTypeMap.find(
              utils::Cli::get_instance().get_reporter())
          != Reporter::ReportTypeMap.end()) {
        add_reporter(Reporter::MakeReport(
            Reporter::ReportTypeMap[utils::Cli::get_instance()
                                        .get_reporter()]));
      }
    }

#ifdef ENABLE_TRAVEL_TRACKING
    add_reporter(Reporter::MakeReport(Reporter::TRAVEL_TRACKING_REPORTER));
#endif

    // initialize reporters
    for (auto* reporter : reporters_) {
      reporter->initialize(utils::Cli::get_instance().get_job_number(),
                            utils::Cli::get_instance().get_output_path());
    }
    spdlog::info("Model initialized reporters.");

    scheduler_->initialize(
        config_->get_simulation_timeframe().get_starting_date(),
        config_->get_simulation_timeframe().get_ending_date());
    spdlog::info("Model initialized scheduler.");

    set_treatment_strategy(
        config_->get_strategy_parameters().get_initial_strategy_id());
    spdlog::info("Model initialized treatment strategy.");

    build_initial_treatment_coverage();
    spdlog::info("Model initialized treatment coverage model.");

    mdc_->initialize();
    spdlog::info("Model initialized data collector.");

    spdlog::info("Model initializing population...");
    population_->initialize();
    spdlog::info("Model initialized population.");

    config_->get_movement_settings().get_spatial_model()->prepare();
    spdlog::info("Model initialized movement model.");

    mosquito_->initialize(config_.get());
    spdlog::info("Model initialized mosquito.");

    population_->introduce_initial_cases();
    spdlog::info("Model initialized initial cases.");

    for (auto* event : config_->get_population_events().get_events()) {
      spdlog::info("Model initialized population event: " + event->name());
      scheduler_->schedule_population_event(event);
    }

    if (utils::Cli::get_instance().get_record_movement()) {
      // Generate a movement reporter
      Reporter* reporter =
          Reporter::MakeReport(Reporter::ReportType::MOVEMENT_REPORTER);
      add_reporter(reporter);
      reporter->initialize(utils::Cli::get_instance().get_job_number(),
                            utils::Cli::get_instance().get_output_path());
    }
    is_initialized_ = true;
  } else {
    spdlog::error("Failed to load configuration file: "
                  + utils::Cli::get_instance().get_input_path());
  }
  return is_initialized_;
}

void Model::release() {
  // Clean up the memory used by the model
  ObjectHelpers::delete_pointer<ClinicalUpdateFunction>(
      progress_to_clinical_update_function_);
  ObjectHelpers::delete_pointer<ImmunityClearanceUpdateFunction>(
      immunity_clearance_update_function_);
  ObjectHelpers::delete_pointer<ImmunityClearanceUpdateFunction>(
      having_drug_update_function_);
  ObjectHelpers::delete_pointer<ImmunityClearanceUpdateFunction>(
      clinical_update_function_);
  ObjectHelpers::delete_pointer<ModelDataCollector>(mdc_);

  treatment_strategy_ = nullptr;
  ObjectHelpers::delete_pointer<ITreatmentCoverageModel>(treatment_coverage_);

  population_.reset();
  random_.reset();
  scheduler_.reset();
  config_.reset();

  for (Reporter* reporter : reporters_) {
    ObjectHelpers::delete_pointer<Reporter>(reporter);
  }
  reporters_.clear();
}

void Model::run() {
  if (!is_initialized_) {
    throw std::runtime_error(
        "Model is not initialized. Call Initialize() first.");
  }
  before_run();
  scheduler_->run();
  after_run();
}

void Model::before_run() {
  spdlog::info("Perform before run events");
  for (auto* reporter : reporters_) { reporter->before_run(); }
}

void Model::after_run() {
  spdlog::info("Perform after run events");

  mdc_->update_after_run();

  for (auto* reporter : reporters_) { reporter->after_run(); }
}

void Model::begin_time_step() {
  // reset daily variables
  mdc_->begin_time_step();
  report_begin_of_time_step();
}

void Model::end_time_step() {
  // update / calculate daily UTL
  mdc_->end_of_time_step();

  // check to switch strategy
  treatment_strategy_->update_end_of_time_step();
}

void Model::daily_update() {
  population_->update_all_individuals();
  // for safety remove all dead by calling perform_death_event
  population_->perform_death_event();
  population_->perform_birth_event();

  // update current foi should be call after perform death, birth event
  // in order to obtain the right all alive individuals,
  // infection event will use pre-calculated individual relative biting rate to
  // infect new infections circulation event will use pre-calculated individual
  // relative moving rate to migrate individual to new location
  population_->update_current_foi();

  population_->perform_infection_event();
  population_->perform_circulation_event();

  // infect new mosquito cohort in prmc must be run after population perform
  // infection event and update current foi because the prmc at the tracking
  // index will be overridden with new cohort to use N days later and infection
  // event used the prmc at the tracking index for the today infection
  auto tracking_index =
      scheduler_->current_time() % config_->number_of_tracking_days();
  mosquito_->infect_new_cohort_in_PRMC(config_.get(), random_.get(), population_.get(),
                                       tracking_index);

  // this function must be called after mosquito infect new cohort in prmc
  population_->persist_current_force_of_infection_to_use_N_days_later();
}

void Model::monthly_update() {
  monthly_report();

  // reset monthly variables
  mdc_->monthly_update();

  //
  treatment_strategy_->monthly_update();

  // update treatment coverage
  treatment_coverage_->monthly_update();
}

void Model::yearly_update() { mdc_->yearly_update(); }

void Model::monthly_report() {
  mdc_->perform_population_statistic();

  for (auto* reporter : reporters_) { reporter->monthly_report(); }
}

void Model::report_begin_of_time_step() {
  for (auto* reporter : reporters_) { reporter->begin_time_step(); }
}

void Model::add_reporter(Reporter* reporter) {
  reporters_.push_back(reporter);
  reporter->set_model(this);
}

ModelDataCollector* Model::get_mdc() { return get_instance()->mdc_; }

Mosquito* Model::get_mosquito() { return get_instance()->mosquito_; }

IStrategy* Model::get_treatment_strategy() {
  return get_instance()->treatment_strategy_;
}

ITreatmentCoverageModel* Model::get_treatment_coverage() {
  return get_instance()->treatment_coverage_;
}

