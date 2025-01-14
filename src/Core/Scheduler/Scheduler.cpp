#include "Scheduler.h"

#include <Configuration/Config.h>

Scheduler::Scheduler(Model* model)
    : current_time_(-1), total_available_time_(-1), model_(model), is_force_stop_(false) {}

Scheduler::~Scheduler() {
  clear_all_events();
}

void Scheduler::extend_total_time(int new_total_time) {
  if (total_available_time_ < new_total_time) {
    // for (auto i = total_available_time_; i <= new_total_time; i++) {
    //   individual_events_list_.push_back(EventPtrVector());
    //   population_events_list_.push_back(EventPtrVector());
    // }
  }
  total_available_time_ = new_total_time;
}

void Scheduler::clear_all_events() {
  // clear_all_events(individual_events_list_);
  // clear_all_events(population_events_list_);
}

void Scheduler::initialize(const date::year_month_day& starting_date, const date::year_month_day& ending_date) {
  int total_time = TimeHelpers::days_between(starting_date, ending_date);
  set_total_available_time(total_time + 720); // Prevent scheduling at the end of simulation
  set_current_time(0);
  calendar_date = date::sys_days(starting_date);
}

// Methods not declared in the header have been removed

void Scheduler::run() {
  current_time_ = 0;
  for (current_time_ = 0; !can_stop(); current_time_++) {
    if (current_time_ % model_->get_config()->get_model_settings().get_days_between_stdout_output() == 0) {
      spdlog::info("Day: {}", current_time_);
    }
    begin_time_step();
    daily_update();
    end_time_step();
    calendar_date += date::days{1};
  }
}

void Scheduler::begin_time_step() const {
  if (model_ != nullptr) {
    model_->begin_time_step();
  }
}

void Scheduler::daily_update() const {
  if (model_ != nullptr) {
    model_->daily_update();

    if (is_today_first_day_of_month()) {
      model_->monthly_update();
    }

    if (is_today_first_day_of_year()) {
      model_->yearly_update();
    }

    // Executing population-related events
    // execute_events_list(population_events_list_[current_time_]);

    // Executing individual-related events
    // execute_events_list(individual_events_list_[current_time_]);
  }
}

void Scheduler::end_time_step() const {
  if (model_ != nullptr) {
    model_->end_time_step();
  }
}

bool Scheduler::can_stop() const {
  return current_time_ > model_->get_config()->get_simulation_timeframe().get_total_time();
}

int Scheduler::current_day_in_year() const {
  return TimeHelpers::day_of_year(calendar_date);
}

date::month Scheduler::current_month_in_year() const {
  return TimeHelpers::month_of_year(calendar_date);
}

bool Scheduler::is_today_last_day_of_year() const {
  date::year_month_day ymd{calendar_date};
  return ymd.month() == date::month{12} && ymd.day() == date::day{31};
}

bool Scheduler::is_today_first_day_of_month() const {
  date::year_month_day ymd{calendar_date};
  return ymd.day() == date::day{1};
}

bool Scheduler::is_today_first_day_of_year() const {
  date::year_month_day ymd{calendar_date};
  return ymd.month() == date::month{1} && ymd.day() == date::day{1};
}

bool Scheduler::is_today_last_day_of_month() const {
  const auto next_date = calendar_date + date::days{1};
  date::year_month_day ymd{next_date};
  return ymd.day() == date::day{1};
}
