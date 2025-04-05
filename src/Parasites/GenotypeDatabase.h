#ifndef INTPARASITEDATABASE_H
#define INTPARASITEDATABASE_H

#include <map>

#include "Utils/TypeDef.hxx"

class Genotype;
class Config;

using GenotypePtrMap = std::map<ul, std::unique_ptr<Genotype>>;

class GenotypeDatabase : public GenotypePtrMap {
public:
  // disallow copy, assign and move
  GenotypeDatabase(const GenotypeDatabase &) = delete;
  void operator=(const GenotypeDatabase &) = delete;
  GenotypeDatabase(GenotypeDatabase &&) = delete;
  GenotypeDatabase &operator=(GenotypeDatabase &&) = delete;

  GenotypeDatabase();

  virtual ~GenotypeDatabase();

  GenotypeDatabase* operator()() { return this; }

  void add(Genotype* genotype);

  Genotype* get_genotype(const std::string &aa_sequence);

  unsigned int get_id(const std::string &aa_sequence);

  Genotype* get_genotype_by_id(const int &id);

  Genotype* get_genotype_from_alleles_structure(const IntVector &alleles);

  double get_min_ec50(int drug_id);

  [[nodiscard]] std::vector<int> get_weight() const { return weight_; }
  void set_weight(const std::vector<int> &value) { weight_ = value; }

  // override at
  Genotype* at(const int &id) {
    auto it = GenotypePtrMap::find(id);
    if (it != GenotypePtrMap::end()) { return it->second.get(); }
    return nullptr;
  }

private:
  std::map<std::string, Genotype*> aa_sequence_id_map_;
  std::map<int, std::map<std::string, double>> drug_id_ec50_;

  unsigned int auto_id_{0};
  std::vector<int> weight_;
};

#endif /* INTPARASITEDATABASE_H */
