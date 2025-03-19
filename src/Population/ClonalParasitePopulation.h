#ifndef CLONALPARASITEPOPULATION_H
#define    CLONALPARASITEPOPULATION_H

#include "ParasiteDensity/ParasiteDensityUpdateFunction.h"
#include "Treatment/Therapies/DrugType.h"
#include "Utils/Index/Indexer.hxx"

class Therapy;

class Genotype;

class SingleHostClonalParasitePopulations;

class ClonalParasitePopulation : public utils::Indexer {
  // OBJECTPOOL(ClonalParasitePopulation);
  //disallow copy and assign
  ClonalParasitePopulation(const ClonalParasitePopulation&) = delete;
  void operator=(const ClonalParasitePopulation&) = delete;

private:
  double last_update_log10_parasite_density_;
  double gametocyte_level_;
  int first_date_in_blood_;
  SingleHostClonalParasitePopulations *parasite_population_;
  Genotype *genotype_;
  ParasiteDensityUpdateFunction *update_function_;

 public:
  static const double LOG_ZERO_PARASITE_DENSITY;

  [[nodiscard]] double last_update_log10_parasite_density() const;
  void set_last_update_log10_parasite_density(const double &value);

  [[nodiscard]] double gametocyte_level() const;
  void set_gametocyte_level(const double &value);

  [[nodiscard]] int first_date_in_blood() const {
      return first_date_in_blood_;
  }
  void set_first_date_in_blood(const int &value) {
      first_date_in_blood_ = value;
  }

  Genotype *genotype() const;
  void set_genotype(Genotype *value);

  ParasiteDensityUpdateFunction *update_function() {
    return update_function_;
  }
  void set_update_function(ParasiteDensityUpdateFunction *value) {
    update_function_ = value;
  }
  [[nodiscard]] SingleHostClonalParasitePopulations *parasite_population() {
    return parasite_population_;
  }
  void set_parasite_population(SingleHostClonalParasitePopulations *value) {
    parasite_population_ = value;
  }


 public:
  ClonalParasitePopulation(Genotype *genotype = nullptr);

  //    BloodParasite(const BloodParasite& orig);
  virtual ~ClonalParasitePopulation();

  double get_current_parasite_density(const int &current_time);

  double get_log10_infectious_density() const;

  bool resist_to(const int &drug_id) const;

  void update();

  void perform_drug_action(const double &percent_parasite_remove);

};

#endif    /* CLONALPARASITEPOPULATION_H */

