#include "NovelDrugReporter.h"

#include <Configuration/Config.h>
#include <MDC/ModelDataCollector.h>
#include <Simulation/Model.h>
#include <Population/Population.h>
#include <Utils/Index/PersonIndexByLocationStateAgeClass.h>
#include <Population/SingleHostClonalParasitePopulations.h>
#include <Treatment/Strategies/IStrategy.h>
#include <Treatment/Strategies/NestedMFTStrategy.h>
#include <Treatment/Strategies/NovelDrugIntroductionStrategy.h>

#include "Core/Scheduler/Scheduler.h"
#include "Parasites/Genotype.h"
#include "Treatment/ITreatmentCoverageModel.h"
#include "Utility/ReporterUtils.h"

void NovelDrugReporter::initialize(int job_number, const std::string &path) {}

void NovelDrugReporter::before_run() {
  // output header for csv file
  ss << "TIME" << sep << "PFPR" << sep << "MUTATIONS" << sep << "NUMBER_OF_TREATMENTS" << sep
     << "NUMBER_OF_TREATMENT_FAILURES" << sep << "NUMBER_OF_SYMPTOMATIC_CASES" << sep;
  for (auto i = 0; i < Model::get_instance().get_config()->get_genotype_parameters().genotype_db.size(); i++) {
    ss << "GENOTYPE_ID_" << i << sep;
  }
  ss << "IS_SWITCHED" << sep;
  for (auto i = 0; i < Model::get_instance().get_mdc()->current_tf_by_therapy().size(); i++) {
    ss << "TF_THERAPY_" << i << sep;
  }
  ss << "AVERAGE_TF_60" << sep;
  ss << "PUBLIC_FRACTION" << sep;
  ss << "PRIVATE_FRACTION";
  // CLOG(INFO, "monthly_reporter") << ss.str();
  spdlog::info("monthly_reporter {}", ss.str());
  ss.str("");
}

void NovelDrugReporter::monthly_report() {
  ss << Model::get_instance().get_scheduler()->current_time() << sep;

  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); ++loc) {
    ss << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location()[loc] * 100 << sep;
  }

  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); ++loc) {
    ss << Model::get_instance().get_mdc()->monthly_number_of_mutation_events_by_location()[loc] << sep;
    ss << Model::get_instance().get_mdc()->monthly_number_of_treatment_by_location()[loc] << sep;
    ss << Model::get_instance().get_mdc()->monthly_number_of_TF_by_location()[loc] << sep;
    ss << Model::get_instance().get_mdc()->monthly_number_of_clinical_episode_by_location()[loc] << sep;
  }

  output_genotype_frequency_3(Model::get_instance().get_config()->get_genotype_parameters().genotype_db.size(),
                              Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());

  ss << (dynamic_cast<NovelDrugIntroductionStrategy*>(Model::get_instance().get_treatment_strategy())->is_switched ? 1 : 0) << sep;
  for (const auto& tf_by_therapy : Model::get_instance().get_mdc()->current_tf_by_therapy()) {
    ss << tf_by_therapy << sep;
  }

  ss << Model::get_instance().get_mdc()->current_TF_by_location()[0] << sep;

  ss << dynamic_cast<NestedMFTStrategy*>(Model::get_instance().get_treatment_strategy())->distribution[0] << sep;
  ss << dynamic_cast<NestedMFTStrategy*>(Model::get_instance().get_treatment_strategy())->distribution[1];

  // CLOG(INFO, "monthly_reporter") << ss.str();
  spdlog::info("monthly_reporter {}", ss.str());
  ss.str("");
}

void NovelDrugReporter::after_run() {
  ss.str("");
  for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); ++loc) {
    ss << Model::get_instance().get_config()->get_spatial_settings().location_db[loc].beta << sep;
    if (Model::get_instance().get_mdc()->EIR_by_location_year()[loc].empty()) {
      ss << 0 << sep;
    } else {
      ss << Model::get_instance().get_mdc()->EIR_by_location_year()[loc].back() << sep;
    }
    ss << Model::get_instance().get_treatment_coverage()->p_treatment_less_than_5[0] << sep;
    ss << Model::get_instance().get_mdc()->cumulative_number_treatments_by_location()[loc] << sep;
    ss << Model::get_instance().get_mdc()->cumulative_TF_by_location()[loc] << sep;
    ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location()[loc] << sep;
    ss << std::to_string(
        dynamic_cast<NovelDrugIntroductionStrategy*>(Model::get_instance().get_treatment_strategy())->newly_introduced_strategy_id)
       << sep;
    ss << std::to_string(dynamic_cast<NovelDrugIntroductionStrategy*>(Model::get_instance().get_treatment_strategy())->tf_threshold) << sep;
    ss << std::to_string(dynamic_cast<NovelDrugIntroductionStrategy*>(Model::get_instance().get_treatment_strategy())->replacement_fraction)
       << sep;
    ss << std::to_string(dynamic_cast<NovelDrugIntroductionStrategy*>(Model::get_instance().get_treatment_strategy())->replacement_duration)
       << sep;
    ss << "FLT" << sep;
    ss << "importation" << sep;
  }

  // CLOG(INFO, "summary_reporter") << ss.str();
  spdlog::info("summary_reporter {}", ss.str());
  ss.str("");
}

void NovelDrugReporter::begin_time_step() {}

void NovelDrugReporter::output_genotype_frequency_3(const int& number_of_genotypes,
                                                    PersonIndexByLocationStateAgeClass* pi) {
  auto sum1_all = 0.0;
  std::vector<double> result3_all(number_of_genotypes, 0.0);
  const auto number_of_locations = pi->vPerson().size();
  const auto number_of_age_classes = pi->vPerson()[0][0].size();

  for (auto loc = 0; loc < number_of_locations; loc++) {
    std::vector<double> result3(number_of_genotypes, 0.0);
    auto sum1 = 0.0;

    for (auto hs = 0; hs < Person::NUMBER_OF_STATE - 1; hs++) {
      for (auto ac = 0; ac < number_of_age_classes; ac++) {
        const auto size = pi->vPerson()[loc][hs][ac].size();
        for (auto i = 0ull; i < size; i++) {
          auto* person = pi->vPerson()[loc][hs][ac][i];

          if (!person->get_all_clonal_parasite_populations()->parasites()->empty()) {
            sum1 += 1;
            sum1_all += 1;
          }

          std::map<int, int> individual_genotype_map;

          for (auto* parasite_population : *(person->get_all_clonal_parasite_populations()->parasites())) {
            const auto g_id = parasite_population->genotype()->genotype_id;
            if (individual_genotype_map.find(g_id) == individual_genotype_map.end()) {
              individual_genotype_map[parasite_population->genotype()->genotype_id] = 1;
            } else {
              individual_genotype_map[parasite_population->genotype()->genotype_id] += 1;
            }
          }

          for (const auto genotype : individual_genotype_map) {
            result3[genotype.first] +=
                genotype.second / static_cast<double>(person->get_all_clonal_parasite_populations()->parasites()->size());
            result3_all[genotype.first] +=
                genotype.second / static_cast<double>(person->get_all_clonal_parasite_populations()->parasites()->size());
          }
        }
      }
    }
    // output per location
    for (auto& i : result3) {
      i /= sum1;
      ss << (sum1 == 0 ? 0 : i) << sep;
    }
  }
}
