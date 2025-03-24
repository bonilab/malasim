#ifndef Genotype_H
#define Genotype_H

#include <array>

#include "Utils/Random.h"
#include "Configuration/GenotypeParameters.h"

class GenotypeParameters;

class PfGenotypeInfo;

class OverrideEC50Pattern;

class DrugDatabase;

class DrugType;

class Therapy;

class Config;

class Random;

typedef std::string GeneStr;
typedef std::vector<GeneStr> ChromosomalGenotypeStr;
typedef std::vector<ChromosomalGenotypeStr> PfGenotypeStr;
typedef std::pair<std::vector<std::pair<int,std::string>>,std::pair<int,int>> MosquitoRecombinedGenotypeInfo;

class Genotype {
  //disallow copy and assign
  Genotype(const Genotype&) = delete;
  void operator=(const Genotype&) = delete;

public:
  int genotype_id_ { -1 };
  PfGenotypeStr pf_genotype_str = std::vector<ChromosomalGenotypeStr>(14);
  std::string aa_sequence;
  double daily_fitness_multiple_infection { 1 };
  std::vector<double> EC50_power_n {};
  std::vector<MosquitoRecombinedGenotypeInfo> resistant_recombinations_in_mosquito;

public:
  explicit Genotype(const std::string& aa_sequence);

  virtual ~Genotype();

  int genotype_id() const { return genotype_id_; }
  void set_genotype_id(int genotype_id){ genotype_id_ = genotype_id; }

  double get_EC50_power_n(DrugType* dt);

  bool resist_to(DrugType* dt);

  Genotype* combine_mutation_to(const int& locus, const int& value);

  Genotype* modify_genotype_allele(const std::vector<std::tuple<int,int,char>> &alleles, Config *pConfig);

  std::string get_aa_sequence();

  bool is_valid(const GenotypeParameters::PfGenotypeInfo& gene_info);

  void calculate_daily_fitness(const GenotypeParameters::PfGenotypeInfo& gene_info);

  void calculate_EC50_power_n(const GenotypeParameters::PfGenotypeInfo& info, DrugDatabase* pDatabase);

  Genotype* perform_mutation_by_drug(Config* pConfig, utils::Random* pRandom, DrugType* pDrugType, double mutation_probability_by_locus) const;

  friend std::ostream& operator<<(std::ostream& os, const Genotype& e);

  void override_EC50_power_n(const std::vector<GenotypeParameters::OverrideEC50Pattern>& override_patterns, DrugDatabase* drug_db);

  bool match_pattern(const std::string& pattern);

  Genotype* free_recombine_with(Config* config, utils::Random* pRandom, Genotype* other);

  static Genotype* free_recombine(Config* config, utils::Random* pRandom, Genotype* f, Genotype* m);

  static std::string Convert_PfGenotypeStr_To_String(const PfGenotypeStr& pfGenotypeStr);
};

#endif /* Genotype_H */
