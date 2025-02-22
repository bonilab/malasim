#ifndef IMMUNESYSTEM_H
#define    IMMUNESYSTEM_H

#include "Utils/TypeDef.hxx"

class Model;

class Person;

class ImmuneComponent;

class Config;

//typedef std::vector<ImmuneComponent*> ImmuneComponentPtrVector;

class ImmuneSystem {
  // OBJECTPOOL(ImmuneSystem)
  //disallow copy and assign
  ImmuneSystem(const ImmuneSystem&) = delete;
  void operator=(const ImmuneSystem&) = delete;

public:
  [[nodiscard]] Person* person() const { return person_; }
  void set_person(Person* person) { person_ = person; }

  [[nodiscard]] ImmuneComponent* immune_component() const;
  void set_immune_component(ImmuneComponent *value);

  [[nodiscard]] bool increase() const { return increase_; }
  void set_increase(bool increase) { increase_ = increase; }

private:
    Person* person_;
    ImmuneComponent* immune_component_;
    bool increase_;

 public:
  explicit ImmuneSystem(Person *p = nullptr);

  virtual ~ImmuneSystem();

  //    virtual void clear();

  virtual void draw_random_immune();

  virtual void update();

  virtual double get_lastest_immune_value() const;

  virtual void set_latest_immune_value(double value);

  virtual double get_current_value() const;

  virtual double
  get_parasite_size_after_t_days(const int &duration, const double &originalSize, const double &fitness) const;

  virtual double get_clinical_progression_probability() const;

};

#endif    /* IMMUNESYSTEM_H */
