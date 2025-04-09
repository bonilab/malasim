# Object Pool Usage

This document explains how to use the `ObjectPool` template class provided in `src/Utils/ObjectPool.h`. The object pool is designed to efficiently manage the allocation and deallocation of objects, particularly useful for frequently created and destroyed objects like `Person` or event types in the simulation.

It avoids repeated calls to `new` and `delete` by recycling objects.

## Overview

The `ObjectPool` allocates memory in chunks and manages a list of available (free) objects. When an object is requested (`acquire_object`), the pool provides one from its free list. If the list is empty, it allocates a new chunk.

Crucially, instead of returning raw pointers or standard smart pointers that would incorrectly `delete` the pooled object, `acquire_object` returns a `std::unique_ptr` with a **custom deleter**. This custom deleter ensures that when the `unique_ptr` goes out of scope (or is reset), the object is returned to the pool's free list instead of being deallocated.

## Declaring an Object Pool

You typically declare an object pool as a member of the class that manages the pooled objects. For example, the `Population` class might manage a pool of `Person` objects.

```c++
// In Population.h (or similar)
#include "Utils/ObjectPool.h"
#include "Population/Person/Person.h" // Assuming Person class definition

class Population {
    // ... other members ...

private:
    ObjectPool<Person> person_pool_;

    // Store the managed Person objects using the pool's unique_ptr type
    std::vector<ObjectPool<Person>::UniqueObjPtr> persons_;

    // ... rest of the class ...
};
```

## Acquiring an Object

Use the `acquire_object` method to get a managed unique pointer to a new or recycled object. You can pass constructor arguments just like you would with `std::make_unique` or `new`.

```c++
// Example within a Population method

void Population::add_new_person(/* potential args */) {
    // Acquire a Person object from the pool.
    // Pass any necessary constructor arguments for Person here.
    // For example, if Person has a constructor Person(int initial_age):
    // auto new_person_ptr = person_pool_.acquire_object(0); // Assuming 0 is the initial age

    // If Person has a default constructor:
    auto new_person_ptr = person_pool_.acquire_object();

    if (new_person_ptr) { // Check if acquisition was successful
        // Initialize the person further if needed
        new_person_ptr->set_location(some_location);
        new_person_ptr->set_population(this);
        // ... other setup ...

        // Move the unique_ptr into the storage container
        persons_.push_back(std::move(new_person_ptr));
    } else {
        // Handle the error case where the pool couldn't provide an object
        // (e.g., memory allocation failure)
        std::cerr << "Error: Failed to acquire person from pool!" << std::endl;
    }
}
```

## Releasing an Object (Automatic)

You **do not** need to manually release objects back to the pool. The custom deleter associated with the `ObjectPool<T>::UniqueObjPtr` handles this automatically. When the `unique_ptr` is destroyed (e.g., goes out of scope, is reset, or removed from a container), its deleter is invoked, which calls the pool's internal `release_object` method.

```c++
// Example within Population for removing a person

void Population::remove_person(Person* person_to_remove) {
    // Find the unique_ptr managing the person
    auto it = std::find_if(persons_.begin(), persons_.end(),
                           [person_to_remove](const auto& p) {
                               return p.get() == person_to_remove;
                           });

    if (it != persons_.end()) {
        // Erasing the unique_ptr from the vector triggers its destructor.
        // The custom deleter returns the Person object to the person_pool_.
        persons_.erase(it);
    }
}
```

## Using with Events

The same pattern applies to managing different types of events using an object pool. You would typically have a separate pool for each distinct event type if they have different sizes or allocation patterns.

```c++
#include "Utils/ObjectPool.h"
#include "Events/EventBase.h" // Assuming a base or specific event class

class EventScheduler {
    // Pool for a specific event type, e.g., MoveEvent
    ObjectPool<MoveEvent> move_event_pool_;

    // Container might store pointers to a base class, but acquired from specific pools
    std::vector<ObjectPool<MoveEvent>::UniqueObjPtr> scheduled_move_events_;

public:
    void schedule_move_event(/* args */) {
        auto new_event = move_event_pool_.acquire_object(/* MoveEvent constructor args */);
        if (new_event) {
            // Configure the event...
            scheduled_move_events_.push_back(std::move(new_event));
        } else {
            // Handle error
        }
    }

    // Removal/processing automatically returns the event to the pool
    void process_events() {
        // ... logic to find and process events ...
        // When an event unique_ptr is removed/reset, it goes back to the pool.
    }
};

```

## Key Points

1.  **Include Header:** `#include "Utils/ObjectPool.h"`
2.  **Declare Pool:** `ObjectPool<YourType> my_pool;`
3.  **Acquire:** `auto obj_ptr = my_pool.acquire_object(constructor_args...);`
    *   Returns `ObjectPool<YourType>::UniqueObjPtr`.
4.  **Store:** Store the `UniqueObjPtr` (e.g., in `std::vector`, `std::list`).
5.  **Release:** Automatic when the `unique_obj_ptr` is destroyed. No manual release needed.
