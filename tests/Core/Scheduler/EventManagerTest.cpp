#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Scheduler/EventManager.h"
#include "Events/Event.h"
#include <memory>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::InSequence;

class EventManagerTest;  // Forward declaration

// Mock Event class using GMock
class MockEvent : public Event {
public:
    explicit MockEvent(const int time) {
        this->time = time;
    }

    MOCK_METHOD(const std::string, name, (), (const, override));
    // Move do_execute to public for testing
    MOCK_METHOD(void, do_execute, (), (override));
    
    // Add mock destructor
    MOCK_METHOD(void, die, ());  // Helper method to track destruction
    ~MockEvent() override {
        die();  // Call mock method in destructor
    }

protected:
    // Implementation will call public do_execute
    friend class Event;  // Allow base class to access protected members
};

// // Add a test event class that tracks destruction
// class DestructionTrackingEvent : public Event {
// public:
//     explicit DestructionTrackingEvent(const int time, bool* destroyed) 
//         : destroyed_(destroyed) {
//         this->time = time;
//     }
    
//     ~DestructionTrackingEvent() override {
//         if (destroyed_) {
//             *destroyed_ = true;
//         }
//     }

//     std::string name() override { return "DestructionTrackingEvent"; }
//     void do_execute() override {}

// private:
//     bool* destroyed_;
// };

class EventManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        event_manager.initialize();
    }

    EventManager event_manager;
};

TEST_F(EventManagerTest, InitialStateIsEmpty) {
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, ScheduleEventAddsToQueue) {
    auto* event = new NiceMock<MockEvent>(10);
    event_manager.schedule_event(event);
    
    EXPECT_EQ(event_manager.get_events().size(), 1);
    EXPECT_EQ(event_manager.get_events().begin()->first, 10);
}

TEST_F(EventManagerTest, ExecuteEventsAtTime) {
    auto* event1 = new NiceMock<MockEvent>(5);
    auto* event2 = new NiceMock<MockEvent>(10);
    auto* event3 = new NiceMock<MockEvent>(15);
    
    // Set expectations for execute calls
    EXPECT_CALL(*event1, do_execute()).Times(1);
    EXPECT_CALL(*event2, do_execute()).Times(1);
    EXPECT_CALL(*event3, do_execute()).Times(1);
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    event_manager.schedule_event(event3);
    
    // Execute events up to time 7
    event_manager.execute_events(7);
    EXPECT_EQ(event_manager.get_events().size(), 2);
    
    // Execute events up to time 12
    event_manager.execute_events(12);
    EXPECT_EQ(event_manager.get_events().size(), 1);
    
    // Execute all events
    event_manager.execute_events(20);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, HasEventTypeCheck) {
    // Initially no events
    EXPECT_FALSE(event_manager.has_event<Event>());
    EXPECT_FALSE(event_manager.has_event<MockEvent>());

    auto* event = new NiceMock<MockEvent>(10);
    event_manager.schedule_event(event);
    
    // After adding MockEvent, both should be true
    EXPECT_TRUE(event_manager.has_event<MockEvent>());
    EXPECT_TRUE(event_manager.has_event<Event>());
}

TEST_F(EventManagerTest, CancelAllEvents) {
    auto* event1 = new NiceMock<MockEvent>(5);
    auto* event2 = new NiceMock<MockEvent>(10);
    
    // Expect these events NOT to be executed
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(0);
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    
    event_manager.cancel_all_events<MockEvent>();
    
    // Events should still be in queue but marked as non-executable
    EXPECT_EQ(event_manager.get_events().size(), 2);
    
    event_manager.execute_events(15);
    
    // Events should be removed but not executed
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, CancelAllEventsExcept) {
    auto* event1 = new NiceMock<MockEvent>(5);
    auto* event2 = new NiceMock<MockEvent>(10);
    auto* event3 = new NiceMock<MockEvent>(15);
    
    // Set expectations: only event2 should be executed
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(1);
    EXPECT_CALL(*event3, do_execute()).Times(0);
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    event_manager.schedule_event(event3);
    
    event_manager.cancel_all_events_except(event2);

    // Verify cancelled events are marked non-executable
    for (auto& [time, event] : event_manager.get_events()) {
        if (event.get() != event2) {
            EXPECT_FALSE(event->executable);
        } else {
            EXPECT_TRUE(event->executable);
        }
    }
    
    // Execute all events
    event_manager.execute_events(20);
    
    // Queue should be empty after execution
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, ExceptionHandling) {
    auto* event = new NiceMock<MockEvent>(5);
    EXPECT_CALL(*event, do_execute())
        .WillOnce(::testing::Throw(std::runtime_error("Test exception")));
    
    event_manager.schedule_event(event);
    
    // Should not crash
    EXPECT_NO_THROW(event_manager.execute_events(10));
    
    // Event should be removed even if it throws
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, HasEventWithoutType) {
    // Initially no events
    EXPECT_FALSE(event_manager.has_event());

    auto* event = new NiceMock<MockEvent>(10);
    event_manager.schedule_event(event);
    
    // After adding an event, should return true
    EXPECT_TRUE(event_manager.has_event());

    // Execute all events
    event_manager.execute_events(20);

    // After executing all events, should return false again
    EXPECT_FALSE(event_manager.has_event());
}

TEST_F(EventManagerTest, NoMemoryLeakOnException) {
    auto* event = new testing::StrictMock<MockEvent>(5);
    
    // Set expectations for both do_execute and name
    EXPECT_CALL(*event, do_execute())
        .WillOnce(::testing::Throw(std::runtime_error("Test exception")));
    EXPECT_CALL(*event, name())
        .WillOnce(::testing::Return("MockEvent"));  // Called during error logging
    EXPECT_CALL(*event, die()).Times(1);  // Should be destroyed even after exception
    
    event_manager.schedule_event(event);
    
    // Should not crash and should destroy the event
    EXPECT_NO_THROW(event_manager.execute_events(10));
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, MemoryManagementOnExecution) {
    auto* event = new testing::StrictMock<MockEvent>(5);
    
    // Expect execute and destruction in order
    {
        testing::InSequence seq;
        EXPECT_CALL(*event, do_execute());
        EXPECT_CALL(*event, die());  // Should be destroyed after execution
    }
    
    event_manager.schedule_event(event);
    event_manager.execute_events(10);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, MemoryManagementOnInitialize) {
    auto* event1 = new testing::StrictMock<MockEvent>(5);
    auto* event2 = new testing::StrictMock<MockEvent>(10);
    
    // Neither event should be executed
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(0);
    
    // Both events should be destroyed
    EXPECT_CALL(*event1, die()).Times(1);
    EXPECT_CALL(*event2, die()).Times(1);
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    
    // Events should be destroyed when event_manager is initialized
    event_manager.initialize();
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, MemoryManagementOnDestruction) {
    auto* event1 = new testing::StrictMock<MockEvent>(5);
    auto* event2 = new testing::StrictMock<MockEvent>(10);
    
    // Expect destructor to be called for both events
    EXPECT_CALL(*event1, die()).Times(1);
    EXPECT_CALL(*event2, die()).Times(1);
    
    // Neither event should be executed
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(0);
    
    {
        EventManager local_event_manager;
        local_event_manager.schedule_event(event1);
        local_event_manager.schedule_event(event2);
    } // local_event_manager goes out of scope here, should destroy events
}

TEST_F(EventManagerTest, ExecuteEventsWithNegativeTime) {
    auto* event = new testing::StrictMock<MockEvent>(-5);  // Negative time event
    
    {
        testing::InSequence seq;
        EXPECT_CALL(*event, do_execute()).Times(1);  // Should still execute
        EXPECT_CALL(*event, die()).Times(1);  // Should be destroyed after execution
    }
    
    event_manager.schedule_event(event);
    event_manager.execute_events(0);  // Execute at time 0
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, ExecuteEventsWithSameTime) {
    // Multiple events at the same time should be executed in some order
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(10);
    auto* event3 = new testing::StrictMock<MockEvent>(10);
    
    // All events should be executed and destroyed, but order is not guaranteed
    EXPECT_CALL(*event1, do_execute()).Times(1);
    EXPECT_CALL(*event1, die()).Times(1);
    EXPECT_CALL(*event2, do_execute()).Times(1);
    EXPECT_CALL(*event2, die()).Times(1);
    EXPECT_CALL(*event3, do_execute()).Times(1);
    EXPECT_CALL(*event3, die()).Times(1);
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    event_manager.schedule_event(event3);
    
    event_manager.execute_events(10);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, CancelEventDuringExecution) {
    // Since execution order isn't guaranteed for same-time events,
    // we'll test cancellation with different times
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(11);
    auto* event3 = new testing::StrictMock<MockEvent>(12);
    
    {
        testing::InSequence seq;  // Order guaranteed by different times
        // event1 will cancel all events except event3
        EXPECT_CALL(*event1, do_execute())
            .WillOnce(::testing::Invoke([this, event3]() {
                event_manager.cancel_all_events_except(event3);
            }));
        EXPECT_CALL(*event1, die()).Times(1);
        
        // event2 should NOT execute as it was cancelled by event1
        EXPECT_CALL(*event2, do_execute()).Times(0);
        EXPECT_CALL(*event2, die()).Times(1);
        
        // event3 should execute as it was the exception
        EXPECT_CALL(*event3, do_execute()).Times(1);
        EXPECT_CALL(*event3, die()).Times(1);
    }
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    event_manager.schedule_event(event3);
    
    event_manager.execute_events(15);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, RescheduleEventDuringExecution) {
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(20);
    
    {
        testing::InSequence seq;
        // event1 will try to reschedule event2 to an earlier time
        EXPECT_CALL(*event1, do_execute())
            .WillOnce(::testing::Invoke([event2]() {
                event2->time = 15;  // This shouldn't affect the already scheduled event
            }));
        EXPECT_CALL(*event1, die()).Times(1);
        
        // event2 should still execute at its original time
        EXPECT_CALL(*event2, do_execute()).Times(1);
        EXPECT_CALL(*event2, die()).Times(1);
    }
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    
    event_manager.execute_events(10);  // Execute event1
    EXPECT_EQ(event_manager.get_events().size(), 1);  // event2 should still be there
    EXPECT_EQ(event_manager.get_events().begin()->first, 20);  // with original time
    
    event_manager.execute_events(20);  // Execute event2
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, ExecuteEventsWithMaxIntTime) {
    auto* event = new testing::StrictMock<MockEvent>(std::numeric_limits<int>::max());
    
    EXPECT_CALL(*event, do_execute()).Times(0);  // Should not execute yet
    
    event_manager.schedule_event(event);
    event_manager.execute_events(std::numeric_limits<int>::max() - 1);  // Should not execute
    EXPECT_FALSE(event_manager.get_events().empty());
    
    {
        testing::InSequence seq;
        EXPECT_CALL(*event, do_execute()).Times(1);  // Should execute now
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    event_manager.execute_events(std::numeric_limits<int>::max());  // Should execute
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, ExceptionChain) {
    // Test chain of events where multiple events throw exceptions
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(10);
    auto* event3 = new testing::StrictMock<MockEvent>(10);
    
    // All events should throw and be destroyed, but order is not guaranteed
    EXPECT_CALL(*event1, do_execute())
        .WillOnce(::testing::Throw(std::runtime_error("Error 1")));
    EXPECT_CALL(*event1, name())
        .WillOnce(::testing::Return("Event1"));
    EXPECT_CALL(*event1, die()).Times(1);
        
    EXPECT_CALL(*event2, do_execute())
        .WillOnce(::testing::Throw(std::runtime_error("Error 2")));
    EXPECT_CALL(*event2, name())
        .WillOnce(::testing::Return("Event2"));
    EXPECT_CALL(*event2, die()).Times(1);
        
    EXPECT_CALL(*event3, do_execute())
        .WillOnce(::testing::Throw(std::runtime_error("Error 3")));
    EXPECT_CALL(*event3, name())
        .WillOnce(::testing::Return("Event3"));
    EXPECT_CALL(*event3, die()).Times(1);
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    event_manager.schedule_event(event3);
    
    // Should handle all exceptions and continue
    EXPECT_NO_THROW(event_manager.execute_events(10));
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, InitializeWithPendingEvents) {
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(20);
    
    // Events should not execute
    EXPECT_CALL(*event1, do_execute()).Times(0);
    EXPECT_CALL(*event2, do_execute()).Times(0);
    
    // Both events should be destroyed during initialize
    EXPECT_CALL(*event1, die()).Times(1);
    EXPECT_CALL(*event2, die()).Times(1);
    
    event_manager.schedule_event(event1);
    event_manager.execute_events(5);  // Execute some events but not all
    event_manager.schedule_event(event2);
    
    // Initialize should clear all pending events
    event_manager.initialize();
    EXPECT_TRUE(event_manager.get_events().empty());
    
    // Further execution should not affect anything
    event_manager.execute_events(30);
    EXPECT_TRUE(event_manager.get_events().empty());
}

// Add a test for partial cancellation during execution
TEST_F(EventManagerTest, CancelLaterEventsOnlyDuringExecution) {
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(15);
    auto* event3 = new testing::StrictMock<MockEvent>(20);
    
    {
        testing::InSequence seq;
        // event1 will execute and then cancel later events
        EXPECT_CALL(*event1, do_execute())
            .WillOnce(::testing::Invoke([this]() {
                event_manager.cancel_all_events<MockEvent>();
            }));
        EXPECT_CALL(*event1, die()).Times(1);
        
        // Later events should not execute but should be destroyed
        EXPECT_CALL(*event2, do_execute()).Times(0);
        EXPECT_CALL(*event2, die()).Times(1);
        
        EXPECT_CALL(*event3, do_execute()).Times(0);
        EXPECT_CALL(*event3, die()).Times(1);
    }
    
    event_manager.schedule_event(event1);
    event_manager.schedule_event(event2);
    event_manager.schedule_event(event3);
    
    // Execute first event
    event_manager.execute_events(10);
    EXPECT_EQ(event_manager.get_events().size(), 2);  // Later events still in queue
    
    // Try to execute later events
    event_manager.execute_events(20);
    EXPECT_TRUE(event_manager.get_events().empty());  // Events removed but not executed
}

TEST_F(EventManagerTest, BatchProcessingAtTimePoint) {
    // Create multiple events at different time points
    std::vector<testing::StrictMock<MockEvent>*> events_t10;  // Events at t=10
    std::vector<testing::StrictMock<MockEvent>*> events_t20;  // Events at t=20
    
    // Add 5 events at t=10
    for (int i = 0; i < 5; ++i) {
        auto* event = new testing::StrictMock<MockEvent>(10);
        events_t10.push_back(event);
    }
    
    // Add 3 events at t=20
    for (int i = 0; i < 3; ++i) {
        auto* event = new testing::StrictMock<MockEvent>(20);
        events_t20.push_back(event);
    }
    
    // Set expectations for t=10 events - order not guaranteed
    for (auto* event : events_t10) {
        EXPECT_CALL(*event, do_execute()).Times(1);
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    // Set expectations for t=20 events - order not guaranteed
    for (auto* event : events_t20) {
        EXPECT_CALL(*event, do_execute()).Times(1);
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    // Schedule all events in mixed order
    event_manager.schedule_event(events_t20[0]);
    event_manager.schedule_event(events_t10[0]);
    event_manager.schedule_event(events_t10[1]);
    event_manager.schedule_event(events_t20[1]);
    event_manager.schedule_event(events_t10[2]);
    event_manager.schedule_event(events_t20[2]);
    event_manager.schedule_event(events_t10[3]);
    event_manager.schedule_event(events_t10[4]);
    
    // Execute events at t=10
    event_manager.execute_events(10);
    EXPECT_EQ(event_manager.get_events().size(), 3);  // Only t=20 events should remain
    
    // Execute events at t=20
    event_manager.execute_events(20);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, BatchDeletionAtTimePoint) {
    // Create multiple events at different time points
    std::vector<testing::StrictMock<MockEvent>*> events_t10;  // Events at t=10
    std::vector<testing::StrictMock<MockEvent>*> events_t20;  // Events at t=20
    
    // Add 5 events at t=10
    for (int i = 0; i < 5; ++i) {
        auto* event = new testing::StrictMock<MockEvent>(10);
        events_t10.push_back(event);
    }
    
    // Add 3 events at t=20
    for (int i = 0; i < 3; ++i) {
        auto* event = new testing::StrictMock<MockEvent>(20);
        events_t20.push_back(event);
    }
    
    // Set expectations for t=10 events - all should be destroyed together
    for (auto* event : events_t10) {
        EXPECT_CALL(*event, do_execute()).Times(1);
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    // Set expectations for t=20 events - all should be destroyed together
    for (auto* event : events_t20) {
        EXPECT_CALL(*event, do_execute()).Times(1);
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    // Schedule all events
    for (auto* event : events_t10) {
        event_manager.schedule_event(event);
    }
    for (auto* event : events_t20) {
        event_manager.schedule_event(event);
    }
    
    // Execute and verify batch deletion
    event_manager.execute_events(10);
    EXPECT_EQ(event_manager.get_events().size(), 3);  // t=20 events should remain
    
    // Execute remaining events
    event_manager.execute_events(20);
    EXPECT_TRUE(event_manager.get_events().empty());
}

// Add a new test specifically for FIFO order when needed
TEST_F(EventManagerTest, ExecuteEventsInFIFOOrder) {
    // Test that events are executed in FIFO order when using different times
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(11);
    auto* event3 = new testing::StrictMock<MockEvent>(12);
    
    {
        testing::InSequence seq;  // Verify strict execution order
        EXPECT_CALL(*event1, do_execute()).Times(1);
        EXPECT_CALL(*event1, die()).Times(1);
        EXPECT_CALL(*event2, do_execute()).Times(1);
        EXPECT_CALL(*event2, die()).Times(1);
        EXPECT_CALL(*event3, do_execute()).Times(1);
        EXPECT_CALL(*event3, die()).Times(1);
    }
    
    // Schedule in mixed order but should execute in time order
    event_manager.schedule_event(event3);  // t=12
    event_manager.schedule_event(event1);  // t=10
    event_manager.schedule_event(event2);  // t=11
    
    event_manager.execute_events(15);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, ExecuteEmptyEventManager) {
    // Test executing events on an empty event_manager
    EXPECT_NO_THROW(event_manager.execute_events(0));
    EXPECT_NO_THROW(event_manager.execute_events(100));
    EXPECT_NO_THROW(event_manager.execute_events(-1));
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, EventTypeFiltering) {
    class SpecialMockEvent : public MockEvent {
    public:
        explicit SpecialMockEvent(const int time) : MockEvent(time) {}
    };

    // Add different types of events
    auto* regular_event = new testing::StrictMock<MockEvent>(10);
    auto* special_event = new testing::StrictMock<SpecialMockEvent>(10);
    
    EXPECT_CALL(*regular_event, die()).Times(1);
    EXPECT_CALL(*special_event, die()).Times(1);
    
    event_manager.schedule_event(regular_event);
    event_manager.schedule_event(special_event);
    
    // Test type filtering
    EXPECT_TRUE(event_manager.has_event<Event>());
    EXPECT_TRUE(event_manager.has_event<MockEvent>());
    EXPECT_TRUE(event_manager.has_event<SpecialMockEvent>());
    
    // Cancel only SpecialMockEvents
    event_manager.cancel_all_events<SpecialMockEvent>();
    
    // Regular event should still be executable
    EXPECT_CALL(*regular_event, do_execute()).Times(1);
    EXPECT_CALL(*special_event, do_execute()).Times(0);
    
    event_manager.execute_events(10);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(EventManagerTest, NonMonotonicTimeExecution) {
    // Test that events with timestamps before current execution time are properly handled
    auto* event1 = new testing::StrictMock<MockEvent>(10);
    auto* event2 = new testing::StrictMock<MockEvent>(5);
    auto* event3 = new testing::StrictMock<MockEvent>(15);
    
    // Set expectations:
    // - event2 (t=5) should be executed at t=10 since we never execute at t=5
    // - event1 (t=10) should execute at t=10
    // - event3 (t=15) should execute at t=15
    {
        testing::InSequence seq;  // Enforce order since we know event timing
        
        // At t=10, both event2 (t=5) and event1 (t=10) should execute
        EXPECT_CALL(*event2, do_execute()).Times(1);  // Past event executes first
        EXPECT_CALL(*event2, die()).Times(1);
        EXPECT_CALL(*event1, do_execute()).Times(1);  // Current time event executes
        EXPECT_CALL(*event1, die()).Times(1);
        
        // At t=15, event3 executes
        EXPECT_CALL(*event3, do_execute()).Times(1);
        EXPECT_CALL(*event3, die()).Times(1);
    }
    
    // Schedule events in any order
    event_manager.schedule_event(event1);  // t=10
    event_manager.schedule_event(event2);  // t=5
    event_manager.schedule_event(event3);  // t=15
    
    // Execute at t=10, should execute both event2 (past) and event1 (current)
    event_manager.execute_events(10);
    EXPECT_EQ(event_manager.get_events().size(), 1);  // Only event3 (t=15) should remain
    
    // Try to execute at earlier time (t=5), should not affect anything
    event_manager.execute_events(5);
    EXPECT_EQ(event_manager.get_events().size(), 1);  // Still only event3
    
    // Execute remaining event at t=15
    event_manager.execute_events(15);
    EXPECT_TRUE(event_manager.get_events().empty());
} 