#ifndef MODIFYNESTEDMFTEVENT_H
#define MODIFYNESTEDMFTEVENT_H

#include <string>

#include "Events/Event.h"

class ModifyNestedMFTEvent : public Event {
public:
  //disallow copy, assign and move
  ModifyNestedMFTEvent(const ModifyNestedMFTEvent &) = delete;
  void operator=(const ModifyNestedMFTEvent &) = delete;
  ModifyNestedMFTEvent(ModifyNestedMFTEvent &&) = delete;
  void operator=(ModifyNestedMFTEvent &&) = delete;

public:
  int strategy_id{-1};

  ModifyNestedMFTEvent(const int &at_time, const int &strategy_id);

  virtual ~ModifyNestedMFTEvent() = default;

  std::string name() override { return "ChangeStrategyEvent"; }

private:
  void execute() override;
};

#endif  // MODIFYNESTEDMFTEVENT_H
