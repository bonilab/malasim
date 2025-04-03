#ifndef EVENT_MANAGER_TEST_COMMON_H
#define EVENT_MANAGER_TEST_COMMON_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Scheduler/EventManager.h"
#include "Events/Event.h"
#include "Population/Person/Person.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::InSequence;

// Mock Event class using GMock
class MockEvent : public PersonEvent {
public:
    explicit MockEvent(const int time) : PersonEvent(nullptr) {
        set_time(time);
    }

    MOCK_METHOD(const std::string, name, (), (const, override));
    MOCK_METHOD(void, do_execute, (), (override));
    MOCK_METHOD(void, die, ());  // Helper method to track destruction
    
    ~MockEvent() override {
        die();  // Call mock method in destructor
    }

protected:
    friend class Event;  // Allow base class to access protected members
};

// Base test fixture class
class EventManagerTestBase : public ::testing::Test {
protected:
    EventManager<PersonEvent> event_manager;

    void SetUp() override {
        event_manager.initialize();
    }

    void TearDown() override {
    }
};

#endif // EVENT_MANAGER_TEST_COMMON_H 