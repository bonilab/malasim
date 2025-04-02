#include "MatureGametocyteEvent.h"

#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Simulation/Model.h"
#include "Population/ClonalParasitePopulation.h"
#include "Population/Person/Person.h"
#include "Population/SingleHostClonalParasitePopulations.h"

// OBJECTPOOL_IMPL(MatureGametocyteEvent)

void MatureGametocyteEvent::do_execute() {
  // spdlog::info("Mature gametocyte event executed {}", get_id());
  auto* person = get_person();
  if (person == nullptr) {
    throw std::runtime_error("Person is nullptr");
  }
  if (person->get_all_clonal_parasite_populations()->contain(blood_parasite_)) {
    blood_parasite_->set_gametocyte_level(
        Model::get_config()->get_epidemiological_parameters().get_gametocyte_level_full());
  }
}
