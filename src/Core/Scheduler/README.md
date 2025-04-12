# Scheduler

The Scheduler module is responsible for managing and executing events in the simulation system. It provides a robust framework for scheduling, managing, and executing time-based events in a simulation environment.

## Directory Contents

### Core Components
- `Scheduler.h/cpp`: Main scheduler implementation
  - Event scheduling and execution
  - Time management
  - Calendar operations
  - Simulation control flow

- `EventManager.h/cpp`: Event queue management
  - Priority-based event queue
  - Event storage and retrieval
  - Thread-safe operations
  - Event dependency tracking

## Implementation Details

### Scheduler Class
The `Scheduler` class (`Scheduler.h/cpp`) provides:

#### Time Management
- Current simulation time tracking
- Total simulation time management
- Time step execution control
- Calendar-based date operations
- Thread-safe time updates

#### Event Scheduling
- Population event scheduling
- Individual event scheduling
- Event cancellation support
- Event queue management
- Thread-safe scheduling operations

#### Calendar Operations
- Current day/month/year tracking
- First/last day checks for:
  - Month
  - Year
  - Custom periods
- Daily update support

### Event Manager Class
The `EventManager` class (`EventManager.h/cpp`) handles:

#### Event Queue
- Priority-based event storage
- Efficient event retrieval
- Thread-safe queue operations
- Event dependency management

#### Event Processing
- Event execution ordering
- Event cancellation
- Queue cleanup
- Resource management

## Usage Example

```cpp
// Initialize the scheduler with start and end dates
auto scheduler = std::make_unique<Scheduler>();
scheduler->initialize(start_date, end_date);

// Schedule events
auto event = std::make_unique<MyEvent>(parameters);
scheduler->schedule_event(std::move(event));

// Run the simulation
scheduler->run();

// Check simulation state
auto current_time = scheduler->current_time();
bool is_month_end = scheduler->is_last_day_of_month();
```

## Implementation Guidelines

1. **Thread Safety**
   - All public methods are thread-safe
   - Use appropriate synchronization
   - Protect shared resources
   - Handle concurrent access

2. **Memory Management**
   - Use smart pointers for events
   - RAII for resource management
   - Proper cleanup in destructors
   - No memory leaks

3. **Error Handling**
   - Validate all inputs
   - Handle edge cases
   - Report errors through logging
   - Maintain simulation stability

## Dependencies

### Internal
- Event system
- Configuration system
- Logging system
- Model context

### External
- `date` library for calendar operations
- STL for containers and algorithms
- Threading support

## Notes

- Non-copyable and non-movable design
- Thread-safe operations
- Exception-safe implementation
- Comprehensive logging
- Performance-optimized
- Extensible design
