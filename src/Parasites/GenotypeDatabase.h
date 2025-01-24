#ifndef INTPARASITEDATABASE_H
#define INTPARASITEDATABASE_H

#include <map>

#include "Utils/TypeDef.hxx"

class Genotype;
class Config;

typedef std::map<ul, Genotype*> GenotypePtrMap;

class GenotypeDatabase : public GenotypePtrMap {
  //disallow copy, assign and move
  GenotypeDatabase(const GenotypeDatabase&) = delete;
  GenotypeDatabase& operator=(const GenotypeDatabase&) = delete;
  GenotypeDatabase(GenotypeDatabase&&) = delete;
  GenotypeDatabase& operator=(GenotypeDatabase&&) = delete;

public:
  std::map<std::string, Genotype*> aa_sequence_id_map;
  std::map<int,std::map<std::string,double>> drug_id_ec50;

public:
  GenotypeDatabase();

  virtual ~GenotypeDatabase();

  void add(Genotype* genotype);

  Genotype* get_genotype(const std::string& aa_sequence);

  unsigned int get_id(const std::string& aa_sequence);

  Genotype* get_genotype_from_alleles_structure(const IntVector& alleles);

  double get_min_ec50(int drug_id);

  [[nodiscard]] std::vector<int> get_weight() const { return weight_; }
    void set_weight(const std::vector<int>& value) { weight_ = value; }

private:
  unsigned int auto_id {0};
  std::vector<int> weight_;
};

#endif /* INTPARASITEDATABASE_H */
