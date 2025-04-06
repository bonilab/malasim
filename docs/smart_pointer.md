# Guidelines for Using Smart Pointers (Focus on `std::unique_ptr`)

## Key Principles

- **Avoid Raw `new` and `delete`:** Directly managing memory with `new` and `delete` is error-prone (leaks, double frees). Let smart pointers handle resource lifetime automatically.
- **Embrace RAII with Smart Pointers:** Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) to ensure resources are acquired and released correctly, even in the presence of exceptions. Resource lifetime is tied to the scope of the smart pointer.
- **Prefer Factory Functions (`make_unique`, `make_shared`):** Use `std::make_unique<T>(...)` and `std::make_shared<T>(...)` instead of constructing smart pointers directly with `new`. This is safer (especially `make_unique` for exception safety) and often clearer.

---

Since `std::unique_ptr` is the primary tool for resource management in this project, these guidelines focus on its effective use.

## `std::unique_ptr`: Exclusive Ownership

- **Purpose:** Represents exclusive ownership of a dynamically allocated resource. When the `std::unique_ptr` goes out of scope, the resource it owns is automatically deleted.
- **Default Choice:** Use `std::unique_ptr` by default when you need dynamic allocation with single ownership.

- **Creation:** Always prefer `std::make_unique` to create `std::unique_ptr`. It's safer (exception safety) and often clearer than raw `new`.
  ```cpp
  // In Person::initialize
  immune_system_ = std::make_unique<ImmuneSystem>(this);

  // In Person event scheduling
  auto event = std::make_unique<BirthdayEvent>(this);
  ```

## Transferring Ownership with `std::unique_ptr`

- **Passing to Functions (Sink):** To transfer ownership *into* a function, pass `std::unique_ptr<T>` *by value*. Use `std::move` at the call site.
  ```cpp
  // Function takes ownership
  void EventManager::schedule_event(std::unique_ptr<EventType> event) {
      // ... manages event ...
      events_.emplace(event->get_time(), std::move(event)); 
  }

  // Caller transfers ownership
  auto event_ptr = std::make_unique<BirthdayEvent>(this);
  event_manager_.schedule_event(std::move(event_ptr)); 
  // event_ptr is now null or in a valid but unspecified state
  ```
- **The Role of `std::move`:** Using `std::move` is mandatory when passing a `std::unique_ptr` to a function that takes it by value (as shown above) to transfer ownership. 
    - **Compiler Enforcement:** Attempting to pass without `std::move` results in a compile error because `std::unique_ptr` is non-copyable (to enforce unique ownership).
    - **Explicit Intent:** `std::move` explicitly signals the intent to transfer ownership from the caller to the function.
    - **Safety:** This explicitness prevents accidental ownership transfers, making resource management safer and clearer.
- **Returning from Functions:** To transfer ownership *out of* a function (e.g., factory functions), return `std::unique_ptr<T>` by value.
  ```cpp
  std::unique_ptr<Widget> create_widget(/*...*/) {
      auto widget_ptr = std::make_unique<Widget>(/*...*/);
      // ... setup ...
      return widget_ptr; // Ownership is implicitly moved out
  }
  ```

## Non-Owning Access to `unique_ptr`-Managed Objects

When a function or another object needs to *use* or *observe* an object owned by a `std::unique_ptr` without taking ownership, use raw pointers (`T*`) or references (`T&`).

- **Use Raw Pointers (`T*`):** Preferred when the accessed object might be null (optional observation) or when interacting with C-style APIs.
    - **Getting a Non-Owning Pointer:** Use the `.get()` method of `std::unique_ptr`.
      ```cpp
      // In Person.h - Getter returns non-owning pointer
      ImmuneSystem* get_immune_system() { return immune_system_.get(); }
      ```
    - **Passing Non-Owning Pointer to Functions:**
      ```cpp
      // EventManager::cancel_event expects a non-owning pointer
      void EventManager::cancel_event(EventType* event);

      // Person needs non-owning access to Population
      void Person::set_population(Population* population);
      ```
- **Use References (`T&`):** Preferred when the accessed object is guaranteed to be non-null.
    - **Passing Non-Owning Reference to Functions:**
      ```cpp
      // Example (general pattern)
      void process_config(const Config& config);
      ```
    - **Returning Non-Owning Reference:** Ensure the referenced object (often a member) outlives the reference.
      ```cpp
      // In EventManager.h
      std::multimap<int, std::unique_ptr<EventType>>& get_events();
      ```

**Important:** When using non-owning raw pointers or references derived from a `unique_ptr`, ensure they do not outlive the owning `unique_ptr`.

## Alternatives for Shared Ownership (Less Common Here)

- **`std::shared_ptr`:** Use when multiple independent parts of the code need to share ownership of a single resource. The resource is deleted only when the last `shared_ptr` owning it is destroyed. (Note: Not currently prevalent in `Person`/`EventManager` code).
- **`std::weak_ptr`:** Use in conjunction with `shared_ptr` to break ownership cycles. It provides temporary, non-owning access by attempting to `lock()` into a `shared_ptr`.

## Resource Management in Classes (Rule of Zero)

- If your class manages ownership of resources *only* through smart pointer members (like `std::unique_ptr`), you often don't need to define custom destructors or copy/move operations (constructors/assignment operators). The compiler-generated versions usually handle the smart pointers correctly.
- (Note: `Person`/`EventManager` explicitly delete copy/move operations, which prevents copying/moving and is also a valid approach when those operations are not desired for the class.)