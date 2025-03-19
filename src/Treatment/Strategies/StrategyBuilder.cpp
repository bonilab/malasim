#include "StrategyBuilder.h"
#include "IStrategy.h"
#include "SFTStrategy.h"
#include "Configuration/Config.h"
#include "CyclingStrategy.h"
#include "AdaptiveCyclingStrategy.h"
#include "DistrictMftStrategy.h"
#include "MFTAgeBasedStrategy.h"
#include "MFTStrategy.h"
#include "NestedMFTStrategy.h"
#include "MFTRebalancingStrategy.h"
#include "MFTMultiLocationStrategy.h"
#include "NestedMFTMultiLocationStrategy.h"
#include "NovelDrugIntroductionStrategy.h"

StrategyBuilder::StrategyBuilder() = default;

StrategyBuilder::~StrategyBuilder() = default;

IStrategy* StrategyBuilder::build(const YAML::Node &ns, const int &strategy_id) {
  const auto type = IStrategy::StrategyTypeMap[ns["type"].as<std::string>()];
  switch (type) {
    case IStrategy::SFT:
      return buildSFTStrategy(ns, strategy_id);
    case IStrategy::Cycling:
      return buildCyclingStrategy(ns, strategy_id);
    case IStrategy::AdaptiveCycling:
      return buildAdaptiveCyclingStrategy(ns, strategy_id);
    case IStrategy::MFT:
      return buildMFTStrategy(ns, strategy_id);
    case IStrategy::MFTRebalancing:
      return buildMFTRebalancingStrategy(ns, strategy_id);
    case IStrategy::NestedMFT:
      return buildNestedSwitchingStrategy(ns, strategy_id);
    case IStrategy::MFTMultiLocation:
      return buildMFTMultiLocationStrategy(ns, strategy_id);
    case IStrategy::NestedMFTMultiLocation:
      return buildNestedMFTDifferentDistributionByLocationStrategy(ns,strategy_id);
    case IStrategy::NovelDrugIntroduction:
      return buildNovelDrugIntroductionStrategy(ns, strategy_id);
    case IStrategy::DistrictMft:
      return buildDistrictMftStrategy(ns, strategy_id);
    case IStrategy::MFTAgeBased:
      return buildMFTAgeBasedStrategy(ns, strategy_id);
    default:
      return nullptr;
  }
}

void StrategyBuilder::add_therapies(const YAML::Node &ns, IStrategy* result) {
  for (auto i = 0; i < ns["therapy_ids"].size(); i++) {
    result->add_therapy(Model::get_config()->get_therapy_parameters().therapy_db[ns["therapy_ids"][i].as<int>()]);
  }
}

void StrategyBuilder::add_distributions(const YAML::Node &ns, DoubleVector &v) {
  for (auto i = 0; i < ns.size(); i++) {
    v.push_back(ns[i].as<double>());
  }
}

IStrategy* StrategyBuilder::buildSFTStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new SFTStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();
  result->add_therapy(Model::get_config()->get_therapy_parameters().therapy_db[ns["therapy_ids"][0].as<int>()]);
  return result;
}

IStrategy* StrategyBuilder::buildCyclingStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new CyclingStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  result->cycling_time = ns["cycling_time"].as<int>();
  result->next_switching_day = ns["cycling_time"].as<int>();

  add_therapies(ns, result);

  return result;
}

IStrategy* StrategyBuilder::buildAdaptiveCyclingStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new AdaptiveCyclingStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  result->trigger_value = ns["trigger_value"].as<double>();
  result->delay_until_actual_trigger = ns["delay_until_actual_trigger"].as<int>();
  result->turn_off_days = ns["turn_off_days"].as<int>();

  add_therapies(ns, result);
  return result;
}

IStrategy* StrategyBuilder::buildMFTStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new MFTStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  add_distributions(ns["distribution"], result->distribution);
  add_therapies(ns, result);
  return result;
}

IStrategy* StrategyBuilder::buildNestedSwitchingStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new NestedMFTStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  add_distributions(ns["start_distribution"], result->start_distribution);
  add_distributions(ns["start_distribution"], result->distribution);
  add_distributions(ns["peak_distribution"], result->peak_distribution);

  result->peak_after = ns["peak_after"].as<int>();

  for (int i = 0; i < ns["strategy_ids"].size(); i++) {
    result->add_strategy(
        Model::get_instance().strategy_db()[ns["strategy_ids"][i].as<int>()]);
  }

  return result;
}

IStrategy* StrategyBuilder::buildMFTRebalancingStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new MFTRebalancingStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  add_distributions(ns["distribution"], result->distribution);
  add_distributions(ns["distribution"], result->next_distribution);

  add_therapies(ns, result);

  result->update_duration_after_rebalancing = ns["update_duration_after_rebalancing"].as<int>();
  result->delay_until_actual_trigger = ns["delay_until_actual_trigger"].as<int>();
  result->latest_adjust_distribution_time = 0;

  return result;
}

IStrategy*
StrategyBuilder::buildMFTMultiLocationStrategy(const YAML::Node &ns, const int &strategy_id) {
  auto* result = new MFTMultiLocationStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  result->distribution.clear();
  result->distribution.resize(static_cast<unsigned long long int>(Model::get_instance().number_of_locations()));

  result->start_distribution.clear();
  result->start_distribution.resize(static_cast<unsigned long long int>(Model::get_instance().number_of_locations()));

  result->peak_distribution.clear();
  result->peak_distribution.resize(static_cast<unsigned long long int>(Model::get_instance().number_of_locations()));

  for (auto loc = 0; loc < Model::get_instance().number_of_locations(); loc++) {
    auto input_loc = ns["start_distribution_by_location"].size() < Model::get_instance().number_of_locations() ? 0 : loc;
    add_distributions(ns["start_distribution_by_location"][input_loc], result->distribution[loc]);
  }
  for (auto loc = 0; loc < Model::get_instance().number_of_locations(); loc++) {
    auto input_loc = ns["start_distribution_by_location"].size() < Model::get_instance().number_of_locations() ? 0 : loc;
    add_distributions(ns["start_distribution_by_location"][input_loc], result->start_distribution[loc]);
  }

  for (auto loc = 0; loc < Model::get_instance().number_of_locations(); loc++) {
    auto input_loc = ns["peak_distribution_by_location"].size() < Model::get_instance().number_of_locations() ? 0 : loc;
    add_distributions(ns["peak_distribution_by_location"][input_loc], result->peak_distribution[loc]);
  }

  add_therapies(ns, result);
  result->peak_after = ns["peak_after"].as<int>();
  return result;
}

IStrategy* StrategyBuilder::buildNestedMFTDifferentDistributionByLocationStrategy(const YAML::Node &ns,
                                                                                  const int &strategy_id) {
  auto* result = new NestedMFTMultiLocationStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  result->distribution.clear();
  result->distribution.resize(static_cast<unsigned long long int>(Model::get_instance().number_of_locations()));

  result->start_distribution.clear();
  result->start_distribution.resize(static_cast<unsigned long long int>(Model::get_instance().number_of_locations()));

  result->peak_distribution.clear();
  result->peak_distribution.resize(static_cast<unsigned long long int>(Model::get_instance().number_of_locations()));

  for (auto loc = 0; loc < Model::get_instance().number_of_locations(); loc++) {
    auto input_loc = ns["start_distribution_by_location"].size() < Model::get_instance().number_of_locations() ? 0 : loc;
    add_distributions(ns["start_distribution_by_location"][input_loc], result->distribution[loc]);
  }
  for (auto loc = 0; loc < Model::get_instance().number_of_locations(); loc++) {
    auto input_loc = ns["start_distribution_by_location"].size() < Model::get_instance().number_of_locations() ? 0 : loc;
    add_distributions(ns["start_distribution_by_location"][input_loc], result->start_distribution[loc]);
  }

  for (auto loc = 0; loc < Model::get_instance().number_of_locations(); loc++) {
    auto input_loc = ns["peak_distribution_by_location"].size() < Model::get_instance().number_of_locations() ? 0 : loc;
    add_distributions(ns["peak_distribution_by_location"][input_loc], result->peak_distribution[loc]);
  }

  for (auto i = 0; i < ns["strategy_ids"].size(); i++) {
    result->add_strategy(Model::get_instance().strategy_db()[ns["strategy_ids"][i].as<int>()]);
  }

  result->peak_after = ns["peak_after"].as<int>();
  //    std::cout << result->to_string() << std::endl;

  return result;
}

IStrategy*
StrategyBuilder::buildNovelDrugIntroductionStrategy(const YAML::Node &ns, const int strategy_id) {
  auto* result = new NovelDrugIntroductionStrategy();
  result->id = strategy_id;
  result->name = ns["name"].as<std::string>();

  add_distributions(ns["start_distribution"], result->start_distribution);
  add_distributions(ns["start_distribution"], result->distribution);
  add_distributions(ns["peak_distribution"], result->peak_distribution);

  result->peak_after = ns["peak_after"].as<int>();

  for (int i = 0; i < ns["strategy_ids"].size(); i++) {
    result->add_strategy(
        Model::get_instance().strategy_db()[ns["strategy_ids"][i].as<int>()]);
  }

  result->newly_introduced_strategy_id = ns["newly_introduced_strategy_id"].as<int>();
  result->tf_threshold = ns["tf_threshold"].as<double>();

  result->replacement_fraction =  ns["replacement_fraction"].as<double>();
  result->replacement_duration =  ns["replacement_duration"].as<double>();

  return result;
}

// Read the YAML and create the DistrictMftStrategy object.
IStrategy* StrategyBuilder::buildDistrictMftStrategy(const YAML::Node &node,
                                                     const int &strategy_id) {
  // Load the initial parts of the object
  auto* strategy = new DistrictMftStrategy();
  strategy->id = strategy_id;
  strategy->name = node["name"].as<std::string>();

  // Track the districts that have been assigned an MFT, we should see all of
  // them once
  std::vector<int> districts;

  // Get district ID range from SpatialData
  const auto& spatial_data = SpatialData::get_instance();
  const auto min_district_id = spatial_data.min_district_id;
  const auto max_district_id = spatial_data.max_district_id;
  const auto expected_district_count = spatial_data.district_count;

  // Read each of the definitions
  for (auto ndx = 0; ndx < node["definitions"].size(); ndx++) {
    // Read the MFT from the child node
    auto child = node["definitions"][std::to_string(ndx)];
    // Use unique_ptr for exception safety
    auto template_mft = std::make_unique<DistrictMftStrategy::MftStrategy>();

    // Make sure the sizes are valid
    if (child["therapy_ids"].size() != child["distribution"].size()) {
      spdlog::error("The The number of therapies and distributions should be the "
                    "same, reading {}", ndx);
      throw std::invalid_argument(
          "Matched therapy and distribution array size.");
    }

    // Read the therapy ids and make sure they make sense
    for (auto ndy = 0; ndy < child["therapy_ids"].size(); ndy++) {
      auto id = child["therapy_ids"][ndy].as<int>();
      if (id < 0) {
        spdlog::error("Drug id should not be less than zero, reading {}",ndx);
        throw std::invalid_argument("Drug id should not be less than zero.");
      }
      if (id > Model::get_config()->get_therapy_parameters().therapy_db.size()) {
        spdlog::error("Drug id exceeds count of known drugs, reading {}", ndx);
        throw std::invalid_argument("Drug id exceeds count of known drugs.");
      }
      template_mft->therapies.push_back(id);
    }

    // Read the distribution percentages for the MFT and make sure they make sense
    auto sum = 0.0f;
    for (auto ndy = 0; ndy < child["distribution"].size(); ndy++) {
      auto percent = child["distribution"][ndy].as<float>();
      if (percent <= 0.0) {
        spdlog::error("Distribution percentage cannot be less than or equal to "
                      "zero, reading {}",ndx);
        throw std::invalid_argument(
            "Distribution percentage cannot be less than or equal to zero.");
      }
      if (percent > 1.0) {
        spdlog::error("Distribution percentage cannot be greater than 100%, reading {}",ndx);
        throw std::invalid_argument(
            "Distribution percentage cannot be greater than 100%.");
      }
      sum += percent;
      template_mft->percentages.push_back(percent);
    }
    if (int(sum) != 1) {
      spdlog::error("Distribution percentage sum does not equal 100%, reading  {}",ndx);
      throw std::invalid_argument(
          "Distribution percentage sum must equal 100%.");
    }

    // Assign the MFT to each of the districts
    for (auto ndy = 0; ndy < child["district_ids"].size(); ndy++) {
      auto id = child["district_ids"][ndy].as<int>();

      // Validate district ID is within valid range
      if (id < min_district_id || id > max_district_id) {
        spdlog::error("Invalid district ID {}, valid range is {} to {}",
                                 id, min_district_id, max_district_id);
        throw std::invalid_argument("District ID out of valid range");
      }

      if (std::find(districts.begin(), districts.end(), id)
          != districts.end()) {
        spdlog::error("District {} encountered a second time, reading {}", id,ndx);
        throw std::invalid_argument("District duplication detected.");
      }

      // Create a new copy of the MFT for this district
      auto district_mft = std::make_unique<DistrictMftStrategy::MftStrategy>(*template_mft);
      strategy->set_district_strategy(id, std::move(district_mft));
      districts.push_back(id);
    }

    // No need to explicitly delete template_mft, unique_ptr handles cleanup
  }

  // All the distributions have been read and assigned, make sure each district has one
  if (districts.size() < expected_district_count) {
    spdlog::error("Number of districts with MFT assigned ({}) is less than total district count ({})",
                               districts.size(), expected_district_count);
    throw std::invalid_argument("Districts missing MFT assignment.");
  }

  // Everything looks good, return the strategy
  return strategy;
}

IStrategy* StrategyBuilder::buildMFTAgeBasedStrategy(const YAML::Node &node,
                                                     const int &strategyId) {
  auto* result = new MFTAgeBasedStrategy();
  result->id = strategyId;
  result->name = node["name"].as<std::string>();
  for (std::size_t i = 0; i < node["therapy_ids"].size(); i++) {
    result->add_therapy(Model::get_config()->get_therapy_parameters().therapy_db[node["therapy_ids"][i].as<int>()]);
  }
  add_distributions(node["age_boundaries"], result->age_boundaries);

  if (result->age_boundaries.size() != result->therapy_list.size() - 1) {
    spdlog::error("The number of age boundaries should be one less than the "
                  "number of therapies.");
    throw std::invalid_argument(
        "The number of age boundaries should be one less than the number of "
        "therapies.");
  }

  return result;
}