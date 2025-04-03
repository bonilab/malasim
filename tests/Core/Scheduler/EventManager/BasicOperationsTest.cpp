#include "EventManagerTestCommon.h"

class BasicOperationsTest : public EventManagerTestBase {};

TEST_F(BasicOperationsTest, Initialize) {
    ASSERT_FALSE(event_manager.has_event());
}

TEST_F(BasicOperationsTest, ScheduleEvent) {
    auto* event = new MockEvent(0);
    event_manager.schedule_event(event);
    ASSERT_TRUE(event_manager.has_event());
}

TEST_F(BasicOperationsTest, InitialStateIsEmpty) {
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(BasicOperationsTest, ScheduleEventAddsToQueue) {
    auto* event = new NiceMock<MockEvent>(10);
    event_manager.schedule_event(event);
    
    EXPECT_EQ(event_manager.get_events().size(), 1);
    EXPECT_EQ(event_manager.get_events().begin()->first, 10);
}

TEST_F(BasicOperationsTest, ExecuteEmptyEventManager) {
    EXPECT_NO_THROW(event_manager.execute_events(0));
    EXPECT_NO_THROW(event_manager.execute_events(100));
    EXPECT_NO_THROW(event_manager.execute_events(-1));
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(BasicOperationsTest, HasEventWithoutType) {
    EXPECT_FALSE(event_manager.has_event());

    auto* event = new NiceMock<MockEvent>(10);
    event_manager.schedule_event(event);
    EXPECT_TRUE(event_manager.has_event());

    event_manager.execute_events(20);
    EXPECT_FALSE(event_manager.has_event());
}

TEST_F(BasicOperationsTest, LocalEventManager) {
    EventManager<PersonEvent> local_event_manager;
    ASSERT_FALSE(local_event_manager.has_event());
} 