# Events/Environment

This module provides event handling functionality for environmental changes in the simulation system. It contains classes for building and executing environment-related events.

## Overview

The Events/Environment module consists of several components that handle environmental changes during simulation runtime:

### Components

#### EnvironmentEventBuilder
A factory class that constructs environment-related events from configuration.
- Features:
  - YAML configuration parsing
  - Event construction from configuration nodes
- Key Methods:
  - `build()`: Creates events from YAML configuration
  - `build_update_ecozone_event()`: Specifically builds ecozone update events

#### UpdateEcozoneEvent
An event class that handles changes in ecological zones during simulation.
- Features:
  - Updates ecozone types from one type to another
  - Integrates with the simulation's seasonality system
- Key Components:
  - Source ecozone type (`from_`)
  - Target ecozone type (`to_`)
  - Execution time (`time`)
- Implementation:
  - Inherits from base `Event` class
  - Updates seasonal equations when executed
  - Logs changes using spdlog

## Usage

### Configuration-based Event Creation
```yaml
# Example YAML configuration
update_ecozone:
  from: 1    # Source ecozone type
  to: 2      # Target ecozone type
  time: 100  # When to execute the event
```

### Code Example
```cpp
// Using EnvironmentEventBuilder
YAML::Node config = YAML::LoadFile("config.yaml");
auto events = EnvironmentEventBuilder::build(config);

// Events will be automatically executed by the simulation
// at their scheduled times
```

## Dependencies

- `yaml-cpp`: For configuration parsing
- `spdlog`: For logging
- Core simulation components:
  - `Model`
  - `Config`
  - `SeasonalitySettings`
  - Base `Event` class

## File Structure

- `EnvironmentEventBuilder.h/cpp`: Event factory implementation
- `UpdateEcozoneEvent.hxx`: Ecozone update event definition
- `README.md`: This documentation file

## Notes

- Events are executed automatically by the simulation system
- Ecozone updates affect the seasonality calculations
- All changes are logged for monitoring and debugging
- Configuration-driven approach allows for flexible event scheduling
