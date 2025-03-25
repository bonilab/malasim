# Events

This module provides the core event system and individual event implementations for the simulation. These events handle various aspects of disease progression, treatment, and individual behavior in the simulation.

## Core Components

### Base Event Class
- `Event.h/cpp`: Base class for all events in the system
  - Provides common functionality for event scheduling and execution
  - Defines the interface that all events must implement

## Event Categories

### Disease Progression Events
- `ProgressToClinicalEvent`: Handles progression to clinical symptoms
- `MatureGametocyteEvent`: Manages gametocyte maturation process
- `MoveParasiteToBloodEvent`: Controls parasite movement to bloodstream
- `EndClinicalEvent`: Manages the end of clinical symptoms
- `EndClinicalByNoTreatmentEvent`: Handles symptom resolution without treatment
- `EndClinicalDueToDrugResistanceEvent`: Manages treatment failure due to resistance

### Treatment Events
- `ReceiveTherapyEvent`: Handles individual treatment administration
- `ReceiveMDATherapyEvent`: Manages Mass Drug Administration treatment
- `TestTreatmentFailureEvent`: Tests for treatment failure
- `ReportTreatmentFailureDeathEvent`: Reports deaths due to treatment failure

### Individual Movement Events
- `CirculateToTargetLocationNextDayEvent`: Manages individual movement between locations
- `ReturnToResidenceEvent`: Handles return to residence location

### Lifecycle Events
- `BirthdayEvent`: Manages individual aging
- `SwitchImmuneComponentEvent`: Controls immune system changes

### Monitoring Events
- `UpdateEveryKDaysEvent`: Periodic update event
- `UpdateWhenDrugIsPresentEvent`: Drug presence monitoring
- `RaptEvent`: Rapid Assessment of Parasite Treatment event

## Usage

Events are typically created and scheduled through the simulation system. Each event has specific parameters and timing requirements.

### Example Event Creation
```cpp
// Creating and scheduling a therapy event
auto therapy_event = new ReceiveTherapyEvent(
    individual,
    therapy_id,
    dosing_days,
    time_of_event
);
scheduler->schedule_event(therapy_event);

// Creating a clinical progression event
auto clinical_event = new ProgressToClinicalEvent(
    individual,
    parasite_density,
    time_of_event
);
scheduler->schedule_event(clinical_event);
```

## Event Implementation Guidelines

1. **Timing**
   - Events must specify their execution time
   - Can be scheduled for immediate or future execution
   - May create follow-up events as needed

2. **State Changes**
   - Events should handle all related state changes
   - Must maintain simulation consistency
   - Should log important state transitions

3. **Error Handling**
   - Events should validate their parameters
   - Must handle edge cases gracefully
   - Should report errors through the logging system

## Dependencies

- Core simulation components:
  - `Model`
  - `Individual`
  - `Population`
  - `DrugDatabase`
- Utility classes:
  - `Config`
  - `Random`
  - `ModelDataCollector`

## File Structure

### Core Files
- `Event.h/cpp`: Base event class definition
- Individual event implementations (.h/.cpp pairs)

### Event Types
- Disease progression events
- Treatment-related events
- Movement events
- Lifecycle events
- Monitoring events

## Notes

- Events are executed in chronological order
- Events may create other events during execution
- All events must be memory managed by the scheduler
- Events should be designed for reusability
- Proper error handling and logging is essential
- Events should be thread-safe when applicable
