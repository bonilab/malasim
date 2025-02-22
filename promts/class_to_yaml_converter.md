```
Here is the example of using yaml-cpp to read in yaml node to class object

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
  [[nodiscard]] const date::year_month_day &get_start_of_comparison_period_date()
      const {
    return start_of_comparison_period_;
  }
  void set_start_of_comparison_period_date(const date::year_month_day &value) {
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


template <>
struct convert<ModelSettings> {
  static Node encode(const ModelSettings &rhs) {
    Node node;
    node["days_between_stdout_output"] = rhs.get_days_between_stdout_output();
    node["initial_seed_number"] = rhs.get_initial_seed_number();
    node["record_genome_db"] = rhs.get_record_genome_db();
    node["starting_date"] = rhs.get_starting_date();
    node["start_of_comparison_period"] = rhs.get_start_of_comparison_period_date();
    node["ending_date"] = rhs.get_ending_date();
    node["start_collect_data_day"] = rhs.get_start_collect_data_day();
    return node;
  }

  static bool decode(const Node &node, ModelSettings &rhs) {
    if (!node["days_between_stdout_output"]) {
      throw std::runtime_error("Missing 'days_between_stdout_output' field.");
    }
    if (!node["initial_seed_number"]) {
      throw std::runtime_error("Missing 'initial_seed_number' field.");
    }
    if (!node["record_genome_db"]) {
      throw std::runtime_error("Missing 'record_genome_db' field.");
    }
    if (!node["starting_date"]) {
      throw std::runtime_error("Missing 'starting_date' field.");
    }
    if (!node["start_of_comparison_period"]) {
      throw std::runtime_error("Missing 'start_of_comparison_period' field.");
    }
    if (!node["ending_date"]) {
      throw std::runtime_error("Missing 'ending_date' field.");
    }
    if (!node["start_collect_data_day"]) {
      throw std::runtime_error("Missing 'start_collect_data_day' field.");
    }
    rhs.set_days_between_stdout_output(
        node["days_between_stdout_output"].as<int>());
    rhs.set_initial_seed_number(node["initial_seed_number"].as<int>());
    rhs.set_record_genome_db(node["record_genome_db"].as<bool>());
    rhs.set_starting_date(node["starting_date"].as<date::year_month_day>());
    rhs.set_start_of_comparison_period_date(
        node["start_of_comparison_period"].as<date::year_month_day>());
    rhs.set_ending_date(node["ending_date"].as<date::year_month_day>());
    rhs.set_start_collect_data_day(node["start_collect_data_day"].as<int>());
    return true;
  }
};

As an expert in C++ developer, would you mind make the similar convert function for the following class:
```
