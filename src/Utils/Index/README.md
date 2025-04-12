# Person Indexing

This module implements the person indexing system for efficient lookup and management of individuals in the malaria simulation.

## Directory Structure

### Core Files
- `Indexer.h`: Base indexer template class
- `PersonIndex.h/cpp`: Abstract base class for person indexing
- `PersonIndexAll.h/cpp`: Complete population index implementation

### Specialized Indexes
- `PersonIndexByLocationStateAgeClass.h/cpp`: Multi-dimensional index by location, state, and age
- `PersonIndexByLocationMovingLevel.h/cpp`: Movement tracking index implementation

### Index Handlers
- `PersonIndexAllHandler.h/cpp`: Global index management
- `PersonIndexByLocationStateAgeClassHandler.h/cpp`: Complex index handling
- `PersonIndexByLocationMovingLevelHandler.h/cpp`: Movement index management

## Implementation Details

### Base Indexer (`Indexer.h`)
```cpp
namespace utils {
class Indexer {
public:
    // Disallow copy and move
    Indexer(const Indexer&) = delete;
    Indexer& operator=(const Indexer&) = delete;
    Indexer(Indexer&&) = delete;
    Indexer& operator=(Indexer&&) = delete;

    // Core functionality
    [[nodiscard]] size_t get_index() const;
    void set_index(const size_t& value);

private:
    size_t index_{};
};
}
```

### Person Index Base Class (`PersonIndex.h`)
```cpp
class PersonIndex {
public:
    // Disallow copy and move
    PersonIndex(const PersonIndex&) = delete;
    PersonIndex& operator=(const PersonIndex&) = delete;

    // Pure virtual interface
    virtual void add(Person* p) = 0;
    virtual void remove(Person* p) = 0;
    virtual std::size_t size() const = 0;
    virtual void update() = 0;
    virtual void notify_change(Person* p, const Person::Property& property,
                             const void* oldValue, const void* newValue) = 0;
};
```

### Location-State-Age Index (`PersonIndexByLocationStateAgeClass.h`)
```cpp
class PersonIndexByLocationStateAgeClass : public PersonIndex {
public:
    // Core operations
    void add(Person* p) override;
    void remove(Person* p) override;
    void update() override;
    
    // Specialized queries
    [[nodiscard]] PersonPtrVector& vPerson();
    [[nodiscard]] size_t size(const int& location, const int& state,
                             const int& age_class) const;
};
```

## Usage Examples

### Basic Indexing
```cpp
// Create and initialize indexes
auto all_index = std::make_unique<PersonIndexAll>();
auto location_index = std::make_unique<PersonIndexByLocationStateAgeClass>();

// Add person to indexes
all_index->add(person);
location_index->add(person);

// Query index
auto count = location_index->size(location, state, age_class);
```

### Movement Tracking
```cpp
// Create movement index
auto movement_index = std::make_unique<PersonIndexByLocationMovingLevel>();

// Track person movement
movement_index->add(person);
movement_index->notify_change(person, Person::LOCATION, &old_loc, &new_loc);
```

## Implementation Guidelines

1. **Thread Safety**
   - All index operations must be thread-safe
   - Use appropriate synchronization
   - Handle concurrent modifications
   - Protect shared state

2. **Performance**
   - Optimize lookup operations
   - Use efficient data structures
   - Minimize memory overhead
   - Cache frequently accessed data

3. **Error Handling**
   - Validate all inputs
   - Handle edge cases
   - Report errors through logging
   - Maintain index consistency

4. **Memory Management**
   - Use smart pointers
   - Clean up resources
   - Prevent memory leaks
   - Handle circular references

## Dependencies

### Internal
- `Person` class
- `Population` system
- `Model` context
- Base `Indexer` class

### External
- STL containers
- Threading support
- Logging system

## Notes

- All index classes follow modern C++ guidelines
- Thread safety is essential for all operations
- Performance is critical for large populations
- Proper error handling is required
- Documentation must be maintained
- Unit tests are essential
- Memory efficiency is important 