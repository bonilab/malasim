# ParasiteDensity

This module implements the parasite density calculations and updates for malaria infections, tracking parasite growth and clearance within hosts.

## Overview

The ParasiteDensity module manages:
- Parasite density calculations
- Growth rate modeling
- Clearance dynamics
- Drug effects on density
- Immunity interactions

## Key Components

### Update Functions
- `ParasiteDensityUpdateFunction`: Core density calculations
  - Growth rate application
  - Drug effect integration
  - Immunity modification
  - Density thresholds
  - Clinical progression

## Key Features

### Density Calculations
- Base growth rates
- Drug-induced reduction
- Immune system effects
- Density-dependent effects
- Clinical thresholds

### Dynamic Factors
- Drug concentrations
- Immune response levels
- Host characteristics
- Treatment timing
- Resistance effects

## Implementation

### Update Function Structure
```cpp
class ParasiteDensityUpdateFunction {
    double calculate_density_change(
        double current_density,
        double drug_effect,
        double immune_effect,
        double growth_rate
    );
    
    double apply_drug_effects(
        double density,
        const std::vector<double>& drug_concentrations
    );
};
```

### Key Calculations
- Growth rate application
- Drug effect integration
- Immunity modification
- Density thresholds
- Clinical progression

## Usage

```cpp
// Update parasite density
auto density_update = new ParasiteDensityUpdateFunction();

// Calculate new density
double new_density = density_update->calculate_density_change(
    current_density,
    drug_effect,
    immune_effect,
    growth_rate
);

// Check clinical threshold
bool is_clinical = new_density > clinical_threshold;
```

## Key Algorithms

### Density Updates
- Exponential growth modeling
- Drug effect calculations
- Immunity impact assessment
- Threshold evaluations
- Clinical state determination

### Effect Integration
- Drug concentration effects
- Immune response impact
- Host factor modification
- Resistance consideration
- Combined effect calculation

## Dependencies

- Core components:
  - `ClonalParasitePopulation`
  - `ImmuneSystem`
  - `DrugsInBlood`
- Configuration:
  - `Config`
  - `Model`

## Notes

- Updates occur at regular intervals
- Drug effects are concentration-dependent
- Immunity modifies growth rates
- Clinical thresholds are important
- Multiple factors affect density
- Performance is critical
- Accuracy affects clinical outcomes
