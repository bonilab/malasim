# Mosquito Module

This module implements the mosquito population dynamics and parasite transmission between mosquitoes and humans in the malaria simulation model.

## Directory Structure

### Core Files
- `Mosquito.h` (2.4KB): Core class declarations and interfaces
  - Population management
  - Transmission dynamics
  - Genetic components
  - Recombination mechanisms

- `Mosquito.cpp` (14KB): Implementation of mosquito dynamics
  - Population calculations
  - Transmission algorithms
  - Genetic operations
  - Performance optimizations

- `README.md`: Module documentation

## Implementation Details

### Mosquito Class
```cpp
class Mosquito {
public:
    // Population Management
    void initialize(const Config* config);
    void update_force_of_infection(const int& location, const double& force_of_infection);
    
    // Transmission Dynamics
    void infect_new_cohort_in_PRMC(const Config* config,
                                  Random* random,
                                  Population* population,
                                  const int& tracking_index);
    
    // Genetic Operations
    [[nodiscard]] int random_genotype(const int& location,
                                     const int& tracking_index) const;
    
    // Monitoring and Statistics
    [[nodiscard]] size_t get_mosquito_count(const int& location) const;
    void update_mosquito_population();
};
```

## Key Features

### Population Management
- Dynamic population sizing by location
- Cohort-based tracking system
- Temporal population changes
- Environmental factor integration

### Transmission Dynamics
- Human-to-mosquito transmission modeling
- Interrupted feeding simulation
- Complex infection probability calculations
- Force of infection implementation

### Genetic Components
- Multi-genotype parasite tracking
- Sophisticated recombination mechanisms
- Drug resistance evolution monitoring
- Mutation pattern analysis

### Recombination System

#### Recombination Scenarios
1. **Complete Recombination** (Within-Host=1, Interrupted=1)
   - Cross-host parasite mixing
   - Multiple genotype sampling
   - Complex recombination patterns

2. **Internal Recombination** (Within-Host=1, Interrupted=0)
   - Single-host multiple genotypes
   - Internal genetic mixing
   - Diversity maintenance

3. **Simple Cross-Host** (Within-Host=0, Interrupted=1)
   - Basic two-host mixing
   - Single genotype per host
   - Direct recombination

4. **No Recombination** (Within-Host=0, Interrupted=0)
   - Single genotype preservation
   - Clonal transmission

## Usage Examples

### Basic Operations
```cpp
// Initialize mosquito system
auto mosquito = std::make_unique<Mosquito>(model);
mosquito->initialize(config);

// Update population dynamics
mosquito->update_mosquito_population();
auto count = mosquito->get_mosquito_count(location);

// Handle transmission
mosquito->update_force_of_infection(location, calculated_foi);
```

### Advanced Features
```cpp
// Genetic operations
auto genotype = mosquito->random_genotype(location, tracking_index);

// Complex transmission
mosquito->infect_new_cohort_in_PRMC(
    config,
    random,
    population,
    tracking_index
);
```

## Implementation Guidelines

1. **Population Management**
   - Use efficient data structures
   - Implement proper synchronization
   - Handle boundary conditions
   - Validate population changes

2. **Transmission Modeling**
   - Ensure accurate probability calculations
   - Handle edge cases
   - Implement proper random sampling
   - Maintain transmission consistency

3. **Performance Optimization**
   - Use efficient algorithms
   - Optimize memory usage
   - Cache frequent calculations
   - Profile critical sections

4. **Thread Safety**
   - Protect shared resources
   - Use appropriate locks
   - Handle concurrent updates
   - Maintain data consistency

## Dependencies

### Internal
- `Model` system
- `Config` management
- `Population` tracking
- `Random` number generation
- `Genotype` system
- `ClonalParasitePopulation`

### External
- STL containers
- Math libraries
- Threading support
- Logging system

## Notes

- Thread-safe implementation required
- Memory efficiency is critical
- Performance optimization is important
- Regular validation needed
- Comprehensive logging implemented
- Unit tests coverage essential
- Documentation maintenance required 