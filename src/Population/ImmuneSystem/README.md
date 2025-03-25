# ImmuneSystem

This module implements the immune system modeling for malaria infection, including age-dependent immunity development and response to parasites.

## Overview

The ImmuneSystem module manages:
- Age-specific immune responses
- Immunity acquisition and decay
- Clinical protection
- Parasite density control
- Treatment interaction effects

## Components

### Base System
- `ImmuneSystem`: Core immunity management
- `ImmuneComponent`: Base class for immune responses
- `ImmunityClearanceUpdateFunction`: Immunity-based parasite clearance

### Age-Specific Components
- `InfantImmuneComponent`: Immunity for ages 0-5
  - Maternal antibody effects
  - Initial immunity development
  - Rapid response changes

- `NonInfantImmuneComponent`: Immunity for ages 5+
  - Long-term immunity
  - Exposure-based enhancement
  - Clinical protection

## Key Features

### Immunity Types
- Clinical immunity
- Parasite immunity
- Transmission immunity
- Maternal protection
- Drug-enhanced clearance

### Dynamic Responses
- Exposure-based development
- Age-dependent changes
- Decay over time
- Boost from infections
- Treatment interactions

## Implementation

### Immune System Structure
```cpp
class ImmuneSystem {
    ImmuneComponent* immune_component_;
    double immune_level_;
    double current_clinical_immunity_;
    double current_parasite_immunity_;
    int last_boost_update_age_;
};
```

### Update Functions
- Immunity level calculations
- Clinical protection updates
- Parasite density effects
- Drug interaction modifiers
- Clearance probability

## Usage

```cpp
// Initialize immune system
auto immune_system = new ImmuneSystem(age);

// Update immunity levels
immune_system->update();

// Calculate immune effects
double clinical_immunity = immune_system->get_clinical_immunity();
double parasite_immunity = immune_system->get_parasite_immunity();

// Handle infection interaction
immune_system->handle_infection(parasite_density);
```

## Key Algorithms

### Immunity Development
- Age-based progression
- Exposure-driven enhancement
- Maternal protection decay
- Clinical immunity acquisition
- Parasite immunity buildup

### Clearance Mechanisms
- Base clearance rates
- Immunity modification
- Drug interaction effects
- Age-specific factors
- Exposure history impact

## Dependencies

- Core components:
  - `Person`
  - `Config`
  - `Model`
- Disease components:
  - `ClonalParasitePopulation`
  - `DrugsInBlood`

## Notes

- Age significantly affects immunity
- Exposure history is crucial
- Multiple immunity types interact
- Drug effects are modified by immunity
- Memory management is important
- Update frequency affects accuracy
- Component selection is age-based
