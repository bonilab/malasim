#ifndef MODIFYNESTEDMFTEVENT_H
#define MODIFYNESTEDMFTEVENT_H

#include <string>

#include "Events/Event.h"

class ModifyNestedMFTEvent : public WorldEvent {
public:
  // Disallow copy
  ModifyNestedMFTEvent(const ModifyNestedMFTEvent&) = delete;
  ModifyNestedMFTEvent& operator=(const ModifyNestedMFTEvent&) = delete;

  // Disallow move
  ModifyNestedMFTEvent(ModifyNestedMFTEvent&&) = delete;
  ModifyNestedMFTEvent& operator=(ModifyNestedMFTEvent&&) = delete;

public:
  int strategy_id{-1};

  ModifyNestedMFTEvent(const int &at_time, const int &strategy_id);

  virtual ~ModifyNestedMFTEvent() = default;

  const std::string name() const override { return "ChangeStrategyEvent"; }

private:
  void do_execute() override;
};

#endif  // MODIFYNESTEDMFTEVENT_H
