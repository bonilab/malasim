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
  ClonalParasitePopulation& operator=(const ClonalParasitePopulation&) = delete;

private:
  double last_update_log10_parasite_density_{LOG_ZERO_PARASITE_DENSITY};
  double gametocyte_level_{0.0};
  int first_date_in_blood_{0};
  SingleHostClonalParasitePopulations *parasite_population_{nullptr};
  Genotype *genotype_{nullptr};
  ParasiteDensityUpdateFunction *update_function_{nullptr};

 public:
  static constexpr double LOG_ZERO_PARASITE_DENSITY = -1000;

  [[nodiscard]] double last_update_log10_parasite_density() const noexcept {
      return last_update_log10_parasite_density_;
  }
  void set_last_update_log10_parasite_density(const double &value) noexcept {
      last_update_log10_parasite_density_ = value;
  }

  [[nodiscard]] double gametocyte_level() const noexcept {
      return gametocyte_level_;
  }
  void set_gametocyte_level(const double &value) noexcept {
      gametocyte_level_ = value;
  }

  [[nodiscard]] int first_date_in_blood() const {
      return first_date_in_blood_;
  }
  void set_first_date_in_blood(const int &value) {
      first_date_in_blood_ = value;
  }

  [[nodiscard]] Genotype *genotype() const noexcept {
      return genotype_;
  }
  void set_genotype(Genotype *value) noexcept {
      genotype_ = value;
  }

  [[nodiscard]] ParasiteDensityUpdateFunction *update_function() const noexcept {
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

  virtual ~ClonalParasitePopulation();

  double get_current_parasite_density(const int &current_time);

  double get_log10_infectious_density() const;

  bool resist_to(const int &drug_id) const;

  void update();

  void perform_drug_action(const double &percent_parasite_remove);

};

#endif    /* CLONALPARASITEPOPULATION_H */

