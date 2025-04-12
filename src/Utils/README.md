# Utils

This module provides essential utility functions, tools, and helper classes used throughout the malaria simulation system.

## Directory Structure

### Core Files
- `Random.h/cpp`: Advanced random number generation and distribution sampling
- `TypeDef.h`: Common type definitions and aliases
- `ObjectPool.h`: Memory management and object pooling
- `Logger.h/cpp`: Logging system implementation
- `Constants.h`: System-wide constants
- `MultinomialDistributionGenerator.h/cpp`: Statistical distribution tools
- `YamlFile.h`: YAML configuration file handling
- `Cli.h`: Command line interface tools
- `MatrixWriter.hxx`: Matrix data output utilities

### Documentation
- `README.md`: This documentation file
- `Random.md`: Detailed documentation for random number generation

### Subdirectories
- `Index/`: Person and location indexing system
  - Base indexer implementation
  - Specialized index types
  - Index management utilities
  
- `Helpers/`: Utility functions and tools
  - String manipulation
  - Time utilities
  - OS operations
  - Number handling

## Key Components

### Random Number Generation (`Random.h/cpp`)
- Thread-safe singleton implementation
- Multiple distribution types:
  - Uniform
  - Normal
  - Poisson
  - Multinomial
  - Custom distributions
- Seed management
- Sequence generation

### Object Pool Management (`ObjectPool.h`)
```cpp
template<typename T>
class ObjectPool {
public:
    // Disallow copy and move
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    
    // Core functionality
    T* acquire();
    void release(T* obj);
    void cleanup();
    
private:
    // Implementation details...
};
```

### Type System (`TypeDef.h`)
- Common type aliases
- Container type definitions
- Forward declarations
- System-wide type configurations

### Logging System (`Logger.h/cpp`)
- Multiple log levels
- File and console output
- Thread-safe logging
- Configurable formats

## Usage Examples

### Random Number Generation
```cpp
// Get the random number generator instance
auto* random = Random::get_instance();

// Generate random values
double uniform = random->random_uniform();
int poisson = random->random_poisson(lambda);
double normal = random->random_normal(mean, sd);

// Sample from distribution
auto sample = random->sample_multinomial(probabilities);
```

### Object Pooling
```cpp
// Create a thread-safe object pool
auto pool = std::make_unique<ObjectPool<MyClass>>();

// Acquire and use an object
auto* obj = pool->acquire();
// ... use obj ...
pool->release(obj);
```

### Configuration Handling
```cpp
// Load and parse YAML configuration
YamlFile config("config.yml");
auto value = config.get<int>("parameter.value");

// Command line handling
Cli cli(argc, argv);
auto param = cli.get_param("--config");
```

## Implementation Guidelines

1. **Thread Safety**
   - Use thread-safe singletons
   - Protect shared resources
   - Use appropriate synchronization
   - Handle concurrent access

2. **Memory Management**
   - Use RAII principles
   - Implement proper cleanup
   - Prevent memory leaks
   - Use smart pointers

3. **Error Handling**
   - Validate all inputs
   - Use appropriate exceptions
   - Provide error context
   - Log error details

4. **Performance**
   - Optimize critical paths
   - Use efficient containers
   - Minimize allocations
   - Profile hot spots

## Dependencies

### Required
- C++17 or later
- STL
- `yaml-cpp`
- `spdlog`

### Optional
- `date` library
- Threading support
- Filesystem support

## Notes

- All utility classes follow modern C++ guidelines
- Thread safety is a primary concern
- Performance is critical for core utilities
- Comprehensive error handling is required
- Documentation must be maintained
- Unit tests are essential 