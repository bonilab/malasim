#include "PersonTestBase.h"

using namespace testing;

class PersonBasicTest : public PersonTestBase {};

// Add this test first to verify mock setup
TEST_F(PersonBasicTest, ConfigMockSetup) {
  // Verify that age_structure mock is working
  const auto &age_struct = Model::get_config()->age_structure();
  EXPECT_EQ(age_struct.size(), age_structure_.size());
  for (size_t i = 0; i < age_struct.size(); ++i) {
    EXPECT_EQ(age_struct[i], age_structure_[i]);
  }

  // Verify number of age classes
  EXPECT_EQ(Model::get_config()->number_of_age_classes(), age_structure_.size());
}

TEST_F(PersonBasicTest, InitialState) {
  EXPECT_EQ(person_->get_host_state(), Person::HostStates::SUSCEPTIBLE);
  EXPECT_EQ(person_->get_age(), 0);
  EXPECT_EQ(person_->get_location(), 0);
  EXPECT_EQ(person_->get_residence_location(), 0);
}

TEST_F(PersonBasicTest, AgeManagement) {
  // Test age setting that changes age class
  // Age 25 should be in age class 2 (between 15 and 30 in our age structure)
  EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::AGE, _, _))
      .Times(1);
  EXPECT_CALL(*mock_population_,
              notify_change(_, Person::Property::AGE_CLASS, _, _))
      .Times(1);

  person_->set_age(25);
  EXPECT_EQ(person_->get_age(), 25);
  EXPECT_EQ(person_->get_age_class(), get_expected_age_class(25));

  // Test age increase that doesn't change age class
  // Age 26 should still be in the same age class
  EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::AGE, _, _))
      .Times(1);
  EXPECT_CALL(*mock_population_,
              notify_change(_, Person::Property::AGE_CLASS, _, _))
      .Times(0);

  person_->increase_age_by_1_year();
  EXPECT_EQ(person_->get_age(), 26);
  EXPECT_EQ(person_->get_age_class(), get_expected_age_class(26));

  // Test age change that crosses age class boundary
  // Age 31 should be in age class 3 (between 30 and 50)
  EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::AGE, _, _))
      .Times(1);
  EXPECT_CALL(*mock_population_,
              notify_change(_, Person::Property::AGE_CLASS, _, _))
      .Times(1);

  person_->set_age(31);
  EXPECT_EQ(person_->get_age(), 31);
  EXPECT_EQ(person_->get_age_class(), get_expected_age_class(31));

  // Test floating age calculation
  person_->set_birthday(100);  // Set birthday in simulation time
  EXPECT_DOUBLE_EQ(person_->age_in_floating(11415),
                   31.0);  // 31 years = 11315 days
}

TEST_F(PersonBasicTest, LocationManagement) {
  // Test location setting
  EXPECT_CALL(*mock_population_,
              notify_change(_, Person::Property::LOCATION, _, _))
      .Times(1);

  person_->set_location(5);
  EXPECT_EQ(person_->get_location(), 5);

  // Test residence location
  person_->set_residence_location(3);
  EXPECT_EQ(person_->get_residence_location(), 3);
}

TEST_F(PersonBasicTest, HostStateTransitions) {
  // Test initial state
  EXPECT_EQ(person_->get_host_state(), Person::HostStates::SUSCEPTIBLE);

  // Test state transition
  EXPECT_CALL(*mock_population_,
              notify_change(_, Person::Property::HOST_STATE, _, _))
      .Times(2);

  person_->set_host_state(Person::HostStates::EXPOSED);
  EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);

  person_->set_host_state(Person::HostStates::CLINICAL);
  EXPECT_EQ(person_->get_host_state(), Person::HostStates::CLINICAL);
}

TEST_F(PersonBasicTest, BitingRateManagement) {
  // Test innate biting rate
  const double test_rate = 0.5;
  person_->set_innate_relative_biting_rate(test_rate);
  EXPECT_DOUBLE_EQ(person_->get_innate_relative_biting_rate(), test_rate);

  // Test current biting rate
  person_->set_current_relative_biting_rate(test_rate * 2);
  EXPECT_DOUBLE_EQ(person_->get_current_relative_biting_rate(), test_rate * 2);
}

TEST_F(PersonBasicTest, MovingLevelManagement) {
  // Test moving level setting
  EXPECT_CALL(*mock_population_,
              notify_change(_, Person::Property::MOVING_LEVEL, _, _))
      .Times(1);

  person_->set_moving_level(2);
  EXPECT_EQ(person_->get_moving_level(), 2);
}

TEST_F(PersonBasicTest, UpdateTime) {
  const int test_time = 100;
  person_->set_latest_update_time(test_time);
  EXPECT_EQ(person_->get_latest_update_time(), test_time);
}

TEST_F(PersonBasicTest, RecurrenceStatus) {
  // Test initial status
  EXPECT_EQ(person_->get_recurrence_status(), Person::RecurrenceStatus::NONE);

  // Test status change
  person_->set_recurrence_status(Person::RecurrenceStatus::WITH_SYMPTOM);
  EXPECT_EQ(person_->get_recurrence_status(),
            Person::RecurrenceStatus::WITH_SYMPTOM);
}

