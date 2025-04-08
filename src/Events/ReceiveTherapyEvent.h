#ifndef RECEIVETHERAPYEVENT_H
#define RECEIVETHERAPYEVENT_H

#include "Event.h"
#include "Population/ClonalParasitePopulation.h"

class Scheduler;

class Person;

class Therapy;

class ClonalParasitePopulation;

class ReceiveTherapyEvent : public PersonEvent {
public:
  // disallow copy and assign and move
  ReceiveTherapyEvent(const ReceiveTherapyEvent &) = delete;
  void operator=(const ReceiveTherapyEvent &) = delete;
  ReceiveTherapyEvent(ReceiveTherapyEvent &&) = delete;
  void operator=(ReceiveTherapyEvent &&) = delete;
  explicit ReceiveTherapyEvent(Person* person) : PersonEvent(person) {}
  ~ReceiveTherapyEvent() override = default;

  [[nodiscard]] const std::string name() const override { return "ReceiveTherapyEvent"; }

  Therapy* received_therapy() { return received_therapy_; }
  void set_received_therapy(Therapy* value) { received_therapy_ = value; }

  ClonalParasitePopulation* clinical_caused_parasite() { return clinical_caused_parasite_; }
  void set_clinical_caused_parasite(ClonalParasitePopulation* value) {
    clinical_caused_parasite_ = value;
  }

  [[nodiscard]] bool is_part_of_mac_therapy() const { return is_part_of_mac_therapy_; }
  void set_is_part_of_mac_therapy(bool value) { is_part_of_mac_therapy_ = value; }

private:
  bool is_part_of_mac_therapy_{false};
  Therapy* received_therapy_{nullptr};
  ClonalParasitePopulation* clinical_caused_parasite_{nullptr};
  void do_execute() override;
};

#endif /* RECEIVETHERAPYEVENT_H */
