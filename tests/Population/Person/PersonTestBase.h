#ifndef PERSON_TEST_BASE_H
#define PERSON_TEST_BASE_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Configuration/Config.h"
#include "Population/Person/Person.h"
#include "Population/Population.h"
#include "Simulation/Model.h"
#include "date/date.h"

using namespace testing;

// Mock classes
class MockConfig : public Config {
public:
  MOCK_METHOD(double, get_age_dependent_biting_factor, (const int &), (const));
  MOCK_METHOD(double, get_p_infection_from_an_infectious_bite, (), (const));
  MOCK_METHOD(int, number_of_age_classes, (), (const));
  MOCK_METHOD(const std::vector<int> &, age_structure, (), (const));
  MOCK_METHOD(const PopulationDemographic &, get_population_demographic, (),
              (const));
};

class MockScheduler : public Scheduler {
public:
  MockScheduler(Model* model) : Scheduler(model) {}
  MOCK_METHOD(void, schedule_population_event, (WorldEvent*));
};

class MockPopulation : public Population {
public:
  MockPopulation(Model* model) : Population(model) {}
  MOCK_METHOD(void, notify_change,
              (Person*, const Person::Property &, const void*, const void*));
};

class PersonTestBase : public ::testing::Test {
protected:
  void SetUp() override {
    // Store original model instance
    original_model_ = &Model::get_instance();

    // model initialization
    original_model_->initialize();

    setup_mock_config();

    setup_mock_scheduler();

    setup_mock_population();

    // Create person instance
    person_ = std::make_unique<Person>();
    person_->set_population(mock_population_.get());
    person_->initialize();
  }

  void TearDown() override {
    // Restore original model state
    person_.reset();
    mock_population_.reset();

    // config will be released automatically
    original_model_->release();
  }

  void setup_mock_config() {
    // Setup Model singleton with mock config
    Model::set_config(new MockConfig());
    mock_config_ = static_cast<MockConfig*>(Model::get_config());

    // Setup default config expectations
    age_structure_ = {5, 15, 30, 50, 70, 90};  // Example age structure

    demographic_.set_number_of_age_classes(age_structure_.size());
    demographic_.set_age_structure(age_structure_);
    mock_config_->set_population_demographic(demographic_);

    mock_config_->get_simulation_timeframe().set_starting_date(date::year_month_day{date::year(2020), date::month(1), date::day(1)});
    mock_config_->get_simulation_timeframe().set_ending_date(date::year_month_day{date::year(2030), date::month(1), date::day(1)}); 

    mock_config_->get_simulation_timeframe().process_config();
  }

  void setup_mock_scheduler() {
    Model::set_scheduler(new MockScheduler(original_model_));
    mock_scheduler_ = static_cast<MockScheduler*>(Model::get_scheduler());

    mock_scheduler_->initialize(
        mock_config_->get_simulation_timeframe().get_starting_date(),
        mock_config_->get_simulation_timeframe().get_ending_date());
    
  }

  void setup_mock_population() {
    // Model::set_population(new MockPopulation(original_model_));
    // mock_population_ = static_cast<MockPopulation*>(Model::get_population());
    mock_population_ = std::make_unique<MockPopulation>(original_model_);
  }

  // Helper methods
  void set_person_age(int age) { person_->set_age(age); }

  void set_person_location(int location) { person_->set_location(location); }

  // Helper method to determine expected age class based on age
  int get_expected_age_class(int age) {
    int ac = 0;
    while (ac < (age_structure_.size() - 1) && age >= age_structure_[ac]) {
      ac++;
    }
    return ac;
  }

protected:
  std::unique_ptr<Person> person_;
  MockConfig* mock_config_;
  MockScheduler* mock_scheduler_;
  std::unique_ptr<MockPopulation> mock_population_;
  Model* original_model_;  // Store original model instance
  std::vector<int> age_structure_;
  PopulationDemographic demographic_;  // Add demographic as member variable
};

#endif  // PERSON_TEST_BASE_H

