# Core

This module contains the core components and functionality that drive the malaria simulation system.

## Directory Structure

### Core Components
- `README.md`: This documentation file

### Subdirectories
- `Scheduler/`: Event scheduling and time management system
  - `Scheduler.h/cpp`: Main scheduler implementation
  - `EventManager.h/cpp`: Event queue management
  - `README.md`: Scheduler-specific documentation

## Key Components

### Scheduler System
The scheduler is the heart of the simulation, managing time progression and event execution:

#### Scheduler (`Scheduler.h/cpp`)
- Time management and progression
- Event scheduling and execution
- Calendar operations
- Simulation flow control
- Thread-safe event handling

#### Event Manager (`EventManager.h/cpp`)
- Event queue management
- Priority-based scheduling
- Event dependency tracking
- Efficient event storage and retrieval
- Thread-safe operations

## Implementation Guidelines

1. **Thread Safety**
   - All core operations must be thread-safe
   - Use appropriate synchronization mechanisms
   - Avoid race conditions
   - Handle concurrent access properly

2. **Performance**
   - Optimize critical paths
   - Use efficient data structures
   - Minimize memory allocations
   - Profile and benchmark regularly

3. **Error Handling**
   - Comprehensive error checking
   - Proper exception handling
   - Detailed error reporting
   - Graceful failure recovery

4. **Memory Management**
   - RAII principles
   - Smart pointer usage
   - Resource cleanup
   - Memory leak prevention

## Dependencies

### Internal Dependencies
- Event system
- Configuration system
- Logging system

### External Dependencies
- Standard Template Library (STL)
- Threading support
- Date/time utilities

## Usage Example

```cpp
// Initialize the scheduler
auto scheduler = std::make_unique<Scheduler>();

// Schedule an event
scheduler->schedule_event(std::make_unique<MyEvent>());

// Run the simulation
scheduler->run();
```

## Notes

- Core components are fundamental to simulation stability
- Changes require thorough testing
- Performance optimizations are critical
- Thread safety is essential
- Documentation must be maintained
- Backward compatibility is important
