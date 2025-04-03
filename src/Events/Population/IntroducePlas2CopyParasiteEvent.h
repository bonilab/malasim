#ifndef PCMS_INTRODUCEPLAS2COPYPARASITEEVENT_H
#define PCMS_INTRODUCEPLAS2COPYPARASITEEVENT_H

#include <string>

// #include "Core/ObjectPool.h"
#include "Events/Event.h"

class IntroducePlas2CopyParasiteEvent : public WorldEvent {
public:
  //disallow copy, assign and move
  IntroducePlas2CopyParasiteEvent(const IntroducePlas2CopyParasiteEvent &) = delete;
  void operator=(const IntroducePlas2CopyParasiteEvent &) = delete;
  IntroducePlas2CopyParasiteEvent(IntroducePlas2CopyParasiteEvent &&) = delete;
  void operator=(IntroducePlas2CopyParasiteEvent &&) = delete;

  // OBJECTPOOL(IntroducePlas2CopyParasiteEvent)

private:
  int location_;
  double fraction_;

public:
  explicit IntroducePlas2CopyParasiteEvent(const int &location = -1,
                                           const int &execute_at = -1,
                                           const double &fraction = 0);

  //    ImportationEvent(const ImportationEvent& orig);
  ~IntroducePlas2CopyParasiteEvent() override;

  const std::string name() const override { return "ImportationEvent"; }

private:
  void do_execute() override;
};

#endif  // PCMS_INTRODUCEPLAS2COPYPARASITEEVENT_H
