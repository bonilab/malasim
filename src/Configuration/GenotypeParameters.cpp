#include "GenotypeParameters.h"

#include "Simulation/Model.h"

void GenotypeParameters::process_config_with_number_of_locations(size_t number_of_locations) {
  spdlog::info("Processing GenotypeParameters");
  for (const auto &initial_genotype_info_raw : get_initial_parasite_info_raw()) {
    const auto location = initial_genotype_info_raw.get_location_id();
    const auto location_from = location == -1 ? 0 : location;
    const auto location_to =
        location == -1 ? number_of_locations : std::min<size_t>(location + 1, number_of_locations);

    // apply for all location
    for (auto loc = location_from; loc < location_to; ++loc) {
      for (const auto &parasite_node : initial_genotype_info_raw.get_parasite_info()) {
        const auto &aa_sequence = parasite_node.get_aa_sequence();
        auto parasite_type_id = Model::get_genotype_db()->get_id(aa_sequence);
        auto prevalence = parasite_node.get_prevalence();
        initial_parasite_info_.emplace_back(loc, parasite_type_id, prevalence);
      }
    }
    // for(auto &initial_genotype_info : get_initial_parasite_info()) {
    // spdlog::debug("Location: {} parasite_type_id: {} prevalence: {}",
    //              initial_genotype_info.location, initial_genotype_info.parasite_type_id,
    // initial_genotype_info.prevalence);
    // }
  }
}

