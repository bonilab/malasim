/**
 * ClinicalStudy.h
 *
 * This class controls how a clinical study is simulated using the model.
 */
#ifndef CLINICALSTUDY_H
#define CLINICALSTUDY_H

#include <vector>

#include "../Event.h"

class Scheduler;
class Person;

class ClinicalStudy : public WorldEvent {
private:
  const int OBSERVATION_PERIOD = 28;
  const int TOTAL_OBSERVATIONS = 10000;

  struct Enrollee {
    Person* person = nullptr;
    int days = 0;
  };

  int observations = 0;
  int recrudescence = 0;
  std::vector<Enrollee> enrollments;

public:
  ClinicalStudy() = default;
  ~ClinicalStudy() = default;

  const std::string name() const override { return "ClinicalStudy"; }

private:
  // Triggered by the scheduler
  void do_execute() override;

  // Check persons that have already been enrolled in the study at T+28
  // to determine if they have cleared the parasite or not.
  //
  // Returns true if the study is complete, false otherwise.
  bool check_enrollees();

  // Check the population for new individuals to enroll.
  void check_population();
};

#endif
