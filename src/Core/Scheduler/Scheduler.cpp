#include "Scheduler.h"
#include <Configuration/Config.h>

#include "Events/Event.h"
#include "Population/Population.h"
#include "Utils/Helpers/ObjectHelpers.h"

Scheduler::Scheduler(Model* model)
    : current_time_(-1), total_available_time_(-1), model_(model), is_force_stop_(false) {}

Scheduler::~Scheduler() {
  clear_all_events();
}

void Scheduler::extend_total_time(int new_total_time) {
  if (total_available_time_ < new_total_time) {
    for (auto i = total_available_time_; i <= new_total_time; i++) {
      population_events_list_.push_back(EventPtrVector());
    }
  }
  total_available_time_ = new_total_time;
}

void Scheduler::clear_all_events() {
  clear_all_events(population_events_list_);
}

void Scheduler::set_total_available_time(const int& value) {
  if (total_available_time_ > 0) {
    clear_all_events();
  }
  total_available_time_ = value;
  population_events_list_.assign(total_available_time_, EventPtrVector());
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
    // spdlog::info("Scheduler Day: {}", current_time_);
    if (current_time_ % model_->get_config()->get_model_settings().get_days_between_stdout_output() == 0) {
      spdlog::info("Day: {}", current_time_);
    }
    begin_time_step();
    daily_update();
    end_time_step();
    calendar_date += date::days{1};
  }
}

void Scheduler::begin_time_step() {
  if (model_ != nullptr) {
    model_->begin_time_step();
  }
}

void Scheduler::daily_update() {
  if (model_ != nullptr) {

    model_->daily_update();

    if (is_today_first_day_of_month()) {
      model_->monthly_update();
    }

    if (is_today_first_day_of_year()) {
      model_->yearly_update();
    }

    // Executing population-related events
    execute_events_list(population_events_list_[current_time_]);

    // Executing individual-related events
    // execute_events_list(individual_events_list_[current_time_]);
    model_->get_population()->update_all_individual_events();
  }
}

void Scheduler::end_time_step() {
  if (model_ != nullptr) {
    model_->end_time_step();
  }
}

bool Scheduler::can_stop() {
  return current_time_ > model_->get_config()->get_simulation_timeframe().get_total_time();
}

int Scheduler::current_day_in_year() {
  return TimeHelpers::day_of_year(calendar_date);
}

int Scheduler::current_month_in_year() {
  return TimeHelpers::month_of_year(calendar_date);
}

bool Scheduler::is_today_last_day_of_year() {
  date::year_month_day ymd{calendar_date};
  return ymd.month() == date::month{12} && ymd.day() == date::day{31};
}

bool Scheduler::is_today_first_day_of_month() {
  date::year_month_day ymd{calendar_date};
  return ymd.day() == date::day{1};
}

bool Scheduler::is_today_first_day_of_year() {
  date::year_month_day ymd{calendar_date};
  return ymd.month() == date::month{1} && ymd.day() == date::day{1};
}

bool Scheduler::is_today_last_day_of_month() {
  const auto next_date = calendar_date + date::days{1};
  date::year_month_day ymd{next_date};
  return ymd.day() == date::day{1};
}

// void Scheduler::schedule_individual_event(Event* event) {
  // schedule_event(individual_events_list_[event->time], event);
// }

void Scheduler::schedule_population_event(Event* event) {
  if (event->time < population_events_list_.size()) {
    schedule_event(population_events_list_[event->time], event);
  }
}

void Scheduler::schedule_event(EventPtrVector& time_events, Event* event) {
  // Schedule event in the future
  // Event time cannot exceed total time or less than current time
  if (event->time > Model::get_config()->get_simulation_timeframe().get_total_time() || event->time < current_time_) {
    if (event->time < current_time()) {
      spdlog::error("Error when schedule event {} at {}. Current_time: {} - total time: {}",
      event->name(), event->time, current_time_, total_available_time_);
      spdlog::error("Cannot schedule event {} at {}. Current_time: {} - total time: {}",
        event->name(), event->time, current_time_, total_available_time_);
    }
    ObjectHelpers::delete_pointer<Event>(event);
  } else {
    time_events.push_back(event);
    event->scheduler = this;
    event->executable = true;
  }
}

void Scheduler::clear_all_events(EventPtrVector2& events_list) {
  for (auto& timestep_events : events_list) {
    for (auto* event : timestep_events) {
      // TODO: consider using dispatcher to manage events
      // maybe population events will be removed
      ObjectHelpers::delete_pointer<Event>(event);
    }
    timestep_events.clear();
  }
  events_list.clear();
}

void Scheduler::cancel(Event* event) {
  event->executable = false;
}

void Scheduler::execute_events_list(EventPtrVector& events_list) {
  for (auto& event : events_list) {
    // std::cout << "execute_events_list " << event->name() << " id: " << event->get_id() << std::endl;
    event->execute();
    ObjectHelpers::delete_pointer<Event>(event);
  }
  ObjectHelpers::clear_vector_memory<Event>(events_list);
}