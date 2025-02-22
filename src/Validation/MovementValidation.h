/*
 * MovementValidation.h
 *
 * Defines various methods needed for movement validation.
 */
#ifndef MOVEMENTVALIDATION_H
#define MOVEMENTVALIDATION_H

//#include "Core/PropertyMacro.h"
#include "Reporters/Specialist/MovementReporter.h"

class MovementValidation {

private:
  bool cell_movement_ = false;
  bool district_movement_ = false;
  bool individual_movement_ = false;

public:
  bool cell_movement() const { return cell_movement_; }
  void set_cell_movement(bool cell_movement) { cell_movement_ = cell_movement; }
  bool district_movement() const { return district_movement_; }
  void set_district_movement(bool district_movement) { district_movement_ = district_movement; }
  bool individual_movement() const { return individual_movement_; }
  void set_individual_movement(bool individual_movement) { individual_movement_ = individual_movement; }

private:
  MovementValidation() {}
  ~MovementValidation() {}

  int replicate;
  MovementReporter* reporter;
  std::string query;

public:
  // Not supported by singleton.
  MovementValidation(MovementValidation const &) = delete;

  // Not supported by singleton.
  void operator=(MovementValidation const &) = delete;

  // Get a reference to the spatial object.
  static MovementValidation &get_instance() {
    static MovementValidation instance;
    return instance;
  }

  // Add the movement to the record
  static void add_move(int individual, int source, int destination);

  // Set the reporter to use
  void set_reporter(MovementReporter* reporter) { this->reporter = reporter; }

  // Write the movement data that is generated after model initiation.
  static void write_movement_data();
};

#endif
