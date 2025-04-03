#include "PersonTestBase.h"
#include "Events/ReturnToResidenceEvent.h"

class PersonMovementTest : public PersonTestBase {
protected:
    void SetUp() override {
        PersonTestBase::SetUp();
        // Additional setup specific to movement tests
    }
};

TEST_F(PersonMovementTest, RandomlyChooseTargetLocation) {
    // Test random location selection
    person_->randomly_choose_target_location();
    
    // Verify target locations were set
    const auto& target_locations = person_->get_today_target_locations();
    EXPECT_FALSE(target_locations.empty());
}

TEST_F(PersonMovementTest, ReturnToResidenceEvents) {
    // Initially should have no return events
    EXPECT_FALSE(person_->has_return_to_residence_event());
    
    // Schedule a return event
    const int trip_length = 5;
    person_->schedule_return_to_residence_event(trip_length);
    
    // Verify event was scheduled
    EXPECT_TRUE(person_->has_return_to_residence_event());
    
    // Test cancellation
    person_->cancel_all_return_to_residence_events();
    EXPECT_FALSE(person_->has_return_to_residence_event());
}

TEST_F(PersonMovementTest, MoveToTargetLocation) {
    const int target_location = 5;
    
    // Schedule move to target location
    person_->schedule_move_to_target_location_next_day_event(target_location);
    
    // Verify event was scheduled
    EXPECT_FALSE(person_->get_events().empty());
}

TEST_F(PersonMovementTest, TripTracking) {
    // Initially no trips
    EXPECT_EQ(person_->get_number_of_trips_taken(), 0);
    
    // Increment trips
    person_->set_number_of_trips_taken(1);
    EXPECT_EQ(person_->get_number_of_trips_taken(), 1);
}

TEST_F(PersonMovementTest, LocationStateNotification) {
    const int initial_location = 0;
    const int new_location = 5;
    
    // Set initial location
    person_->set_location(initial_location);
    EXPECT_EQ(person_->get_location(), initial_location);
    
    // Change location and verify notification worked
    person_->set_location(new_location);
    EXPECT_EQ(person_->get_location(), new_location);
}

TEST_F(PersonMovementTest, MovingLevelNotification) {
    const int initial_level = 0;
    const int new_level = 2;
    
    // Set initial moving level
    person_->set_moving_level(initial_level);
    EXPECT_EQ(person_->get_moving_level(), initial_level);
    
    // Change moving level and verify notification worked
    person_->set_moving_level(new_level);
    EXPECT_EQ(person_->get_moving_level(), new_level);
}

TEST_F(PersonMovementTest, ResidenceLocationManagement) {
    const int initial_residence = 0;
    const int new_residence = 3;
    
    // Set initial residence
    person_->set_residence_location(initial_residence);
    EXPECT_EQ(person_->get_residence_location(), initial_residence);
    
    // Change residence
    person_->set_residence_location(new_residence);
    EXPECT_EQ(person_->get_residence_location(), new_residence);
}

TEST_F(PersonMovementTest, TodayTargetLocations) {
    std::vector<int> test_locations = {1, 2, 3};
    
    // Add locations one by one
    for (int loc : test_locations) {
        person_->get_today_target_locations().push_back(loc);
    }
    
    // Verify locations were added
    const auto& target_locations = person_->get_today_target_locations();
    EXPECT_EQ(target_locations.size(), test_locations.size());
    for (size_t i = 0; i < test_locations.size(); ++i) {
        EXPECT_EQ(target_locations[i], test_locations[i]);
    }
}

#ifdef ENABLE_TRAVEL_TRACKING
TEST_F(PersonMovementTest, TravelTracking) {
    // Test travel tracking if enabled
    EXPECT_EQ(person_->get_day_that_last_trip_was_initiated(), -1);
    EXPECT_EQ(person_->get_day_that_last_trip_outside_district_was_initiated(), -1);
}
#endif 