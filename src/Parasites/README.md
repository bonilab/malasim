# Parasites

This module implements the genetic system for malaria parasites (P. falciparum) in the simulation, including genotype representation, mutation, drug resistance, and database management.

## Overview

The Parasites module is responsible for:
- Managing parasite genotypes and their properties
- Handling drug resistance mechanisms
- Implementing mutation and recombination
- Tracking fitness costs
- Managing the genotype database

## Key Components

### Genotype System
- Represents P. falciparum genetic information
- Implements 14-chromosome structure
- Manages amino acid sequences
- Handles allele combinations
- Tracks fitness costs

### Drug Resistance
- EC50 calculations for different drugs
- Resistance pattern matching
- Drug-specific mutation handling
- Multiple resistance tracking
- Resistance evolution modeling

### Mutation and Recombination
- Drug-induced mutations
- Locus-specific mutations
- Free recombination implementation
- Fitness impact calculations
- Genotype combination logic

### Genotype Database
- Efficient genotype storage
- Amino acid sequence mapping
- ID-based lookup system
- EC50 tracking by drug
- Weight-based selection

## Data Structures

### Genotype Representation
```cpp
class Genotype {
    int genotype_id_;                    // Unique identifier
    PfGenotypeStr pf_genotype_str;      // 14 chromosomes
    std::string aa_sequence;             // Amino acid sequence
    double daily_fitness_multiple_infection;
    std::vector<double> EC50_power_n;    // Drug resistance levels
};
```

### Database Organization
```cpp
class GenotypeDatabase {
    std::map<ul, Genotype*> genotype_map;
    std::map<std::string, Genotype*> aa_sequence_id_map;
    std::map<int, std::map<std::string,double>> drug_id_ec50;
};
```

## Usage

### Genotype Management
```cpp
// Create new genotype
auto genotype = new Genotype("||||YY1||KTHFIMG,x||||||FNCMYRIPRPYRA|1");

// Check drug resistance
bool is_resistant = genotype->resist_to(drug_type);

// Perform mutation
auto mutated = genotype->perform_mutation_by_drug(
    config,
    random,
    drug_type,
    mutation_probability
);

// Recombine genotypes
auto recombined = Genotype::free_recombine(
    config,
    random,
    genotype1,
    genotype2
);
```

### Database Operations
```cpp
// Add genotype to database
database.add(genotype);

// Retrieve by sequence
auto found = database.get_genotype(aa_sequence);

// Get minimum EC50 for drug
double min_ec50 = database.get_min_ec50(drug_id);
```

## Implementation Details

### Genotype Properties
- Unique ID system
- Amino acid sequence representation
- Chromosomal structure
- Fitness calculations
- EC50 computations

### Mutation System
- Drug-specific mutations
- Probability-based changes
- Fitness impact tracking
- Resistance development
- Pattern validation

### Database Management
- Efficient lookup mechanisms
- Memory optimization
- Thread safety considerations
- ID generation system
- Reference counting

## Dependencies

- Core simulation components:
  - `Config`
  - `Random`
  - `DrugDatabase`
- Configuration classes:
  - `GenotypeParameters`
  - `DrugType`
  - `Therapy`
- Utility classes:
  - Vector types from `TypeDef.hxx`

## Notes

- Genotypes are immutable once created
- Memory management is critical for large simulations
- EC50 values determine drug effectiveness
- Fitness costs affect parasite survival
- Database performance impacts simulation speed
- Mutation rates are configurable
- Recombination follows biological rules
- Drug resistance patterns are validated
