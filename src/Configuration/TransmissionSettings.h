#ifndef TRANSMISSION_SETTINGS_H
#define TRANSMISSION_SETTINGS_H

#include <stdexcept>

class TransmissionSettings {
public:
  // Getter for transmission_parameter
  [[nodiscard]] double get_transmission_parameter() const {
    return transmission_parameter_;
  }

  // Setter for transmission_parameter with validation
  void set_transmission_parameter(double value) {
    if (value <= 0)
      throw std::invalid_argument(
          "transmission_parameter must be greater than 0");
    transmission_parameter_ = value;
  }

  // Getter for p_infection_from_an_infectious_bite
  [[nodiscard]] double get_p_infection_from_an_infectious_bite() const {
    return p_infection_from_an_infectious_bite_;
  }

  // Setter for p_infection_from_an_infectious_bite with validation
  void set_p_infection_from_an_infectious_bite(double value) {
    if (value < 0 || value > 1)
      throw std::invalid_argument(
          "p_infection_from_an_infectious_bite must be between 0 and 1");
    p_infection_from_an_infectious_bite_ = value;
  }

private:
  double transmission_parameter_;
  double p_infection_from_an_infectious_bite_;
};
#endif  // TRANSMISSION_SETTINGS_H

