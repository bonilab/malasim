#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <string>
#include <Core/Scheduler/Dispatcher.h>
#include <Utils/Random.h>

#include "Population/ImmuneSystem/ImmunityClearanceUpdateFunction.h"
#include "date/date.h"


class Reporter;
// Forward declaration
class Config;
class Random;
class Cli;
class Scheduler;
class Population;
class ModelDataCollector;
class Mosquito;
class IStrategy;
class ITreatmentCoverageModel;
class ImmunityClearanceUpdateFunction;
class ClinicalUpdateFunction;

class Model {
public:
  // Provides global access to the singleton instance
  static Model &get_instance() {
    static Model instance;
    return instance;
  }

  // Initialize the model
  bool initialize();

  // Prevent copying and moving
  Model(const Model &) = delete;
  void operator=(const Model &) = delete;
  Model(Model &&) = delete;
  Model &operator=(Model &&) = delete;

private:
  // Private constructor and destructor
  Model(const int &object_pool_size = 100000);
  ~Model() = default;

  bool is_initialized_;

  Config* config_;
  Scheduler* scheduler_;
  Population* population_;
  utils::Random* random_;
  ModelDataCollector* mdc_;
  Mosquito* mosquito_;
  IStrategy *treatment_strategy_{nullptr};
  ITreatmentCoverageModel *treatment_coverage_{nullptr};
  ClinicalUpdateFunction* progress_to_clinical_update_function_;
  ImmunityClearanceUpdateFunction* immunity_clearance_update_function_;
  ImmunityClearanceUpdateFunction* having_drug_update_function_;
  ImmunityClearanceUpdateFunction* clinical_update_function_;
  std::vector<Reporter *> reporters_;
public:
  void before_run();
  void run();
  void after_run();
  void begin_time_step();
  void end_time_step();
  void daily_update();
  void monthly_update();
  void yearly_update();
  void release();
  Model* get_model();
  static Config* get_config();
  static Scheduler* get_scheduler();
  static utils::Random* get_random();
  static ModelDataCollector* get_mdc();
  static Population* get_population();
  static Mosquito* get_mosquito();
  static ITreatmentCoverageModel* get_treatment_coverage();
  static IStrategy* get_treatment_strategy();
  ClinicalUpdateFunction* progress_to_clinical_update_function() {
    return progress_to_clinical_update_function_;
  }
  ImmunityClearanceUpdateFunction* having_drug_update_function() {
    return having_drug_update_function_;
  }
  ImmunityClearanceUpdateFunction* immunity_clearance_update_function() {
    return immunity_clearance_update_function_;
  }
  ImmunityClearanceUpdateFunction* clinical_update_function() {
    return clinical_update_function_;
  }

  void set_treatment_coverage(ITreatmentCoverageModel* tcm);
  void set_treatment_strategy(const int &strategy_id);
  void build_initial_treatment_coverage();
  void monthly_report();
  void report_begin_of_time_step();
  void add_reporter(Reporter *reporter);
};

#endif  // MODEL_H

