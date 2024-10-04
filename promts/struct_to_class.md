```
Here is an example of conversion a struct into class:


struct PopulationDemographic {
  int number_of_age_classes;
  std::vector<int> age_structure;
  std::vector<int> initial_age_structure;
  double birth_rate;
  std::vector<double> death_rate_by_age_class;
  std::vector<double> mortality_when_treatment_fail_by_age_class;
  double artificial_rescaling_of_population_size;
};

class PopulationDemographic {
public:
  // Getters
  [[nodiscard]] int get_number_of_age_classes() const {
    return number_of_age_classes_;
  }
  // Setters with validation
  void set_number_of_age_classes(int value) {
    if (value <= 0)
      throw std::invalid_argument(
          "number_of_age_classes must be greater than 0");
    number_of_age_classes_ = value;
  }

  [[nodiscard]] const std::vector<int> &get_age_structure() const {
    return age_structure_;
  }
  void set_age_structure(const std::vector<int> &value) {
    if (value.size() != number_of_age_classes_)
      throw std::invalid_argument(
          "age_structure size must match number_of_age_classes");
    age_structure_ = value;
  }

  [[nodiscard]] const std::vector<int> &get_initial_age_structure() const {
    return initial_age_structure_;
  }
  void set_initial_age_structure(const std::vector<int> &value) {
    if (value.size() != number_of_age_classes_)
      throw std::invalid_argument(
          "initial_age_structure size must match number_of_age_classes");
    initial_age_structure_ = value;
  }

  [[nodiscard]] double get_birth_rate() const { return birth_rate_; }
  void set_birth_rate(double value) {
    if (value < 0)
      throw std::invalid_argument("birth_rate must be non-negative");
    birth_rate_ = value;
  }

  [[nodiscard]] const std::vector<double> &get_death_rate_by_age_class() const {
    return death_rate_by_age_class_;
  }

  void set_death_rate_by_age_class(const std::vector<double> &value) {
    if (value.size() != number_of_age_classes_)
      throw std::invalid_argument(
          "death_rate_by_age_class size must match number_of_age_classes");
    death_rate_by_age_class_ = value;
  }

  [[nodiscard]] const std::vector<double> &
  get_mortality_when_treatment_fail_by_age_class() const {
    return mortality_when_treatment_fail_by_age_class_;
  }

  void set_mortality_when_treatment_fail_by_age_class(
      const std::vector<double> &value) {
    if (value.size() != number_of_age_classes_)
      throw std::invalid_argument(
          "mortality_when_treatment_fail_by_age_class size must match "
          "number_of_age_classes");
    mortality_when_treatment_fail_by_age_class_ = value;
  }

  [[nodiscard]] double get_artificial_rescaling_of_population_size() const {
    return artificial_rescaling_of_population_size_;
  }

  void set_artificial_rescaling_of_population_size(double value) {
    if (value <= 0)
      throw std::invalid_argument(
          "artificial_rescaling_of_population_size must be greater than 0");
    artificial_rescaling_of_population_size_ = value;
  }

private:
  int number_of_age_classes_;
  std::vector<int> age_structure_;
  std::vector<int> initial_age_structure_;
  double birth_rate_;
  std::vector<double> death_rate_by_age_class_;
  std::vector<double> mortality_when_treatment_fail_by_age_class_;
  double artificial_rescaling_of_population_size_;
};



As an expert C++ developer, would you mind help me convert the following struct.
Requirments:
  The getter and setter should be in pair, next to each other.


```
