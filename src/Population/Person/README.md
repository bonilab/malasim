# Person

This module implements the individual-level modeling for the malaria simulation, representing each person in the population with their characteristics and state.

## Overview

The Person module manages:
- Individual demographics and characteristics
- Health and infection states
- Movement and location tracking
- Treatment and immunity history
- Clinical progression

## Key Features

### Individual Properties
- Age and demographics
- Location and residence
- Biting level
- Movement patterns
- Host state (Susceptible, Infected, etc.)

### Health Management
- Clinical state tracking
- Infection history
- Treatment records
- Immune status
- Drug concentrations

### Movement System
- Current location
- Home location
- Movement probabilities
- Travel patterns
- Circulation status

### Treatment Tracking
- Drug administration
- Treatment history
- Drug resistance encounters
- Treatment outcomes
- Side effects

## Implementation

### State Management
```cpp
class Person {
    int birth_time_;
    int age_;
    int location_;
    int residence_location_;
    double biting_level_;
    HostStates host_state_;
    
    // Health-related components
    ImmuneSystem* immune_system_;
    DrugsInBlood* drugs_in_blood_;
    ClonalParasitePopulations* all_clonal_parasite_populations_;
};
```

### Key Methods
- State transitions
- Age updates
- Location changes
- Treatment administration
- Infection handling
- Immunity updates

## Usage

```cpp
// Create new person
auto person = new Person(
    birth_time,
    location,
    residence_location,
    biting_level,
    host_state
);

// Update person state
person->update();

// Handle movement
person->change_location(new_location);

// Administer treatment
person->receive_therapy(therapy_id);

// Check infection status
bool is_infected = person->has_infection();
```

## Dependencies

- Core components:
  - `ImmuneSystem`
  - `DrugsInBlood`
  - `ClonalParasitePopulations`
- Configuration:
  - `Config`
  - `Model`
- Event system for state changes

## Notes

- Person objects are the core unit of simulation
- State changes trigger appropriate events
- Memory management is critical
- Multiple indexes track different properties
- Movement affects transmission dynamics
- Treatment history affects outcomes
- Age affects immune response
