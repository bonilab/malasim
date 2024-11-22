# Core Scheduler

The Scheduler module is responsible for managing the simulation's time progression and scheduling events. It interacts with the `Model` class to execute daily, monthly, and yearly updates.

## Files

- `Scheduler.h` and `Scheduler.cpp`: Defines the `Scheduler` class, which handles the simulation's time management and event scheduling.

## Classes

### Scheduler

The `Scheduler` class is responsible for:

- Managing the simulation's current time and total available time.
- Interacting with the `Model` class to perform updates.
- Handling the simulation's calendar date.
- Providing methods to start, run, and stop the simulation.

#### Methods

- `Scheduler(Model *model = nullptr)`: Constructor that initializes the scheduler with an optional `Model` instance.
- `~Scheduler()`: Destructor that clears all events.
- `void extend_total_time(int new_total_time)`: Extends the total available time for the simulation.
- `void clear_all_events()`: Clears all scheduled events.
- `void initialize(const date::year_month_day &starting_date, const int &total_time)`: Initializes the scheduler with a starting date and total time.
- `void run()`: Runs the simulation.
- `void begin_time_step() const`: Begins a time step in the simulation.
- `void end_time_step() const`: Ends a time step in the simulation.
- `bool can_stop() const`: Checks if the simulation can stop.
- `int current_day_in_year() const`: Returns the current day in the year.
- `int current_month_in_year() const`: Returns the current month in the year.
- `bool is_today_last_day_of_month() const`: Checks if today is the last day of the month.
- `bool is_today_first_day_of_month() const`: Checks if today is the first day of the month.
- `bool is_today_first_day_of_year() const`: Checks if today is the first day of the year.
- `bool is_today_last_day_of_year() const`: Checks if today is the last day of the year.
- `void daily_update() const`: Performs daily updates in the simulation.
