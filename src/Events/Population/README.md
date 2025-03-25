# Events/Population

This module provides a comprehensive framework for managing population-level events in the simulation system. It includes a wide range of events related to parasite introduction, mutation, treatment strategies, and population dynamics.

## Overview

The Events/Population module consists of multiple components organized into several categories:

### Event Builder

#### PopulationEventBuilder
A factory class that constructs various population-level events from YAML configuration.
- Core functionality for creating all population-related events
- Follows the factory pattern for event construction
- Provides extensive validation of configuration nodes

### Event Categories

#### 1. Parasite Introduction Events
- `ImportationEvent`: Basic parasite importation
- `ImportationPeriodicallyEvent`: Periodic parasite introduction
- `ImportationPeriodicallyRandomEvent`: Random periodic importation
- `DistrictImportationDailyEvent`: Daily district-level importation
- `IntroduceParasitesPeriodicallyEventV2`: Advanced periodic parasite introduction

#### 2. Mutation Events
- `IntroduceMutantEventBase`: Base class for mutant introduction
- `Introduce580YMutantEvent`: Specific 580Y mutation introduction
- `IntroduceAmodiaquineMutantEvent`: Amodiaquine resistance mutation
- `IntroduceLumefantrineMutantEvent`: Lumefantrine resistance mutation
- `IntroduceTripleMutantToDPMEvent`: Triple mutant introduction
- `IntroducePlas2CopyParasiteEvent`: Plas2 copy variant introduction
- `TurnOnMutationEvent`: Enables mutation
- `TurnOffMutationEvent`: Disables mutation
- `ChangeMutationProbabilityPerLocusEvent`: Modifies mutation rates
- `ChangeMutationMaskEvent`: Updates mutation masks

#### 3. Treatment Strategy Events
- `ChangeTreatmentStrategyEvent`: Modifies treatment approach
- `ChangeTreatmentCoverageEvent`: Updates treatment coverage
- `SingleRoundMDAEvent`: Mass Drug Administration event
- `ModifyNestedMFTEvent`: Multiple First-line Treatment modification
- `RotateStrategyEvent`: Treatment strategy rotation

#### 4. Environmental and Population Dynamic Events
- `UpdateBetaRasterEvent`: Updates transmission parameters
- `AnnualBetaUpdateEvent`: Yearly transmission updates
- `AnnualCoverageUpdateEvent`: Yearly coverage updates
- `ChangeCirculationPercentEvent`: Modifies circulation rates
- `ChangeInterruptedFeedingRateEvent`: Updates feeding rates

## Usage

### Configuration-based Event Creation
```yaml
# Mutation Control Events
- name: turn_on_mutation
  info:
    - date: 2000/1/1

- name: change_mutation_mask
  info:
    - date: 2000/1/1
      mutation_mask: "||||111||1000000,0||||||0000000000000|1"
    - date: 2001/1/1
      mutation_mask: "||||111||1000000,0||||||0010000000001|1"

- name: change_mutation_probability_per_locus
  info:
    - date: 2000/1/1
      mutation_probability_per_locus: 0.001

# Treatment Strategy Events Example
- name: change_treatment_strategy
  info:
    - date: 2000/3/2
      strategy_id: 11
    - date: 2005/6/2
      strategy_id: 3

# Mass Drug Administration Example
- name: single_round_MDA
  info:
    - date: 2001/1/1
      fraction_population_targeted: [1.0]
      days_to_complete_all_treatments: 14

# Treatment Coverage Example
- name: change_treatment_coverage
  info:
    - type: SteadyTCM
      date: 2005/2/1
      p_treatment_under_5_by_location: [0.600000]
      p_treatment_over_5_by_location: [0.550000]
    - type: InflatedTCM
      date: 2006/3/1
      annual_inflation_rate: 0.01

# Parasite Introduction Example
- name: introduce_parasites
  info:
    - location: 0
      parasite_info:
        - date: 2002/3/20
          genotype_aa_sequence: "||||YY1||KTHFIMG,x||||||FNCMYRIPRPYRA|1"
          number_of_cases: 5

# Mutant Introduction Example
- name: introduce_580Y_parasites
  info:
    - location: 1
      date: 2002/7/15
      fraction: 0.2
      alleles:
        - chromosome: 13
          locus: 11
          allele: "Y"
        - chromosome: 13
          locus: 12
          allele: "I"
```

### Code Example
```cpp
// Using PopulationEventBuilder
YAML::Node config = YAML::LoadFile("population_events.yaml");
auto events = PopulationEventBuilder::build(config);

// Events will be automatically executed by the simulation
// at their scheduled times
```

## Dependencies

- `yaml-cpp`: Configuration parsing
- `spdlog`: Logging functionality
- Core simulation components:
  - `Model`
  - `Config`
  - Base `Event` class
  - Various strategy and mutation-related classes

## Directory Structure

### Main Components
- `PopulationEventBuilder.h/cpp`: Main event factory implementation
- Various event implementation files (.h/.cpp pairs)
- Event-specific header files (.hxx)

### Subdirectories
- `EventBuilders/`: Additional specialized event builders

## Notes

- All events are configured through YAML configuration files
- Events are executed automatically by the simulation system
- Most events support location-specific targeting
- Extensive logging is implemented for debugging and monitoring
- Events can be combined and scheduled in any order
- Configuration validation ensures proper event setup
