#ifndef SINGLEHOSTCLONALPARASITEPOPULATIONS_H
#define SINGLEHOSTCLONALPARASITEPOPULATIONS_H

#include <vector>

#include "Utils/TypeDef.hxx"
#include "Population/ClonalParasitePopulation.h"
class ClonalParasitePopulation;

class Person;

class DrugType;

class DrugsInBlood;

class ParasiteDensityUpdateFunction;

class SingleHostClonalParasitePopulations {
  // OBJECTPOOL(SingleHostClonalParasitePopulations)

  //disallow copy and assign
  SingleHostClonalParasitePopulations(const SingleHostClonalParasitePopulations &) = delete;
  void operator=(const SingleHostClonalParasitePopulations &) = delete;

private:
  Person* person_;
  std::vector<ClonalParasitePopulation*>* parasites_;

public:
  // this value will be automatically updated daily in the function clear_cured_parasites
  // in order to have accurate sum of all density
  double log10_total_infectious_denstiy{ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY};

  [[nodiscard]] double log10_total_infectious_density() const {
    return log10_total_infectious_denstiy;
  }
  void set_log10_total_infectious_density(const double &value) {
    log10_total_infectious_denstiy = value;
  }

  [[nodiscard]] Person* person() const {
    return person_;
  }
  void set_person(Person* value) {
    person_ = value;
  }

  [[nodiscard]] std::vector<ClonalParasitePopulation*>* parasites() const {
    return parasites_;
  }

  void set_parasites(std::vector<ClonalParasitePopulation*>* value) {
    parasites_ = value;
  }

public:
  SingleHostClonalParasitePopulations(Person *person = nullptr);

  //    ParasitePopulation(const ParasitePopulation& orig);
  virtual ~SingleHostClonalParasitePopulations();

  void init();

  virtual int size();

  virtual void add(ClonalParasitePopulation *blood_parasite);

  virtual void remove(ClonalParasitePopulation *blood_parasite);

  virtual void remove(const int &index);

  virtual int latest_update_time() const;

  virtual bool contain(ClonalParasitePopulation *blood_parasite);

  void change_all_parasite_update_function(ParasiteDensityUpdateFunction *from,
                                           ParasiteDensityUpdateFunction *to) const;

  void update();

  void clear_cured_parasites();

  void clear();

  void update_by_drugs(DrugsInBlood *drugs_in_blood) const;

  bool has_detectable_parasite() const;

  bool is_gametocytaemic() const;
};

#endif /* SINGLEHOSTCLONALPARASITEPOPULATIONS_H */
