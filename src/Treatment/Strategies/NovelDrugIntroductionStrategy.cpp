#include "NovelDrugIntroductionStrategy.h"
#include "Simulation/Model.h"
#include "Utils/Random.h"
#include "Treatment/Therapies/Therapy.h"
#include "MDC/ModelDataCollector.h"
#include "Configuration/Config.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "Core/Scheduler/Scheduler.h"
#include "SFTStrategy.h"
#include "MFTStrategy.h"

NovelDrugIntroductionStrategy::NovelDrugIntroductionStrategy() {
  name = "NovelDrugIntroductionStrategy";
  type = NovelDrugIntroduction;
}


std::string NovelDrugIntroductionStrategy::to_string() const {
  std::stringstream sstm;
  sstm << id << "-" << name << "-";
  sstm << NestedMFTStrategy::to_string();
  return sstm.str();
}

void NovelDrugIntroductionStrategy::monthly_update() {
  NestedMFTStrategy::monthly_update();

  if (!is_switched) {
    auto public_sector_strategy = strategy_list[0];

    int current_public_therapy_id =
        public_sector_strategy->type == SFT
        ? dynamic_cast<SFTStrategy*>(public_sector_strategy)->get_therapy_list()[0]->get_id()
        : dynamic_cast<MFTStrategy*>(public_sector_strategy)->therapy_list[0]->get_id();
    if (Model::get_scheduler()->current_time() > 3000 && Model::get_mdc()->current_tf_by_therapy()[current_public_therapy_id] >= tf_threshold) {

      // switch to novel drugs

      auto novel_SFT_strategy = Model::get_config()->strategy_db()[newly_introduced_strategy_id];

      auto* new_public_stategy = new NestedMFTStrategy();

      new_public_stategy->strategy_list.push_back(public_sector_strategy);
      new_public_stategy->strategy_list.push_back(novel_SFT_strategy);
      new_public_stategy->distribution.push_back(1 - replacement_fraction);
      new_public_stategy->distribution.push_back(replacement_fraction);

      new_public_stategy->start_distribution.push_back(1);
      new_public_stategy->start_distribution.push_back(0);

      new_public_stategy->peak_distribution.push_back(1 - replacement_fraction);
      new_public_stategy->peak_distribution.push_back(replacement_fraction);

      new_public_stategy->peak_after = replacement_duration;
      new_public_stategy->starting_time = Model::get_scheduler()->current_time();

      strategy_list[0] = new_public_stategy;
      new_public_stategy->id = static_cast<int>(Model::get_config()->
          get_strategy_parameters().strategy_db.size());

      Model::get_config()->strategy_db().push_back(new_public_stategy);

      //reset the time point to collect ntf
      Model::get_config()->get_simulation_timeframe().set_start_of_comparison_period(Model::get_scheduler()->current_time());

      //reset the total time to 10 years after this time point
      const date::sys_days next_10_year{date::year_month_day{Model::get_scheduler()->calendar_date} + date::years{10}};
      const auto new_total_time = Model::get_scheduler()->current_time() + TimeHelpers::number_of_days(
          Model::get_scheduler()->calendar_date, next_10_year
      );

      if (new_total_time > Model::get_config()->get_simulation_timeframe().get_total_time()) {
        //extend the scheduler
        Model::get_scheduler()->extend_total_time(new_total_time);
      }
      Model::get_config()->get_simulation_timeframe().set_total_time(new_total_time);

      std::cout << date::year_month_day{Model::get_scheduler()->calendar_date} << ": Switch to novel drug with id "
                << newly_introduced_strategy_id;
      std::cout << "New total time: " << new_total_time;
      is_switched = true;
    } else {

      // check and extend total time if total time is less than 10 years
      const date::sys_days next_10_year{date::year_month_day{Model::get_scheduler()->calendar_date} + date::years{10}};
      const auto new_total_time = Model::get_scheduler()->current_time() + TimeHelpers::number_of_days(
          Model::get_scheduler()->calendar_date, next_10_year
      );

      if (new_total_time > Model::get_config()->get_simulation_timeframe().get_total_time()) {
        //extend the scheduler
        Model::get_scheduler()->extend_total_time(new_total_time + 10 * 365);
        Model::get_config()->get_simulation_timeframe().set_total_time(new_total_time + 10 * 365);
        Model::get_config()->get_simulation_timeframe().set_start_of_comparison_period(new_total_time + 10 * 365);
      }
    }
  }
}
