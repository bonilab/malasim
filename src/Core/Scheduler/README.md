# Scheduler

The Scheduler module is responsible for managing and executing events in the simulation system. It provides a robust framework for scheduling, managing, and executing time-based events in a simulation environment.

## Overview

The `Scheduler` class is the core component that handles:
- Event scheduling and execution
- Time management
- Calendar-based operations
- Simulation control flow

## Key Features

### Time Management
- Tracks current simulation time
- Manages total available simulation time
- Supports time step execution
- Handles calendar-based date operations

### Event Management
- Schedules population-level events
- Manages event execution lists
- Provides event cancellation capabilities
- Supports clearing of all scheduled events

### Calendar Operations
- Tracks current day and month in the year
- Provides utilities for checking first/last days of:
  - Month
  - Year
- Supports daily updates

### Simulation Control
- Initializes simulation with start and end dates
- Manages simulation execution flow
- Provides force stop capabilities
- Handles time step begin/end operations

## Usage

The Scheduler is typically used as follows:

```cpp
// Initialize the scheduler with start and end dates
scheduler.initialize(start_date, end_date);

// Schedule events
scheduler.schedule_population_event(event);

// Run the simulation
scheduler.run();
```

## Dependencies

- Requires a `Model` instance for simulation context
- Uses the `date` library for calendar operations
- Relies on event-related types defined in `Utils/TypeDef.hxx`

## Notes

- The Scheduler class is non-copyable and non-movable
- It maintains a list of population events for execution
- Supports dynamic extension of total simulation time
- Provides various utility methods for time and date management
