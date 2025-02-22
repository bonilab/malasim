#ifndef INFANTIMMUNECOMPONENT_H
#define    INFANTIMMUNECOMPONENT_H

#include "ImmuneComponent.h"

class InfantImmuneComponent : public ImmuneComponent {
  //disallow copy and assign
  InfantImmuneComponent(const InfantImmuneComponent&) = delete;
  void operator=(const InfantImmuneComponent&) = delete;

 public:
  explicit InfantImmuneComponent(ImmuneSystem *immune_system = nullptr);

  // InfantImmuneComponent(const InfantImmuneComponent& orig);
  virtual ~InfantImmuneComponent();

  double get_decay_rate(const int &age) const override;

  double get_acquire_rate(const int &age) const override;

  double get_current_value() override;

};

#endif    /* INFANTIMMUNECOMPONENT_H */
