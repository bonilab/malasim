# Reporters

This module implements various reporting and data collection systems for the malaria simulation, providing different ways to monitor, validate, and analyze simulation results.

## Overview

The Reporters module provides:
- Multiple output formats and destinations
- Real-time monitoring capabilities
- Data validation tools
- Specialized analysis reporters
- Database integration

## Key Components

### Base System
- `Reporter`: Base class for all reporters
- `SQLiteDbReporter`: Core database reporting
- `ConsoleReporter`: Real-time console output

### Periodic Reporters
- `MonthlyReporter`: Monthly statistics
- `SQLiteMonthlyReporter`: Monthly database records
- `MMCReporter`: Mass Medical Campaign reporting

### Specialized Reporters
- `ValidationReporter`: Data validation and verification
- `TACTReporter`: Triple Artemisinin Combination Therapy analysis
- `NovelDrugReporter`: New drug intervention studies
- `TravelTrackingReporter`: Population movement analysis

## Key Features

### Data Collection
- Population statistics
- Disease progression
- Treatment outcomes
- Drug resistance
- Movement patterns
- Intervention effects

### Output Formats
- SQLite database
- Console output
- Validation reports
- Statistical summaries
- Custom formats

### Analysis Types
- Monthly aggregation
- Real-time monitoring
- Intervention analysis
- Movement tracking
- Resistance evolution

## Implementation

### Base Reporter Structure
```cpp
class Reporter {
    virtual void before_run();
    virtual void after_run();
    virtual void begin_time_step();
    virtual void after_time_step();
    virtual void monthly_report();
};
```

### Database Integration
```cpp
class SQLiteDbReporter {
    void create_tables();
    void insert_simulation_data();
    void update_simulation_stats();
    void export_to_database();
};
```

## Usage

### Basic Reporting
```cpp
// Initialize reporter
auto reporter = new ConsoleReporter();
reporter->before_run();

// Regular updates
reporter->begin_time_step();
reporter->after_time_step();

// Monthly reporting
reporter->monthly_report();

// Finalize
reporter->after_run();
```

### Database Reporting
```cpp
// Setup database reporter
auto db_reporter = new SQLiteMonthlyReporter(
    "simulation_results.db"
);

// Configure tables
db_reporter->create_tables();

// Record data
db_reporter->record_monthly_data();

// Export results
db_reporter->export_data();
```

## Specialized Features

### Validation
- Data consistency checks
- Statistical validation
- Model verification
- Output validation
- Historical comparison

### TACT Analysis
- Combination therapy effects
- Resistance development
- Treatment outcomes
- Efficacy analysis
- Population impact

### Movement Tracking
- Population circulation
- Travel patterns
- Location changes
- Movement impact
- Transmission dynamics

## Dependencies

- Core components:
  - `Model`
  - `Config`
  - `Population`
- Database:
  - `SQLite3`
- Utility classes:
  - Statistical functions
  - Data formatting
  - File handling

## Notes

- Reporters can be combined for comprehensive analysis
- Database reporters require proper indexing
- Memory usage should be monitored
- Regular data export is recommended
- Validation is crucial for accuracy
- Custom reporters can be implemented
- Output formats are configurable
- Performance impact should be considered
