#ifndef MODEL_SETTINGS_H
#define MODEL_SETTINGS_H

#include <date/date.h>

#include <stdexcept>

class ModelSettings {
public:
  // Getters
  [[nodiscard]] int get_days_between_stdout_output() const {
    return days_between_stdout_output_;
  }
  // Setters with validation
  void set_days_between_stdout_output(int value) {
    if (value <= 0)
      throw std::invalid_argument(
          "days_between_stdout_output must be greater than 0");
    days_between_stdout_output_ = value;
  }
  [[nodiscard]] int get_initial_seed_number() const {
    return initial_seed_number_;
  }
  void set_initial_seed_number(int value) {
    if (value < 0)
      throw std::invalid_argument("initial_seed_number must be non-negative");
    initial_seed_number_ = value;
  }
  [[nodiscard]] bool get_record_genome_db() const { return record_genome_db_; }
  void set_record_genome_db(bool value) { record_genome_db_ = value; }
  [[nodiscard]] const date::year_month_day &get_starting_date() const {
    return starting_date_;
  }
  void set_starting_date(const date::year_month_day &value) {
    starting_date_ = value;
  }
  [[nodiscard]] const date::year_month_day &get_start_of_comparison_period()
      const {
    return start_of_comparison_period_;
  }
  void set_start_of_comparison_period(const date::year_month_day &value) {
    if (value < starting_date_)
      throw std::invalid_argument(
          "start_of_comparison_period cannot be before starting_date");
    start_of_comparison_period_ = value;
  }
  [[nodiscard]] const date::year_month_day &get_ending_date() const {
    return ending_date_;
  }
  void set_ending_date(const date::year_month_day &value) {
    if (value < start_of_comparison_period_)
      throw std::invalid_argument(
          "ending_date cannot be before start_of_comparison_period");
    ending_date_ = value;
  }
  [[nodiscard]] int get_start_collect_data_day() const {
    return start_collect_data_day_;
  }
  void set_start_collect_data_day(int value) {
    if (value < 0)
      throw std::invalid_argument(
          "start_collect_data_day must be non-negative");
    start_collect_data_day_ = value;
  }

private:
  int days_between_stdout_output_;
  int initial_seed_number_;
  bool record_genome_db_;
  date::year_month_day starting_date_;
  date::year_month_day start_of_comparison_period_;
  date::year_month_day ending_date_;
  int start_collect_data_day_;
};
#endif  // MODEL_SETTINGS_H

