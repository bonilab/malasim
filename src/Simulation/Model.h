#ifndef MODEL_H
#define MODEL_H

#include <Utils/Random.h>

#include <memory>

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Population/ImmuneSystem/ImmunityClearanceUpdateFunction.h"
#include "Population/Population.h"

namespace Spatial {
class Location;
}

class Reporter;
class Cli;
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
  static Model* get_instance() {
    if (instance == nullptr) {
      instance = new Model();
    }
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

  static Model* instance;
  bool is_initialized_;

  std::unique_ptr<Config> config_{nullptr};
  std::unique_ptr<Scheduler> scheduler_{nullptr};
  std::unique_ptr<Population> population_{nullptr};
  std::unique_ptr<utils::Random> random_;
  std::unique_ptr<GenotypeDatabase> genotype_db_{nullptr};
  ModelDataCollector* mdc_;
  Mosquito* mosquito_;
  IStrategy* treatment_strategy_{nullptr};
  ITreatmentCoverageModel* treatment_coverage_{nullptr};
  ClinicalUpdateFunction* progress_to_clinical_update_function_;
  ImmunityClearanceUpdateFunction* immunity_clearance_update_function_;
  ImmunityClearanceUpdateFunction* having_drug_update_function_;
  ImmunityClearanceUpdateFunction* clinical_update_function_;
  std::vector<Reporter*> reporters_;

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
  
  static Config* get_config() { return get_instance()->config_.get(); }
  static void set_config(Config* config) {
    get_instance()->config_.reset(config);
  }

  static Scheduler* get_scheduler() { return get_instance()->scheduler_.get(); }

  static void set_scheduler(Scheduler* scheduler) {
    get_instance()->scheduler_.reset(scheduler);
  }

  static utils::Random* get_random() { return get_instance()->random_.get(); }
  
  static void set_random(utils::Random* random) {
    get_instance()->random_.reset(random);
  }

  static Population* get_population() { return get_instance()->population_.get(); }
  static void set_population(Population* population) {
    get_instance()->population_.reset(population);
  }

  static GenotypeDatabase* get_genotype_db() { return get_instance()->genotype_db_.get(); }
  static void set_genotype_db(GenotypeDatabase* genotype_db) {
    get_instance()->genotype_db_.reset(genotype_db);
  }
  
  static ModelDataCollector* get_mdc();
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
  void add_reporter(Reporter* reporter);
};

#endif  // MODEL_H

