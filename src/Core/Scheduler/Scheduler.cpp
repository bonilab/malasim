#include "Scheduler.h"

#include <Configuration/Config.h>

#include <vector>

using namespace date;

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

void Scheduler::initialize(const date::year_month_day& starting_date, const int& total_time) {
  set_total_available_time(total_time + 720); // Prevent scheduling at the end of simulation
  set_current_time(0);
  calendar_date = sys_days(starting_date);
}

// Methods not declared in the header have been removed

void Scheduler::run() {
  // LOG(INFO) << "Simulation is running";
  current_time_ = 0;

  for (current_time_ = 0; !can_stop(); current_time_++) {
    spdlog::info("Day: {}", current_time_);
    begin_time_step();
    daily_update();
    end_time_step();
    calendar_date += days{1};
  }
}

void Scheduler::begin_time_step() const {
  if (model_ != nullptr) {
    // model_->begin_time_step();
  }
}

void Scheduler::daily_update() {
  if (model_ != nullptr) {
    // model_->daily_update();

    if (is_today_first_day_of_month()) {
      // model_->monthly_update();
    }

    if (is_today_first_day_of_year()) {
      // model_->yearly_update();
    }

    // Executing population-related events
    // execute_events_list(population_events_list_[current_time_]);

    // Executing individual-related events
    // execute_events_list(individual_events_list_[current_time_]);
  }
}

void Scheduler::end_time_step() const {
  if (model_ != nullptr) {
    // model_->end_time_step();
  }
}

bool Scheduler::can_stop() const {
  return current_time_ > model_->get_config()->get_simulation_timeframe().get_total_time();
}

int Scheduler::current_day_in_year() const {
  return utils::Time::instance().day_of_year(calendar_date);
}

int Scheduler::current_month_in_year() const {
  return utils::Time::instance().month_of_year(calendar_date);
}

bool Scheduler::is_today_last_day_of_year() const {
  year_month_day ymd{calendar_date};
  return ymd.month() == month{12} && ymd.day() == day{31};
}

bool Scheduler::is_today_first_day_of_month() const {
  year_month_day ymd{calendar_date};
  return ymd.day() == day{1};
}

bool Scheduler::is_today_first_day_of_year() const {
  year_month_day ymd{calendar_date};
  return ymd.month() == month{1} && ymd.day() == day{1};
}

bool Scheduler::is_today_last_day_of_month() const {
  const auto next_date = calendar_date + days{1};
  year_month_day ymd{next_date};
  return ymd.day() == day{1};
}
