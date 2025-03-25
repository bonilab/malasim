# Drug Therapies

This module implements the drug therapy system for malaria treatment, including drug types, combinations, and their effects on parasites.

## Overview

The Drug Therapies module provides:
- Drug type definitions
- Therapy combinations
- Drug database management
- Resistance tracking
- Treatment effects

## Components

### Core Systems
- `Drug`: Individual drug implementation
- `DrugType`: Drug type definitions
- `Therapy`: Base therapy class
- `DrugDatabase`: Drug information storage

### Therapy Types
- `SCTherapy`: Single Compound therapy
- `MACTherapy`: Multiple Active Compounds
- `TherapyBuilder`: Therapy construction

## Implementation

### Drug System
```cpp
class Drug {
    string name_;
    double half_life_;
    double maximum_parasite_killing_rate_;
    double ec50_;
    
    double calculate_drug_effect();
    void decay_drug_concentration();
};
```

### Therapy System
```cpp
class Therapy {
    vector<Drug*> drugs_;
    int dosing_days_;
    
    void apply_therapy();
    double calculate_total_effect();
};
```

## Usage

### Drug Creation
```cpp
// Create new drug
auto drug = new Drug("Artemether");
drug->set_half_life(3.0);
drug->set_killing_rate(0.999);

// Add to database
drug_db->add_drug(drug);
```

### Therapy Administration
```cpp
// Create therapy
auto therapy = new SCTherapy();
therapy->add_drug(drug);
therapy->set_dosing_days(3);

// Apply to person
therapy->apply_to_person(person);
```

## Key Features

### Drug Properties
- Half-life calculation
- Killing rates
- EC50 values
- Concentration tracking
- Decay modeling

### Therapy Effects
- Combined drug effects
- Resistance impact
- Treatment duration
- Side effects
- Efficacy tracking

### Database Management
- Drug information storage
- Property tracking
- Resistance patterns
- Usage statistics
- Effect monitoring

## Dependencies

### Core Components
- `Person`
- `Parasites`
- `Config`
- `Random`

### Support Systems
- `Events`
- `Statistics`
- `Logging`
- `Database`

## Notes

### Drug Implementation
- Model pharmacokinetics
- Track concentrations
- Handle resistance
- Monitor effects
- Log outcomes
- Validate parameters
- Test combinations
- Document interactions

### Performance Considerations
- Optimize calculations
- Cache common values
- Monitor memory usage
- Profile critical paths
- Handle concurrency
- Validate inputs
- Clean up resources
- Update efficiently

### Safety Guidelines
- Validate drug parameters
- Check combinations
- Monitor side effects
- Track outcomes
- Log treatments
- Handle errors
- Document changes
- Test thoroughly
