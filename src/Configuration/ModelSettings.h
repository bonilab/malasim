#ifndef MODEL_SETTINGS_H
#define MODEL_SETTINGS_H

#include <date/date.h>

struct ModelSettings {
  int days_between_stdout_output;
  int initial_seed_number;
  bool record_genome_db;
  date::year_month_day starting_date;
  date::year_month_day start_of_comparison_period;
  date::year_month_day ending_date;
  int start_collect_data_day;
};

#endif  // MODEL_SETTINGS_H

