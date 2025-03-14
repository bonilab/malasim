/**
 * IntroduceMutantEventBuilder.cpp
 *
 * This file contains the implementation details for events that inherit from
 * the IntroduceMutantEventBase class.
 */
#include "Configuration/Config.h"
#include "Events/Population/IntroduceMutantEvent.hxx"
#include "Events/Population/IntroduceMutantRasterEvent.hxx"
#include "Events/Population/PopulationEventBuilder.h"
#include "Spatial/GIS/AscFile.h"
#include "Spatial/GIS/SpatialData.h"
#include "Simulation/Model.h"

// Parse the file provided and return the location ids from it.
//
// NOTE That this is a more in line with the C way of doing things, but we are
// keeping this logic isolated to just where
//      it will be used as opposed to having it somewhere else.
std::vector<int> get_locations_from_raster(const std::string &filename) {
  // Load the ASC file, any errors will be caught by the build function
  auto* file = AscFileManager::read(filename);

  // Note the number of locations so that we can provide some error checking
  auto count = Model::get_config()->get_spatial_settings().get_number_of_locations();

  // Iterate through the raster and note the valid locations
  std::vector<int> locations;
  auto id = 0;
  for (auto row = 0; row < file->NROWS; row++) {
    for (auto col = 0; col < file->NCOLS; col++) {
      if (file->data[row][col] == file->NODATA_VALUE) { continue; }

      if (id > count) {
        throw std::runtime_error(
            fmt::format("Raster misalignment: pixel count exceeds {} expected "
                        "while loading {}",
                        count, filename));
      }

      switch (static_cast<int>(file->data[row][col])) {
        case 0:
          break;
        case 1:
          locations.emplace_back(id);
          break;
        default:
          throw std::runtime_error(
              fmt::format("Raster for mutation events should only be zero or "
                          "one, found {} at {}, {} in {}",
                          file->data[row][col], row, col, filename));
      }

      // Update the id counter
      id++;
    }
  }

  // Verify that the total number of locations matches the location database
  if (id != count) {
    throw std::runtime_error(fmt::format(
        "Raster misalignment: found {} pixels, expected {} while loading {}",
        id, count, filename));
  }

  // Return the locations
  return locations;
}

std::vector<Event*> PopulationEventBuilder::build_introduce_mutant_event(
    const YAML::Node &node, Config* config) {
  try {
    std::vector<Event*> events;
    for (const auto &entry : node) {
      // Load the values
      auto start_date = entry["date"].as<date::year_month_day>();
      auto time =
          (date::sys_days{start_date} - date::sys_days{config->get_simulation_timeframe().get_starting_date()})
              .count();
      auto district = entry["district"].as<int>();
      auto fraction = entry["fraction"].as<double>();
      std::vector<std::tuple<int,int,char>> alleles;
      for (const auto &allele_node : entry["alleles"]) {
        if (allele_node["allele"].as<std::string>().size() > 1) {
          spdlog::error("Allele {} should be 1 character",allele_node["allele"].as<std::string>());
        }
        else {
          alleles.push_back(std::tuple(allele_node["chromosome"].as<int>(),allele_node["locus"].as<int>(),
            allele_node["allele"].as<std::string>().front()));
        }
      }
      for (auto & allele : alleles) {
        spdlog::info("Mutation at {}:{} {}",std::get<0>(allele),std::get<1>(allele),std::get<2>(allele));
      }

      // Make sure the district GIS data is loaded and the district id makes
      // sense
      if (district < 0) {
        spdlog::error("The target district must be greater than or equal to zero.");
        throw std::invalid_argument(
            "Target district must be greater than or equal to zero");
      }
      if (district > SpatialData::get_instance().district_count) {
        spdlog::error("Target district is greater than the district count.");
        throw std::invalid_argument(
            "Target district greater than district count.");
      }

      // Make sure the fraction makes sense
      if (fraction <= 0) {
        spdlog::error("The fraction of the mutants must be greater than zero.");
        throw std::invalid_argument(
            "Mutant fraction must be greater than zero");
      }

      // Log and add the event to the queue
      auto* event = new IntroduceMutantEvent(time, district, fraction,alleles);
      spdlog::debug(
          "Adding event {} start date: {} district: {} fraction: {}",
          event->name(), StringHelpers::date_as_string(start_date), district, fraction);
      events.push_back(event);
    }
    return events;
  } catch (YAML::BadConversion &error) {
    spdlog::error("Unrecoverable error parsing YAML value in {}"
                  " node: {}",
                  IntroduceMutantEvent::EventName, error.msg);
    exit(1);
  }
}

std::vector<Event*> PopulationEventBuilder::build_introduce_mutant_raster_event(
    const YAML::Node &node, Config* config) {
  try {
    std::vector<Event*> events;
    for (const auto &entry : node) {
      // Load the values
      auto start_date = entry["date"].as<date::year_month_day>();
      auto time =
          (date::sys_days{start_date} - date::sys_days{config->get_simulation_timeframe().get_starting_date()})
              .count();
      auto filename = entry["raster"].as<std::string>();
      auto fraction = entry["fraction"].as<double>();
      std::vector<std::tuple<int,int,char>> alleles;
      for (const auto &allele_node : entry["alleles"]) {
        if (allele_node["allele"].as<std::string>().size() > 1) {
          spdlog::error("Allele {} should be 1 character",allele_node["allele"].as<std::string>());
        }
        else {
          alleles.push_back(std::tuple(allele_node["chromosome"].as<int>(),allele_node["locus"].as<int>(),
            allele_node["allele"].as<std::string>().front()));
        }
      }
      for (auto & allele : alleles) {
        spdlog::info("Mutation at {}:{} {}",std::get<0>(allele),std::get<1>(allele),std::get<2>(allele));
      }

      // Make sure the fraction makes sense
      if (fraction <= 0) {
        spdlog::error("The fraction of the mutants must be greater than zero.");
        throw std::invalid_argument(
            "Mutant fraction must be greater than zero");
      }

      // Load the locations from the file, then add the new event to the queue
      auto locations = get_locations_from_raster(filename);
      auto* event = new IntroduceMutantRasterEvent(time, locations, fraction,alleles);
      spdlog::debug(
          "Adding event {} start date: {} with {} locations fraction: {}",
          event->name(), StringHelpers::date_as_string(start_date),
          locations.size(),fraction);
      events.push_back(event);
    }
    return events;
  } catch (YAML::BadConversion &error) {
    spdlog::error("Unrecoverable error parsing YAML value in {}"
                  " node: {}",
                  IntroduceMutantRasterEvent::EventName, error.msg);
    exit(EXIT_FAILURE);
  } catch (std::runtime_error &error) {
    // Runtime errors are most likely to be thrown when reading the ASC file, so
    // just echo what it says
    spdlog::error("Unrecoverable error processing YAML file: {}", error.what());
    exit(EXIT_FAILURE);
  }
}
