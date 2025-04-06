#include "EventManagerTestCommon.h"

class ExecutionOrderTest : public EventManagerTestBase {};

TEST_F(ExecutionOrderTest, ExecuteEventsInFIFOOrder) {
    auto event1 = std::make_unique<testing::StrictMock<MockEvent>>(10);
    auto event2 = std::make_unique<testing::StrictMock<MockEvent>>(11);
    auto event3 = std::make_unique<testing::StrictMock<MockEvent>>(12);
    
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
    event_manager.schedule_event(std::move(event3));  // t=12
    event_manager.schedule_event(std::move(event1));  // t=10
    event_manager.schedule_event(std::move(event2));  // t=11
    
    event_manager.execute_events(15);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(ExecutionOrderTest, ExecuteEventsWithSameTime) {
    auto event1 = std::make_unique<testing::StrictMock<MockEvent>>(10);
    auto event2 = std::make_unique<testing::StrictMock<MockEvent>>(10);
    auto event3 = std::make_unique<testing::StrictMock<MockEvent>>(10);
    
    // All events should be executed and destroyed
    // do_execute calls can happen in any order, but must all happen before any die calls
    {
        testing::InSequence seq;
        
        // All executions happen first, in any order
        EXPECT_CALL(*event1, do_execute()).Times(1);
        EXPECT_CALL(*event2, do_execute()).Times(1);
        EXPECT_CALL(*event3, do_execute()).Times(1);
        
        // All destructions happen after executions, in any order
        EXPECT_CALL(*event1, die()).Times(1);
        EXPECT_CALL(*event2, die()).Times(1);
        EXPECT_CALL(*event3, die()).Times(1);
    }
    
    event_manager.schedule_event(std::move(event1));
    event_manager.schedule_event(std::move(event2));
    event_manager.schedule_event(std::move(event3));
    
    event_manager.execute_events(10);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(ExecutionOrderTest, BatchProcessingAtTimePoint) {
    std::vector<std::unique_ptr<testing::StrictMock<MockEvent>>> events_t10;  // Events at t=10
    std::vector<std::unique_ptr<testing::StrictMock<MockEvent>>> events_t20;  // Events at t=20
    
    // Add 5 events at t=10
    for (int i = 0; i < 5; ++i) {
        auto event = std::make_unique<testing::StrictMock<MockEvent>>(10);
        events_t10.push_back(std::move(event));
        event_manager.schedule_event(std::move(event));
    }
    
    // Add 3 events at t=20
    for (int i = 0; i < 3; ++i) {
        auto event = std::make_unique<testing::StrictMock<MockEvent>>(20);
        events_t20.push_back(std::move(event));
        event_manager.schedule_event(std::move(event));
    }
    
    // Set expectations for t=10 events - order not guaranteed
    for (auto& event : events_t10) {
        EXPECT_CALL(*event, do_execute()).Times(1);
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    // Set expectations for t=20 events - order not guaranteed
    for (auto& event : events_t20) {
        EXPECT_CALL(*event, do_execute()).Times(1);
        EXPECT_CALL(*event, die()).Times(1);
    }
    
    // Schedule all events in mixed order
    for (size_t i = 0; i < std::max(events_t10.size(), events_t20.size()); ++i) {
        if (i < events_t20.size()) event_manager.schedule_event(std::move(events_t20[i]));
        if (i < events_t10.size()) event_manager.schedule_event(std::move(events_t10[i]));
    }
    
    // Execute events at t=10
    event_manager.execute_events(10);
    EXPECT_EQ(event_manager.get_events().size(), 3);  // Only t=20 events should remain
    
    // Execute events at t=20
    event_manager.execute_events(20);
    EXPECT_TRUE(event_manager.get_events().empty());
}

TEST_F(ExecutionOrderTest, NonMonotonicTimeExecution) {
    auto event1 = std::make_unique<testing::StrictMock<MockEvent>>(10);
    auto event2 = std::make_unique<testing::StrictMock<MockEvent>>(5);
    auto event3 = std::make_unique<testing::StrictMock<MockEvent>>(15);
    
    {
        testing::InSequence seq;
        
        // At t=10, both event2 (t=5) and event1 (t=10) should execute
        EXPECT_CALL(*event2, do_execute()).Times(1);  // Past event executes first
        EXPECT_CALL(*event2, die()).Times(1);
        EXPECT_CALL(*event1, do_execute()).Times(1);  // Current time event executes
        EXPECT_CALL(*event1, die()).Times(1);
        
        // At t=15, event3 executes
        EXPECT_CALL(*event3, do_execute()).Times(1);
        EXPECT_CALL(*event3, die()).Times(1);
    }
    
    event_manager.schedule_event(std::move(event1));  // t=10
    event_manager.schedule_event(std::move(event2));  // t=5
    event_manager.schedule_event(std::move(event3));  // t=15
    
    event_manager.execute_events(10);
    EXPECT_EQ(event_manager.get_events().size(), 1);
    
    event_manager.execute_events(5);  // Should not affect anything
    EXPECT_EQ(event_manager.get_events().size(), 1);
    
    event_manager.execute_events(15);
    EXPECT_TRUE(event_manager.get_events().empty());
} 