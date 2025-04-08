#pragma once

#include "Event.h"

class Person;

class RaptEvent : public PersonEvent {
public:
  RaptEvent(const RaptEvent &) = delete;
  RaptEvent &operator=(const RaptEvent &) = delete;
  RaptEvent(RaptEvent &&) = delete;
  RaptEvent &operator=(RaptEvent &&) = delete;

  explicit RaptEvent(Person* person) : PersonEvent(person) {}
  ~RaptEvent() override = default;

  [[nodiscard]] const std::string name() const override { return "RAPT Event"; }

private:
  void do_execute() override;
};
