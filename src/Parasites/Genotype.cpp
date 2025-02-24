#include "Genotype.h"
#include <algorithm>
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Helpers/NumberHelpers.hxx"
#include "Simulation/Model.h"
#include "Treatment/Therapies/DrugDatabase.h"
// #include "Therapies/SCTherapy.h"

Genotype::Genotype(const std::string &in_aa_sequence) : aa_sequence { in_aa_sequence } {
  // create aa structure
  std::string chromosome_str;
  std::istringstream tokenStream(in_aa_sequence);
  auto ii = 0;
  while (std::getline(tokenStream, chromosome_str, '|')) {
    std::string gene_str;
    std::istringstream chromosomeTokenStream(chromosome_str);
    auto jj = 0;
    while (std::getline(chromosomeTokenStream, gene_str, ',')) {
      pf_genotype_str[ii].push_back(gene_str);
      jj++;
    }
    ii++;
  }
  resistant_recombinations_in_mosquito = std::vector<MosquitoRecombinedGenotypeInfo>();
}

Genotype::~Genotype() = default;

bool Genotype::resist_to(DrugType *dt) {
  return EC50_power_n[dt->id()] > pow(dt->base_EC50, dt->n());
}

Genotype *Genotype::combine_mutation_to(const int &locus, const int &value) {
  // TODO: remove this
  return this;
}

double Genotype::get_EC50_power_n(DrugType *dt) const {
  return EC50_power_n[dt->id()];
}

std::ostream &operator<<(std::ostream &os, const Genotype &e) {
  os << e.genotype_id << "\t";
  os << e.get_aa_sequence();
  return os;
}

std::string Genotype::get_aa_sequence() const {
  return aa_sequence;
}
bool Genotype::is_valid(const GenotypeParameters::PfGenotypeInfo &genotype_info) {
  for (int chromosome_i = 0; chromosome_i < 14; ++chromosome_i) {
    auto chromosome_info = genotype_info.chromosome_infos[chromosome_i];
    if (chromosome_info.get_genes().size() != pf_genotype_str[chromosome_i].size()) {
      spdlog::error("Error {} {}", pf_genotype_str[chromosome_i].size(), chromosome_info.get_genes().size());
      return false;
    }

    for (int gene_i = 0; gene_i < pf_genotype_str[chromosome_i].size(); ++gene_i) {
      auto gene_info = chromosome_info.get_genes()[gene_i];
      auto max_aa_pos = gene_info.get_max_copies() > 1 ? pf_genotype_str[chromosome_i][gene_i].size() - 1
                                                 : pf_genotype_str[chromosome_i][gene_i].size();

      // check same size with aa postions info
      if (gene_info.get_aa_positions().size() != max_aa_pos) {
        spdlog::error("Error {} {}", pf_genotype_str[chromosome_i][gene_i],gene_info.get_aa_positions().size());
        return false;
      }

      for (int aa_i = 0; aa_i < max_aa_pos; ++aa_i) {
        auto aa_pos_info = gene_info.get_aa_positions()[aa_i];
        auto element = pf_genotype_str[chromosome_i][gene_i][aa_i];
        if (std::find(aa_pos_info.get_amino_acids().begin(), aa_pos_info.get_amino_acids().end(), std::string(1,element))
        == aa_pos_info.get_amino_acids().end()) {
          spdlog::error("Incorrect amino acid in aa sequence");
          return false;
        }
      }

      // check number copy valid or not
      if (gene_info.get_max_copies() > 1) {
        auto copy_number = NumberHelpers::char_to_single_digit_number(pf_genotype_str[chromosome_i][gene_i].back());
        if (copy_number > gene_info.get_max_copies()) {
          spdlog::error("Incorrect copy number");
          return false;
        }
      }
    }
  }
  // spdlog::info("Genotype {} is valid", aa_sequence);
  return true;
}

void Genotype::calculate_daily_fitness(const GenotypeParameters::PfGenotypeInfo &genotype_info) {
  daily_fitness_multiple_infection = 1.0;

  spdlog::trace("Genotype: {}",  aa_sequence);
  for (int chromosome_i = 0; chromosome_i < pf_genotype_str.size(); ++chromosome_i) {
    auto chromosome_info = genotype_info.chromosome_infos[chromosome_i];

    for (int gene_i = 0; gene_i < pf_genotype_str[chromosome_i].size(); ++gene_i) {
      auto res_gene_info = chromosome_info.get_genes()[gene_i];
      auto max_aa_pos = res_gene_info.get_max_copies() > 1 ? pf_genotype_str[chromosome_i][gene_i].size() - 1
                                                     : pf_genotype_str[chromosome_i][gene_i].size();


        for (int aa_i = 0; aa_i < max_aa_pos; ++aa_i) {
            // calculate cost of resistance
            auto aa_pos_info = res_gene_info.get_aa_positions()[aa_i];
            auto element = pf_genotype_str[chromosome_i][gene_i][aa_i];

            auto it = std::find(aa_pos_info.get_amino_acids().begin(),
              aa_pos_info.get_amino_acids().end(), std::string(1,element));
            auto element_id = it - aa_pos_info.get_amino_acids().begin();

            auto cr = aa_pos_info.get_daily_crs()[element_id];

            if (res_gene_info.get_average_daily_crs() > 0) {
                daily_fitness_multiple_infection *= (1 - res_gene_info.get_average_daily_crs()*cr);
              spdlog::trace("\tUsing average CRS chromosome_i: {} gene_i: {} aa_i: {} cr: {} average_daily_crs: {} cr: {} (1 - res_gene_info.average_daily_crs*cr): {}",
                chromosome_i + 1, gene_i, aa_i, cr, res_gene_info.get_average_daily_crs(), cr, (1 - res_gene_info.get_average_daily_crs()*cr));
            } else {
                daily_fitness_multiple_infection *= (1 - cr);
            }
            spdlog::trace("Genotype: {} chromosome_i: {} gene_i: {} aa_i: {} cr: {} daily_fitness_multiple_infection: {}",
              aa_sequence, chromosome_i + 1, gene_i, aa_i, cr, daily_fitness_multiple_infection);
        }

      // calculate for number copy variation
      if (res_gene_info.get_max_copies() > 1) {
        auto copy_number = (int)pf_genotype_str[chromosome_i][gene_i].back() - 48;
        if (copy_number > 1) {
          daily_fitness_multiple_infection *= 1 - res_gene_info.get_cnv_daily_crs()[copy_number - 1];
          spdlog::trace("Genotype: {} chromosome_i: {} gene_i: {} copy_number: {} daily_fitness_multiple_infection: {}",
            aa_sequence, chromosome_i + 1, gene_i, copy_number, daily_fitness_multiple_infection);
        }
      }
    }
  }
//  LOG(INFO) << "\n";
}

void Genotype::calculate_EC50_power_n(const GenotypeParameters::PfGenotypeInfo &genotype_info, DrugDatabase *drug_db) {
  EC50_power_n.resize(drug_db->size());
  for (const auto &[drug_id, dt] : *drug_db) {
    EC50_power_n[drug_id] = dt->base_EC50;
  }

  for (int chromosome_i = 0; chromosome_i < pf_genotype_str.size(); ++chromosome_i) {
    auto chromosome_info = genotype_info.chromosome_infos[chromosome_i];

    for (int gene_i = 0; gene_i < pf_genotype_str[chromosome_i].size(); ++gene_i) {
      auto res_gene_info = chromosome_info.get_genes()[gene_i];
      auto max_aa_pos = res_gene_info.get_max_copies() > 1 ? pf_genotype_str[chromosome_i][gene_i].size() - 1
                                                     : pf_genotype_str[chromosome_i][gene_i].size();
      std::vector<int> number_of_effective_mutations_in_same_genes(drug_db->size(), 0);

      for (int aa_i = 0; aa_i < max_aa_pos; ++aa_i) {
        // calculate cost of resistance
        auto aa_pos_info = res_gene_info.get_aa_positions()[aa_i];
        auto element = pf_genotype_str[chromosome_i][gene_i][aa_i];
        auto it = std::find(aa_pos_info.get_amino_acids().begin(),
          aa_pos_info.get_amino_acids().end(), std::string(1,element));
        if (it == aa_pos_info.get_amino_acids().end()) {
          spdlog::error("Incorrect AA in aa sequence");
        }
        auto element_id = it - aa_pos_info.get_amino_acids().begin();

        for (const auto &[drug_id, dt] : *drug_db) {

          for(auto const &ec50s : aa_pos_info.get_multiplicative_effect_on_EC50()) {
            if(ec50s.get_drug_id() == drug_id) {
              auto multiplicative_effect_factor = ec50s.get_factors()[element_id];
              if (multiplicative_effect_factor > 1) {
                // encounter resistant aa
                number_of_effective_mutations_in_same_genes[drug_id] += 1;
                if (number_of_effective_mutations_in_same_genes[drug_id] > 1) {
                  for(auto const &ec50s_2_or_more : res_gene_info.get_multiplicative_effect_on_ec50_for_2_or_more_mutations()) {
                    if(ec50s_2_or_more.get_drug_id() == drug_id) {
                      multiplicative_effect_factor = ec50s_2_or_more.get_factor();
                      spdlog::trace("aa_sequence: {} DOUBLE MUT drug_id: {} chr: {} gene: {} aa: {} EC50_power_n: {} * multiplicative_effect_factor: {}  = {}",
                        aa_sequence, drug_id, chromosome_i + 1, gene_i, aa_i, EC50_power_n[drug_id], multiplicative_effect_factor,
                        EC50_power_n[drug_id]*multiplicative_effect_factor);
                    }
                    spdlog::trace("aa_sequence: {} SINGLE MUT drug_id: {} chr: {} gene: {} aa: {} EC50_power_n: {} * multiplicative_effect_factor: {}  = {}",
                      aa_sequence, drug_id, chromosome_i + 1, gene_i, aa_i, EC50_power_n[drug_id], multiplicative_effect_factor,
                      EC50_power_n[drug_id]*multiplicative_effect_factor);
                  }
                }
              }
              EC50_power_n[drug_id] *= multiplicative_effect_factor;
            }
          }
        }
      }

      // calculate for number copy variation
      if (res_gene_info.get_max_copies() > 1) {
        auto copy_number = (int)pf_genotype_str[chromosome_i][gene_i].back() - 48;
        if (copy_number > 1) {
          for (const auto &[drug_id, dt] : *drug_db) {
            for(auto const &ec50s : res_gene_info.get_cnv_multiplicative_effect_on_EC50()) {
              if(ec50s.get_drug_id() == drug_id) {
                spdlog::trace("aa_sequence: {} CNV drug_id: {} chr: {} gene: {} EC50_power_n: {} * multiplicative_effect_factor: {}  = {}",
                  aa_sequence, drug_id, chromosome_i + 1, gene_i, EC50_power_n[drug_id], ec50s.get_factors()[copy_number - 1],
                  EC50_power_n[drug_id]*ec50s.get_factors()[copy_number - 1]);
              }
            }
          }
        }
      }
    }
  }

  // power n
  for (const auto &[drug_id, dt] : *drug_db) {
    EC50_power_n[drug_id] = pow(EC50_power_n[drug_id], dt->n());
  }
}

Genotype *Genotype::perform_mutation_by_drug(Config *pConfig, utils::Random *pRandom, DrugType *pDrugType, double mutation_probability_by_locus) const {
  std::string new_aa_sequence { aa_sequence };
//  LOG(INFO) << "mask: " << pConfig->mutation_mask();
//  LOG(INFO) << "old aa_sequence: " << aa_sequence;
  for(int aa_pos_id = 0; aa_pos_id < pDrugType->resistant_aa_locations.size(); aa_pos_id++) {
    // get aa position info (aa index in aa string, is copy number)
    auto aa_pos = pDrugType->resistant_aa_locations[aa_pos_id];
    if(pConfig->get_genotype_parameters().get_mutation_mask()[aa_pos.aa_index_in_aa_string] == '1'){
        const auto p = pRandom->random_flat(0.0, 1.0);
//        LOG(INFO) << "p: " << p << " Mutation probability: " << mutation_probability_by_locus;
//        LOG(INFO) << "aa_pos_id: " << aa_pos_id << " aa_pos.aa_index_in_aa_string: " << aa_pos.aa_index_in_aa_string;
        if (p < mutation_probability_by_locus){
          if (aa_pos.is_copy_number) {
                // increase or decrease by 1 step
                auto old_copy_number = NumberHelpers::char_to_single_digit_number(aa_sequence[aa_pos.aa_index_in_aa_string]);
//                LOG(INFO) << "old_copy_number: " << old_copy_number;
                if (old_copy_number == 1) {
//                  LOG(INFO) << "old_copy_number == 1";
                    new_aa_sequence[aa_pos.aa_index_in_aa_string] = NumberHelpers::single_digit_number_to_char(old_copy_number + 1);
                } else if (old_copy_number == pConfig->get_genotype_parameters().get_pf_genotype_info()
                                   .chromosome_infos[aa_pos.chromosome_id]
                                   .get_genes()[aa_pos.gene_id]
                                   .get_max_copies()) {
//                    LOG(INFO) << "old_copy_number == max_copies";
                    new_aa_sequence[aa_pos.aa_index_in_aa_string] = NumberHelpers::single_digit_number_to_char(old_copy_number - 1);
                } else {
                    auto new_copy_number = pRandom->random_uniform() < 0.5 ? old_copy_number - 1 : old_copy_number + 1;
//                    LOG(INFO) << "else " << "pRandom->random_uniform(): " << pRandom->random_uniform() << " new_copy_number: " << new_copy_number;
                    new_aa_sequence[aa_pos.aa_index_in_aa_string] = NumberHelpers::single_digit_number_to_char(new_copy_number);
                }
            } else {
                auto &aa_list = pConfig->get_genotype_parameters().get_pf_genotype_info()
                        .chromosome_infos[aa_pos.chromosome_id]
                        .get_genes()[aa_pos.gene_id]
                        .get_aa_positions()[aa_pos.aa_id]
                        .get_amino_acids();
//                LOG(INFO) << "aa_list: [" << aa_list[0] << "," << aa_list[1] << "]";
                // draw random aa id
                auto new_aa_id = pRandom->random_uniform(aa_list.size() - 1);
//                LOG(INFO) << "pRandom->random_uniform(aa_list.size() - 1) " << pRandom->random_uniform(aa_list.size() - 1);
                auto old_aa = aa_sequence[aa_pos.aa_index_in_aa_string];
                auto new_aa = aa_list[new_aa_id][0];
//                LOG(INFO) << "Mutation old_aa: " << old_aa << " -> new_aa: " << new_aa;
//                if (new_aa == old_aa) {
//                    LOG(INFO) << "old_aa: " << old_aa << " == new_aa: " << new_aa;
//                    new_aa = aa_list[new_aa_id + 1];
//                }
                if (new_aa == old_aa) {
                  if (new_aa_id + 1 < aa_list.size()) {
                    new_aa = aa_list[new_aa_id + 1][0];
                  } else {
                    new_aa = aa_list[0][0];
                  }
                }
                new_aa_sequence[aa_pos.aa_index_in_aa_string] = new_aa;
                if (aa_pos.aa_index_in_aa_string == 9 && old_aa == 'K' && new_aa == 'T'){
                  spdlog::info("{} p: {} < {} select new_aa_id: {} from [0,{}] aa_list[new_aa_id] = aa_list[{}] = {}",
                            Model::get_instance().get_scheduler()->current_time(), p, mutation_probability_by_locus,
                            new_aa_id, aa_list.size() - 1, new_aa_id, aa_list[new_aa_id]);
                  spdlog::info("{} Mutation {} -> {} old:{} new: {} aa_pos_id: {} aa_pos: {}",
                            Model::get_instance().get_scheduler()->current_time(), p, mutation_probability_by_locus,
                            old_aa, new_aa, aa_pos_id, aa_pos.aa_index_in_aa_string);
                }
            }
        }
    }
  }
  // get genotype pointer from gene database based on aa sequence
  return pConfig->get_genotype_parameters().genotype_db.get_genotype(new_aa_sequence);
}

void Genotype::override_EC50_power_n(const std::vector<GenotypeParameters::OverrideEC50Pattern> &override_patterns, DrugDatabase *drug_db) {
  if (EC50_power_n.size() != drug_db->size()) {
    EC50_power_n.resize(drug_db->size());
  }

  for (const auto &pattern : override_patterns) {
    if (match_pattern(pattern.get_pattern())) {
      // override ec50 power n
      EC50_power_n[pattern.get_drug_id()] = pow(pattern.get_ec50(), drug_db->at(pattern.get_drug_id())->n());
      spdlog::trace("aa_sequence: {} OVERRIDE drug_id: {} genotype: {} EC50:{} n: {} EC50_power_n: {}",
                aa_sequence, pattern.get_drug_id(), aa_sequence, pattern.get_ec50(),
                drug_db->at(pattern.get_drug_id())->n(), EC50_power_n[pattern.get_drug_id()]);
    }
  }
}

bool Genotype::match_pattern(const std::string &pattern) {
  int id = 0;
  while (id < aa_sequence.length() && (aa_sequence[id] == pattern[id] || pattern[id] == '.')) {
    id++;
  }
  return id >= aa_sequence.length();
}

Genotype *Genotype::free_recombine_with(Config *pConfig, utils::Random *pRandom, Genotype *other) {
  // TODO: this function is not optimized 100%, use with care
  PfGenotypeStr new_pf_genotype_str = std::vector<ChromosomalGenotypeStr>(14);
  // for each chromosome
  for (int chromosome_id = 0; chromosome_id < pf_genotype_str.size(); ++chromosome_id) {
    if (pf_genotype_str[chromosome_id].empty()) continue;
    if (pf_genotype_str[chromosome_id].size() == 1) {
      // if single gene
      // draw random
      auto topOrBottom = pRandom->random_uniform();
      // if < 0.5 take from current, otherwise take from other
      auto gene_str = topOrBottom < 0.5 ? pf_genotype_str[chromosome_id][0] : other->pf_genotype_str[chromosome_id][0];
      new_pf_genotype_str[chromosome_id].push_back(gene_str);
    } else {
      // if multiple genes
      // draw random to determine whether
      // within chromosome recombination happens
      auto with_chromosome_recombination = pRandom->random_uniform();
      if (with_chromosome_recombination < pConfig->get_parasite_parameters()
        .get_recombination_parameters().get_within_chromosome_recombination_rate()) {
        // if happen draw a random crossover point based on ','
        auto cutting_gene_id = pRandom->random_uniform(pf_genotype_str[chromosome_id].size() - 1) + 1;
        // draw another random to do top-bottom or bottom-top cross over
        auto topOrBottom = pRandom->random_uniform();
        for (auto gene_id = 0; gene_id < cutting_gene_id; ++gene_id) {
          auto gene_str = topOrBottom < 0.5 ? pf_genotype_str[chromosome_id][gene_id]
                                            : other->pf_genotype_str[chromosome_id][gene_id];
          new_pf_genotype_str[chromosome_id].push_back(gene_str);
        }
        for (auto gene_id = cutting_gene_id; gene_id < pf_genotype_str[chromosome_id].size(); ++gene_id) {
          auto gene_str = topOrBottom < 0.5 ? other->pf_genotype_str[chromosome_id][gene_id]
                                            : pf_genotype_str[chromosome_id][gene_id];
          new_pf_genotype_str[chromosome_id].push_back(gene_str);
        }
      } else {
        // if not do the same with single gene
        auto topOrBottom = pRandom->random_uniform();
        for (int gene_id = 0; gene_id < pf_genotype_str[chromosome_id].size(); ++gene_id) {
          auto gene_str = topOrBottom < 0.5 ? pf_genotype_str[chromosome_id][gene_id]
                                            : other->pf_genotype_str[chromosome_id][gene_id];

          new_pf_genotype_str[chromosome_id].push_back(gene_str);
        }
      }
    }
  }

  auto new_aa_sequence = Convert_PfGenotypeStr_To_String(new_pf_genotype_str);
  return pConfig->get_genotype_parameters().genotype_db.get_genotype(new_aa_sequence);
}

std::string Genotype::Convert_PfGenotypeStr_To_String(const PfGenotypeStr &pfGenotypeStr) {
  std::stringstream ss;

  for (auto &chromosome : pfGenotypeStr) {
    for (auto &gene : chromosome) {
      ss << gene;
      if (gene != chromosome.back()) {
        ss << ",";
      }
    }

    if (&chromosome != &pfGenotypeStr.back()) {
      ss << "|";
    }
  }

  return ss.str();
}
Genotype *Genotype::free_recombine(Config *config, utils::Random *pRandom, Genotype *f, Genotype *m) {
  PfGenotypeStr new_pf_genotype_str = std::vector<ChromosomalGenotypeStr>(14);
  // for each chromosome
  for (int chromosome_id = 0; chromosome_id < f->pf_genotype_str.size(); ++chromosome_id) {
    if (f->pf_genotype_str[chromosome_id].empty()) continue;
    if (f->pf_genotype_str[chromosome_id].size() == 1) {
      // if single gene
      // draw random
      auto topOrBottom = pRandom->random_uniform();
      // if < 0.5 take from current, otherwise take from other
      auto gene_str = topOrBottom < 0.5 ? f->pf_genotype_str[chromosome_id][0] : m->pf_genotype_str[chromosome_id][0];
      new_pf_genotype_str[chromosome_id].push_back(gene_str);
    } else {
      // if multiple genes
      // draw random to determine whether
      // within chromosome recombination happens
      auto with_chromosome_recombination = pRandom->random_uniform();
      if (with_chromosome_recombination < config->get_parasite_parameters()
        .get_recombination_parameters().get_within_chromosome_recombination_rate()) {
        // if happen draw a random crossover point based on ','
        auto cutting_gene_id = pRandom->random_uniform(f->pf_genotype_str[chromosome_id].size() - 1) + 1;
        // draw another random to do top-bottom or bottom-top cross over
        auto topOrBottom = pRandom->random_uniform();
        for (auto gene_id = 0; gene_id < cutting_gene_id; ++gene_id) {
          auto gene_str = topOrBottom < 0.5 ? f->pf_genotype_str[chromosome_id][gene_id]
                                            : m->pf_genotype_str[chromosome_id][gene_id];
          new_pf_genotype_str[chromosome_id].push_back(gene_str);
        }
        for (auto gene_id = cutting_gene_id; gene_id < f->pf_genotype_str[chromosome_id].size(); ++gene_id) {
          auto gene_str = topOrBottom < 0.5 ? m->pf_genotype_str[chromosome_id][gene_id]
                                            : f->pf_genotype_str[chromosome_id][gene_id];
          new_pf_genotype_str[chromosome_id].push_back(gene_str);
        }
      } else {
        // if there is no within chromosome recombination
        // do the same with single gene
        auto topOrBottom = pRandom->random_uniform();
        for (int gene_id = 0; gene_id < f->pf_genotype_str[chromosome_id].size(); ++gene_id) {
          auto gene_str = topOrBottom < 0.5 ? f->pf_genotype_str[chromosome_id][gene_id]
                                            : m->pf_genotype_str[chromosome_id][gene_id];

          new_pf_genotype_str[chromosome_id].push_back(gene_str);
        }
      }
    }
  }

  auto new_aa_sequence = Convert_PfGenotypeStr_To_String(new_pf_genotype_str);
  return config->get_genotype_parameters().genotype_db.get_genotype(new_aa_sequence);
}
