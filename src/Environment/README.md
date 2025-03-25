# Environment

This module provides a framework for handling seasonal patterns and environmental factors in the simulation system. It implements various approaches to model seasonal variations that can affect simulation behavior.

## Overview

The Environment module consists of several classes that implement different seasonal pattern behaviors:

### Core Interface
- `ISeasonalInfo`: Base interface that defines the contract for all seasonal pattern implementations
  - Provides virtual `get_seasonal_factor()` method that all implementations must override
  - Takes a date and location as input to calculate seasonal factors

### Implementations

#### SeasonalPattern
The base implementation for location-based seasonal patterns.
- Features:
  - Supports both monthly and daily patterns
  - Handles administrative unit-based adjustments
  - Configurable through file-based input
- Key Methods:
  - `build()`: Initializes the pattern with spatial data
  - `read()`: Loads pattern data from file
  - `get_seasonal_factor()`: Calculates seasonal factor for given date and location

#### SeasonalEquation
Implements seasonal patterns using mathematical equations.
- Features:
  - Supports raster-based configurations
  - Uses trigonometric equations for seasonal variations
  - Configurable base, amplitude, and phase parameters
- Key Components:
  - Base value
  - Amplitude (A)
  - Secondary amplitude (B)
  - Phase shift (phi)
  - Support for reference values

#### SeasonalRainfall
Implements rainfall-based seasonal patterns.
- Features:
  - File-based configuration
  - Period-based adjustments
  - Simple rainfall factor calculations
- Key Methods:
  - `build()`: Initializes the rainfall pattern
  - `read()`: Loads rainfall data from file
  - `get_seasonal_factor()`: Calculates rainfall-based seasonal factor

#### SeasonalDisabled
A null implementation that disables seasonal effects.
- Returns constant values regardless of date or location
- Useful for testing or scenarios where seasonal effects should be ignored

## Usage

Each seasonal pattern implementation can be used by:
1. Creating an instance of the desired pattern class
2. Configuring it with appropriate parameters
3. Using `get_seasonal_factor()` to obtain seasonal adjustments for specific dates and locations

Example:
```cpp
// Using SeasonalPattern
auto pattern = SeasonalPattern();
pattern.set_period(12);  // Monthly pattern
pattern.set_is_monthly(true);
pattern.read("seasonal_data.txt");

// Get seasonal factor for a specific date and location
double factor = pattern.get_seasonal_factor(date, location_id);
```

## Dependencies

- `date` library for date handling
- `yaml-cpp` for configuration parsing (SeasonalEquation)
- `spdlog` for logging
- Custom spatial data handling classes

## File Structure

- `SeasonalInfo.h`: Core interface definition
- `SeasonalPattern.h/cpp`: Base pattern implementation
- `SeasonalEquation.h/cpp`: Equation-based implementation
- `SeasonalRainfall.h/cpp`: Rainfall-based implementation
- `SeasonalDisabled.h/cpp`: Null implementation
