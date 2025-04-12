# Model Data Collector (MDC)

This module provides comprehensive data collection and statistical analysis functionality for the simulation model. It tracks and analyzes various metrics related to disease progression, treatment outcomes, and population dynamics.

## Directory Structure

### Core Files
- `ModelDataCollector.h/cpp`: Main implementation of data collection and analysis
  - Comprehensive data collection functionality
  - Statistical analysis methods
  - Real-time metric tracking
  - Performance-optimized implementation

### Documentation
- `README.md`: This documentation file

## Implementation Details

### ModelDataCollector Class
The `ModelDataCollector` class (`ModelDataCollector.h/cpp`) provides:

#### Data Collection
- Population statistics by location and age
- Disease progression metrics
- Treatment outcomes and drug resistance
- Transmission dynamics
- Mutation and recombination events

#### Statistical Analysis
- Blood slide prevalence calculation
- EIR (Entomological Inoculation Rate) computation
- Treatment failure rates
- Drug resistance frequencies
- Population dynamics metrics

#### Performance Features
- Optimized data structures
- Efficient calculation methods
- Memory-conscious design
- Thread-safe operations

## Key Components

### Population Metrics
```cpp
class ModelDataCollector {
public:
    // Population tracking
    void update_person_population_by_state();
    void update_total_population();
    
    // Disease metrics
    void collect_1_clinical_episode(const int& location, const int& age, const int& age_class);
    void record_1_treatment(const int& location, const int& age, const int& age_class,
                          const int& therapy_id);
    
    // Resistance tracking
    void record_1_mutation(const int& location, Genotype* from_genotype,
                         Genotype* to_genotype);
};
```

### Statistical Methods
```cpp
class ModelDataCollector {
public:
    // Prevalence calculations
    [[nodiscard]] double get_blood_slide_prevalence(const int& location,
                                                   const int& age_from,
                                                   const int& age_to) const;
    
    // Treatment analysis
    [[nodiscard]] double get_treatment_failure_rate(const int& therapy_id) const;
    
    // Transmission metrics
    void calculate_eir();
    void calculate_percentage_bites_on_top_20();
};
```

## Usage Examples

### Basic Data Collection
```cpp
// Initialize collector
auto collector = std::make_unique<ModelDataCollector>();
collector->initialize();

// Record events
collector->collect_1_clinical_episode(location, age, age_class);
collector->record_1_treatment(location, age, age_class, therapy_id);

// Calculate statistics
collector->monthly_update();
auto prevalence = collector->get_blood_slide_prevalence(location, 0, 100);
```

### Advanced Analysis
```cpp
// Resistance tracking
collector->record_1_mutation(location, from_genotype, to_genotype);
auto resistance_freq = collector->get_resistance_frequency(location, drug_id);

// Transmission analysis
collector->calculate_eir();
auto eir = collector->get_eir_by_location(location);
```

## Implementation Guidelines

1. **Thread Safety**
   - All collection methods are thread-safe
   - Use appropriate synchronization
   - Protect shared data
   - Handle concurrent updates

2. **Performance**
   - Use efficient data structures
   - Optimize calculation methods
   - Minimize memory allocations
   - Cache frequently used data

3. **Memory Management**
   - Use smart pointers
   - Clean up resources properly
   - Prevent memory leaks
   - Handle large datasets efficiently

4. **Error Handling**
   - Validate all inputs
   - Handle edge cases
   - Log errors appropriately
   - Maintain data consistency

## Dependencies

### Internal
- `Model` class
- `Person` system
- `Genotype` system
- `ClonalParasitePopulation`

### External
- STL containers
- Threading support
- Logging system
- Statistical libraries

## Notes

- All metrics are updated in real-time
- Thread safety is essential
- Memory efficiency is critical
- Performance optimization is important
- Documentation must be maintained
- Unit tests are required
- Data validation is crucial 