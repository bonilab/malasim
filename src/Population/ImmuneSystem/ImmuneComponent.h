#ifndef IMMUNECOMPONENT_H
#define    IMMUNECOMPONENT_H

//#include "ObjectPool.h"
class ImmuneSystem;

class Model;

class ImmuneComponent {
    //disallow copy and assign
    ImmuneComponent(const ImmuneComponent&) = delete;
    void operator=(const ImmuneComponent&) = delete;

public:
    [[nodiscard]] ImmuneSystem* immune_system() const { return immune_system_; }
    void set_immune_system(ImmuneSystem* immune_system) { immune_system_ = immune_system; }

    [[nodiscard]] double latest_value() const { return latest_value_; }
    void set_latest_value(double latest_value) { latest_value_ = latest_value; }

private:
    ImmuneSystem *immune_system_;
    double latest_value_;

 public:
  explicit ImmuneComponent(ImmuneSystem *immune_system = nullptr);

  //    ImmuneComponent(const ImmuneComponent& orig);
  virtual ~ImmuneComponent();

  void update();

  virtual void draw_random_immune();

  virtual double get_current_value();

  virtual double get_decay_rate(const int &age = 0) const = 0;

  virtual double get_acquire_rate(const int &age = 0) const = 0;

};

#endif    /* IMMUNECOMPONENT_H */
