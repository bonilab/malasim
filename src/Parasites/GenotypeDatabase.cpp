#include "GenotypeDatabase.h"
#include "Configuration/Config.h"
#include "Genotype.h"
#include "Utils/Helpers/NumberHelpers.hxx"
#include "Simulation/Model.h"
#include "Utils/TypeDef.hxx"

GenotypeDatabase::GenotypeDatabase() : auto_id(0) {}

GenotypeDatabase::~GenotypeDatabase() {
  for (auto &i : *this) {
    delete i.second;
  }
  clear();
}

void GenotypeDatabase::add(Genotype *genotype) {
  if (this->find(genotype->genotype_id()) != this->end()) {
    delete (*this)[genotype->genotype_id()];
  }
  (*this)[genotype->genotype_id()] = genotype;
  // spdlog::info("GenotypeDatabase Added genotype id: {} aa_sequence: {}", genotype->genotype_id(), genotype->get_aa_sequence());
}

Genotype *GenotypeDatabase::get_genotype_from_alleles_structure(const IntVector &alleles) {
  auto id = 0;
  for (auto i = 0; i < alleles.size(); i++) {
    id += weight_[i] * alleles[i];
  }
  return this->at(id);
}

unsigned int GenotypeDatabase::get_id(const std::string &aa_sequence) {
  return get_genotype(aa_sequence)->genotype_id();
}

Genotype *GenotypeDatabase::get_genotype(const std::string &aa_sequence) {
  if (aa_sequence_id_map.find(aa_sequence) == aa_sequence_id_map.end()) {
    // not yet exist then initialize new genotype
    auto new_id = auto_id;
    auto new_genotype = new Genotype(aa_sequence);
    new_genotype->set_genotype_id(new_id);
    new_genotype->resistant_recombinations_in_mosquito = std::vector<MosquitoRecombinedGenotypeInfo>();

    // check if aa_sequence is valid
    if (!new_genotype->is_valid(Model::get_config()->get_genotype_parameters().get_pf_genotype_info())) {
      spdlog::error("Invalid genotype: " + aa_sequence);
    }

    // calculate cost of resistance
    new_genotype->calculate_daily_fitness(Model::get_config()->get_genotype_parameters().get_pf_genotype_info());

    // calculate ec50
    new_genotype->calculate_EC50_power_n(Model::get_config()->get_genotype_parameters().get_pf_genotype_info(),
      Model::get_config()->get_drug_parameters().drug_db);

    new_genotype->override_EC50_power_n(Model::get_config()->get_genotype_parameters().get_override_ec50_patterns(),
      Model::get_config()->get_drug_parameters().drug_db);

    // add min ec50 of each drug to db
    for(int drug_id = 0; drug_id <  Model::get_config()->get_drug_parameters().drug_db->size(); drug_id++){
      if (drug_id_ec50.find(drug_id) == drug_id_ec50.end()){
        if (drug_id_ec50[drug_id].find(new_genotype->get_aa_sequence()) == drug_id_ec50[drug_id].end()){
          drug_id_ec50[drug_id][new_genotype->get_aa_sequence()] = new_genotype->get_EC50_power_n(
            Model::get_config()->get_drug_parameters().drug_db->at(drug_id));
        }
        else{
          if (drug_id_ec50[drug_id][new_genotype->get_aa_sequence()] > new_genotype->get_EC50_power_n(
            Model::get_config()->get_drug_parameters().drug_db->at(drug_id))){
            drug_id_ec50[drug_id][new_genotype->get_aa_sequence()] = new_genotype->get_EC50_power_n(
              Model::get_config()->get_drug_parameters().drug_db->at(drug_id));
          }
        }
      }
    }

    aa_sequence_id_map[aa_sequence] = new_genotype;
    add(new_genotype);
    new_genotype->resistant_recombinations_in_mosquito = std::vector<MosquitoRecombinedGenotypeInfo>();
    auto_id++;
    // spdlog::info("GenotypeDB new genotype id {} aa_sequence {}",auto_id,aa_sequence);
    return new_genotype;
  } else {
    return aa_sequence_id_map[aa_sequence];
  }
}

double GenotypeDatabase::get_min_ec50(int drug_id){
  auto it = min_element(drug_id_ec50[drug_id].begin(), drug_id_ec50[drug_id].end(),
                                                 [](const auto& l, const auto& r) { return l.second < r.second; });

  return it->second;
}

Genotype* GenotypeDatabase::get_genotype_by_id(const int& id) {
  return (*this)[id];
}

// GenotypeDatabase* GenotypeDatabase::all() {
//   return this;
// }

