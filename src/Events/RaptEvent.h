#pragma once

#include "Event.h"

class Person;

class RaptEvent : public Event {
public:
  RaptEvent(const RaptEvent &) = delete;
  RaptEvent &operator=(const RaptEvent &) = delete;

public:
  RaptEvent() = default;
  ~RaptEvent() override = default;

  const std::string name() const override { return "RAPT Event"; }

private:
  void do_execute() override;
};
