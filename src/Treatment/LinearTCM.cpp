#include "LinearTCM.h"

#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"

void LinearTCM::monthly_update() {
  if (Model::get_scheduler()->current_time() <= end_time) {
    for (std::size_t loc = 0; loc < p_treatment_under_5.size(); loc++) {
      p_treatment_under_5[loc] += rate_of_change_under_5[loc];
      p_treatment_over_5[loc] += rate_of_change_over_5[loc];
    }
  }
}

void LinearTCM::update_rate_of_change() {
  for (std::size_t loc = 0; loc < p_treatment_under_5.size(); loc++) {
    rate_of_change_under_5.push_back(30 * (p_treatment_under_5_to[loc] - p_treatment_under_5[loc])
                                     / (end_time - starting_time));
    rate_of_change_over_5.push_back(30 * (p_treatment_over_5_to[loc] - p_treatment_over_5[loc])
                                    / (end_time - starting_time));
  }
}
