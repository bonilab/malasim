#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Scheduler/Dispatcher.h"
#include "Events/Event.h"

using ::testing::_;
using ::testing::NiceMock;

class DispatcherTest;  // Forward declaration

// Mock Event class using GMock
class MockEvent : public Event {
public:
    explicit MockEvent(const int time) {
        this->time = time;
    }

    MOCK_METHOD(const std::string, name, (), (const, override));
    // Move do_execute to public for testing
    MOCK_METHOD(void, do_execute, (), (override));

protected:
    // Implementation will call public do_execute
    friend class Event;  // Allow base class to access protected members
};

class DispatcherTest : public ::testing::Test {
protected:
    void SetUp() override {
        dispatcher.initialize();
    }

    Dispatcher dispatcher;
};

TEST_F(DispatcherTest, InitialStateIsEmpty) {
    EXPECT_TRUE(dispatcher.get_events().empty());
}

TEST_F(DispatcherTest, ScheduleEventAddsToQueue) {
    auto* event = new NiceMock<MockEvent>(10);
    dispatcher.schedule_event(event);
    
    EXPECT_EQ(dispatcher.get_events().size(), 1);
    EXPECT_EQ(dispatcher.get_events().begin()->first, 10);
}

TEST_F(DispatcherTest, ExecuteEventsAtTime) {
    auto* event1 = new NiceMock<MockEvent>(5);
    auto* event2 = new NiceMock<MockEvent>(10);
    auto* event3 = new NiceMock<MockEvent>(15);
    
    // Set expectations for execute calls
    EXPECT_CALL(*event1, do_execute()).Times(1);
    EXPECT_CALL(*event2, do_execute()).Times(1);
    EXPECT_CALL(*event3, do_execute()).Times(1);
    
    dispatcher.schedule_event(event1);
    dispatcher.schedule_event(event2);
    dispatcher.schedule_event(event3);
    
    // Execute events up to time 7
    dispatcher.execute_events(7);
    EXPECT_EQ(dispatcher.get_events().size(), 2);
    
    // Execute events up to time 12
    dispatcher.execute_events(12);
    EXPECT_EQ(dispatcher.get_events().size(), 1);
    
    // Execute all events
    dispatcher.execute_events(20);
    EXPECT_TRUE(dispatcher.get_events().empty());
}

TEST_F(DispatcherTest, HasEventTypeCheck) {
    // Initially no events
    EXPECT_FALSE(dispatcher.has_event<Event>());
    EXPECT_FALSE(dispatcher.has_event<MockEvent>());

    auto* event = new NiceMock<MockEvent>(10);
    dispatcher.schedule_event(event);
    
    // After adding MockEvent, both should be true
    EXPECT_TRUE(dispatcher.has_event<MockEvent>());
    EXPECT_TRUE(dispatcher.has_event<Event>());
}

TEST_F(DispatcherTest, CancelAllEvents) {
    auto* event1 = new NiceMock<MockEvent>(5);
    auto* event2 = new NiceMock<MockEvent>(10);
    
    // Expect these events NOT to be executed
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(0);
    
    dispatcher.schedule_event(event1);
    dispatcher.schedule_event(event2);
    
    dispatcher.cancel_all_events<MockEvent>();
    
    // Events should still be in queue but marked as non-executable
    EXPECT_EQ(dispatcher.get_events().size(), 2);
    
    dispatcher.execute_events(15);
    
    // Events should be removed but not executed
    EXPECT_TRUE(dispatcher.get_events().empty());
}

TEST_F(DispatcherTest, CancelAllEventsExcept) {
    auto* event1 = new NiceMock<MockEvent>(5);
    auto* event2 = new NiceMock<MockEvent>(10);
    auto* event3 = new NiceMock<MockEvent>(15);
    
    // Set expectations: only event2 should be executed
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(1);
    EXPECT_CALL(*event3, do_execute()).Times(0);
    
    dispatcher.schedule_event(event1);
    dispatcher.schedule_event(event2);
    dispatcher.schedule_event(event3);
    
    dispatcher.cancel_all_events_except(event2);

    // Verify cancelled events are marked non-executable
    for (auto& [time, event] : dispatcher.get_events()) {
        if (event.get() != event2) {
            EXPECT_FALSE(event->executable);
        } else {
            EXPECT_TRUE(event->executable);
        }
    }
    
    // Execute all events
    dispatcher.execute_events(20);
    
    // Queue should be empty after execution
    EXPECT_TRUE(dispatcher.get_events().empty());
}

TEST_F(DispatcherTest, ExceptionHandling) {
    auto* event = new NiceMock<MockEvent>(5);
    EXPECT_CALL(*event, do_execute())
        .WillOnce(::testing::Throw(std::runtime_error("Test exception")));
    
    dispatcher.schedule_event(event);
    
    // Should not crash
    EXPECT_NO_THROW(dispatcher.execute_events(10));
    
    // Event should be removed even if it throws
    EXPECT_TRUE(dispatcher.get_events().empty());
}

TEST_F(DispatcherTest, HasEventWithoutType) {
    // Initially no events
    EXPECT_FALSE(dispatcher.has_event());

    auto* event = new NiceMock<MockEvent>(10);
    dispatcher.schedule_event(event);
    
    // After adding an event, should return true
    EXPECT_TRUE(dispatcher.has_event());

    // Execute all events
    dispatcher.execute_events(20);

    // After executing all events, should return false again
    EXPECT_FALSE(dispatcher.has_event());
} 