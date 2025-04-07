# Simulation

This module implements the core simulation model and execution system for the malaria simulation.

## Overview

The Simulation module provides:
- Main simulation model
- Execution control
- State management
- Component coordination
- System integration

## Components

### Model
- `Model`: Core simulation model
  - State management
  - Component coordination
  - Event handling
  - Time progression
  - Data collection

### Main Program
- `main.cpp`: Entry point
  - Configuration loading
  - Model initialization
  - Execution control
  - Output management
  - Resource cleanup

## Key Features

### Simulation Control
- Model initialization
- Execution flow
- State management
- Event coordination
- Resource management

### Integration
- Component coordination
- System integration
- Data flow management
- Resource allocation
- Error handling

## Implementation

### Model Structure
```cpp
class Model {
public:
    // Singleton access
    static Model* get_instance();

    // Initialization
    bool initialize();

    // Simulation control
    void before_run();
    void run();
    void after_run();
    void begin_time_step();
    void end_time_step();
    void daily_update();

    // Configuration and component access (static getters/setters)
    static Config* get_config();
    static void set_config(Config* config);
    // ... other getters/setters ...

private:
    // Private constructor for singleton
    Model() = default;
    
    // ... private members ...
};
```

## Usage

```cpp
// Get the model instance
auto model = Model::get_instance();

// Assume 'config' is loaded elsewhere
// Model::set_config(config); // Configuration might be set during initialization steps

// Initialize model
model->initialize();

// Run the full simulation
model->run();

// Note: Step-by-step execution is typically managed externally, 
// potentially calling methods like model->begin_time_step(), 
// model->daily_update(), model->end_time_step() in a loop.

// this step is optional, but it you want to re-used the model instance for another run
// or expirement,... run release to reset the model class
model->release();

```

## Key Systems

### State Management
- Component states
- System state
- Transition handling
- State validation
- History tracking

### Event Processing
- Event queue
- Handler dispatch
- Priority management
- Timing control
- Event validation

## Dependencies

- Core components:
  - `Config`
  - `Population`
  - `Environment`
- Support systems:
  - `Events`
  - `Reporters`
  - `MDC`

## Notes

- Central coordination point
- Performance critical
- Memory management important
- Error handling crucial
- State consistency required
- Event ordering matters
- Resource cleanup essential
- Documentation needed
