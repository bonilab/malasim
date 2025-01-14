#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <chrono>
#include "date/date.h"
#include "Simulation/Model.h" // Assuming Model is defined in a separate header file

class Scheduler {
public:
  // Disable copy and assignment
  Scheduler(const Scheduler&) = delete;
  Scheduler& operator=(const Scheduler&) = delete;
  Scheduler(Scheduler&&) = delete;
  Scheduler& operator=(Scheduler&&) = delete;

  int current_time_;
  int total_available_time_;
  Model* model_;
  bool is_force_stop_;

  date::sys_days calendar_date;

  explicit Scheduler(Model *model = nullptr);

  virtual ~Scheduler();

  // Getter and Setter for current_time
  [[nodiscard]] int current_time() const { return current_time_; }
  void set_current_time(int time) { current_time_ = time; }

  // Getter and Setter for total_available_time
  [[nodiscard]] int total_available_time() const { return total_available_time_; }
  void set_total_available_time(int total_time) { total_available_time_ = total_time; }

  // Getter and Setter for model
  [[nodiscard]] Model* model() const { return model_; }
  void set_model(Model* model) { model_ = model; }

  // Getter and Setter for is_force_stop
  [[nodiscard]] bool is_force_stop() const { return is_force_stop_; }
  void set_is_force_stop(bool force_stop) { is_force_stop_ = force_stop; }

  void extend_total_time(int new_total_time);

  void clear_all_events();

//  void clear_all_events(EventPtrVector2 &events_list) const;
//
//  virtual void schedule_individual_event(Event *event);
//
//  virtual void schedule_population_event(Event *event);
//
//  virtual void schedule_event(EventPtrVector &time_events, Event *event);
//
//  virtual void cancel(Event *event);
//
//  void execute_events_list(EventPtrVector &events_list) const;

  void initialize(const date::year_month_day &starting_date,
    const date::year_month_day &ending_date);

  void run();

  void begin_time_step() const;

  void end_time_step() const;

  [[nodiscard]] bool can_stop() const;

  [[nodiscard]] int current_day_in_year() const;

  [[nodiscard]] date::month current_month_in_year() const;

  [[nodiscard]] bool is_today_last_day_of_month() const;

  [[nodiscard]] bool is_today_first_day_of_month() const;

  [[nodiscard]] bool is_today_first_day_of_year() const;

  [[nodiscard]] bool is_today_last_day_of_year() const;

  void daily_update() const;
};

#endif  /* SCHEDULER_H */
