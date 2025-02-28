#include "Population.h"

#include <Utils/Random.h>
#include <spdlog/spdlog.h>
#include <Simulation/Model.h>
#include <cfloat>

#include "ClinicalUpdateFunction.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Events/BirthdayEvent.h"
#include "Events/SwitchImmuneComponentEvent.h"
#include "Person/Person.h"
#include "Utils/Helpers/ObjectHelpers.h"
#include "ImmuneSystem/ImmuneSystem.h"
#include "ImmuneSystem/InfantImmuneComponent.h"
#include "ImmuneSystem/NonInfantImmuneComponent.h"
#include "MDC/ModelDataCollector.h"
#include "Mosquito/Mosquito.h"
#include "Utils/Index/PersonIndex.h"
#include "Utils/Index/PersonIndexAll.h"
#include "Utils/Index/PersonIndexByLocationMovingLevel.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"

Population::Population(Model* model){
  model_ = model;
  person_index_list_ = new PersonIndexPtrList();
  all_persons_ = new PersonIndexAll();

  person_index_list_->push_back(all_persons_);
}

Population::~Population() {
  // persons_.clear();
  // release memory for all persons
  if (all_persons_ != nullptr) {
    for (auto& person : all_persons_->vPerson()) {
      ObjectHelpers::delete_pointer<Person>(person);
    }
    all_persons_->vPerson().clear();
    all_persons_ = nullptr;
  }

  // release person_indexes
  if (person_index_list_ != nullptr) {
    for (PersonIndex* person_index : *person_index_list_) {
      ObjectHelpers::delete_pointer<PersonIndex>(person_index);
    }

    person_index_list_->clear();
    ObjectHelpers::delete_pointer<PersonIndexPtrList>(person_index_list_);
  }
}

void Population::add_person(Person* person) {
  // persons_.push_back(person);
  for (PersonIndex* person_index : *person_index_list_) {
    person_index->add(person);
  }
  person->set_population(this);

  // Update the count at the location
  popsize_by_location_[person->get_location()]++;
}

void Population::remove_dead_person(Person* person) {
  remove_person(person);
  ObjectHelpers::delete_pointer<Person>(person);
}

void Population::remove_person(Person* person) {
  // persons_.erase(std::ranges::remove(persons_, person).begin(), persons_.end());
  for (PersonIndex* person_index : *person_index_list_) {
    person_index->remove(person);
  }
  person->set_population(nullptr);
  popsize_by_location_[person->get_location()]--;
  assert(popsize_by_location_[person->get_location()] >= 0);
}

void Population::notify_change(Person* p, const Person::Property& property, const void* oldValue,
                               const void* newValue) {
  for (PersonIndex* person_index : *person_index_list_) {
    person_index->notify_change(p, property, oldValue, newValue);
  }
}

void Population::notify_movement(const int source, const int destination) {
  popsize_by_location_[source]--;
  assert(popsize_by_location_[source] >= 0);
  popsize_by_location_[destination]++;
}

std::size_t Population::size(const int& location, const int& age_class) {
  if (location == -1) {
    return all_persons_->size();
  }
  auto pi_lsa = get_person_index<PersonIndexByLocationStateAgeClass>();

  if (pi_lsa == nullptr) {
    return 0;
  }
  std::size_t temp = 0;
  if (age_class == -1) {
    for (auto state = 0; state < Person::NUMBER_OF_STATE - 1; state++) {
      for (auto ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
        temp += pi_lsa->vPerson()[location][state][ac].size();
      }
    }
  } else {
    for (auto state = 0; state < Person::NUMBER_OF_STATE - 1; state++) {
      temp += pi_lsa->vPerson()[location][state][age_class].size();
    }
  }
  return temp;
}

std::size_t Population::size(const int& location, const Person::HostStates& hs, const int& age_class) {
  if (location == -1) {
    return all_persons_->size();
  }
  auto pi_lsa = get_person_index<PersonIndexByLocationStateAgeClass>();
  return (pi_lsa->vPerson()[location][hs][age_class].size());
}

// new
std::size_t Population::size_residents_only(const int& location) {
  if (location == -1) {
    return all_persons_->size();
  }

  auto pi_lsa = get_person_index<PersonIndexByLocationStateAgeClass>();

  if (pi_lsa == nullptr) {
    return 0;
  }
  auto temp = 0ul;
  for (auto state = 0; state < Person::NUMBER_OF_STATE - 1; state++) {
    for (auto ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
      for (auto i = 0; i < pi_lsa->vPerson()[location][state][ac].size(); i++) {
        if (pi_lsa->vPerson()[location][state][ac][i]->get_residence_location() == location) {
          temp++;
        }
      }
    }
  }
  return temp;
}

// std::size_t Population::size() { return persons_.size(); }

std::size_t Population::size_at(const int &location) {
  return popsize_by_location_[location];
}


void Population::perform_infection_event() {
  //    std::cout << "Infection Event" << std::endl;

  PersonPtrVector today_infections;
  auto tracking_index = model_->get_scheduler()->current_time()
                        % model_->get_config()->get_epidemiological_parameters().get_number_of_tracking_days();
  for (auto loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    const auto force_of_infection = force_of_infection_for_N_days_by_location[tracking_index][loc];
    if (force_of_infection <= DBL_EPSILON) continue;

    const auto new_beta = model_->get_config()->get_spatial_settings().location_db[loc].beta
                          * model_->get_config()->get_seasonality_settings().get_seasonal_factor(model_->get_scheduler()->calendar_date, loc);

    auto poisson_means = new_beta * force_of_infection;

    auto number_of_bites = model_->get_random()->random_poisson(poisson_means);
    if (number_of_bites <= 0) continue;

    // data_collector store number of bites
    model_->get_mdc()->collect_number_of_bites(loc, number_of_bites);

    auto persons_bitten_today = model_->get_random()->roulette_sampling<Person>(
        number_of_bites, individual_relative_biting_by_location[loc], all_alive_persons_by_location[loc], false,
        sum_relative_biting_by_location[loc]);

    for (auto* person : persons_bitten_today) {
      assert(person->get_host_state() != Person::DEAD);
      person->increase_number_of_times_bitten();

      auto genotype_id = model_->get_mosquito()->random_genotype(loc, tracking_index);

      const auto p_infectious = model_->get_random()->random_flat(0.0, 1.0);
      // only infect with real infectious bite
      if (model_->get_config()->get_epidemiological_parameters().get_using_variable_probability_infectious_bites_cause_infection()) {
        if (p_infectious <= person->p_infection_from_an_infectious_bite()) {
          if (person->get_host_state() != Person::EXPOSED && person->liver_parasite_type() == nullptr) {
            person->get_today_infections()->push_back(genotype_id);
            today_infections.push_back(person);
          }
        }
      } else if (p_infectious <= model_->get_config()->get_transmission_settings().get_p_infection_from_an_infectious_bite()) {
        if (person->get_host_state() != Person::EXPOSED && person->liver_parasite_type() == nullptr) {
          person->get_today_infections()->push_back(genotype_id);
          today_infections.push_back(person);
        }
      }
    }
  }
  //    std::cout << "Solve infections"<< std::endl;
  // solve Multiple infections
  if (today_infections.empty()) return;

  for (auto* p : today_infections) {
    if (!p->get_today_infections()->empty()) {
      model_->get_mdc()->monthly_number_of_new_infections_by_location()[p->get_location()] += 1;
      model_->get_mdc()->record_1_infection(p->get_location());
    }
    p->randomly_choose_parasite();
  }

  today_infections.clear();
}

// void Population::initialize() {
//
//   const auto number_of_locations = model_->get_config()->get_spatial_settings().get_number_of_locations();
//
//   individual_relative_biting_by_location =
//       std::vector<std::vector<double>>(number_of_locations, std::vector<double>());
//   individual_relative_moving_by_location =
//       std::vector<std::vector<double>>(number_of_locations, std::vector<double>());
//   individual_foi_by_location = std::vector<std::vector<double>>(number_of_locations, std::vector<double>());
//
//   all_alive_persons_by_location = std::vector<std::vector<Person*>>(number_of_locations, std::vector<Person*>());
//
//   sum_relative_biting_by_location = std::vector<double>(number_of_locations, 0);
//   sum_relative_moving_by_location = std::vector<double>(number_of_locations, 0);
//
//   current_force_of_infection_by_location = std::vector<double>(number_of_locations, 0);
//
//   force_of_infection_for_N_days_by_location = std::vector<std::vector<double>>(
//       model_->get_config()->get_epidemiological_parameters().get_number_of_tracking_days(), std::vector<double>(number_of_locations, 0));
//
//   persons_.clear();
//   for (int i = 0; i < 10000; ++i) {
//     auto *person = new Person();
//     person->initialize();
//     if (model_->get_random()->random_uniform<double>(0, 1) < 0.5){ // 5% chance to schedule the event
//       const auto event = std::make_shared<BirthdayEvent>();
//       event->executable = true;
//       event->schedule_event(Model::get_instance()->get_scheduler(), person, model_->get_random()->random_uniform(0, 365));
//     }
//     add_person(person);
//   }
// }

void Population::initialize() {
  if (model_ != nullptr) {
    // those vector will be used in the initial infection
    const auto number_of_locations = model_->get_config()->get_spatial_settings().get_number_of_locations();

    // Prepare the population size vector
    popsize_by_location_ = IntVector(number_of_locations, 0);

    individual_relative_biting_by_location =
        std::vector<std::vector<double>>(number_of_locations, std::vector<double>());
    individual_relative_moving_by_location =
        std::vector<std::vector<double>>(number_of_locations, std::vector<double>());
    individual_foi_by_location = std::vector<std::vector<double>>(number_of_locations, std::vector<double>());

    all_alive_persons_by_location = std::vector<std::vector<Person*>>(number_of_locations, std::vector<Person*>());

    sum_relative_biting_by_location = std::vector<double>(number_of_locations, 0);
    sum_relative_moving_by_location = std::vector<double>(number_of_locations, 0);

    current_force_of_infection_by_location = std::vector<double>(number_of_locations, 0);

    force_of_infection_for_N_days_by_location = std::vector<std::vector<double>>(
        model_->get_config()->get_epidemiological_parameters().get_number_of_tracking_days(),
        std::vector<double>(number_of_locations, 0));

    // initalize other person index
    initialize_person_indices();

    // Initialize population
    auto &location_db = model_->get_config()->get_spatial_settings().location_db;
    for (auto loc = 0; loc < number_of_locations; loc++) {
     // spdlog::info("Cell {}, population {}", loc,
     //                         location_db[loc].population_size);

      const auto popsize_by_location = static_cast<int>(
          location_db[loc].population_size
          * model_->get_config()->get_population_demographic().get_artificial_rescaling_of_population_size());
      auto temp_sum = 0;
      for (auto age_class = 0;
           age_class < model_->get_config()->get_population_demographic().get_initial_age_structure().size();
           age_class++) {
        auto number_of_individual_by_loc_ageclass = 0;
        if (age_class == model_->get_config()->get_population_demographic().get_initial_age_structure().size() - 1) {
          number_of_individual_by_loc_ageclass = popsize_by_location - temp_sum;
        } else {
          number_of_individual_by_loc_ageclass = static_cast<int>(
              popsize_by_location * location_db[loc].age_distribution[age_class]);
          temp_sum += number_of_individual_by_loc_ageclass;
        }
        for (int i = 0; i < number_of_individual_by_loc_ageclass; i++) {
          generate_individual(loc, age_class);
        }
      }
      // spdlog::info("individual_relative_moving_by_location[{}] size: {} sum: {}",loc,
      //              individual_relative_moving_by_location[loc].size(),
      //              sum_relative_moving_by_location[loc]);
    }
  }
}

void Population::generate_individual(int location, int age_class) {
  auto p = new Person();
  p->initialize();

  p->set_location(location);
  p->set_residence_location(location);
  p->set_host_state(Person::SUSCEPTIBLE);

  // Set the age of the individual, which also sets the age class. Note that we
  // are defining the types to conform to the signature of random_uniform_int
  unsigned long age_from =
      (age_class == 0) ? 0
                       : model_->get_config()->get_population_demographic().get_initial_age_structure()[age_class - 1];
  unsigned long age_to = model_->get_config()->get_population_demographic().get_initial_age_structure()[age_class];
  p->set_age(static_cast<int>(
      Model::get_random()->random_uniform_int(age_from, age_to + 1)));

  auto days_to_next_birthday = static_cast<int>(Model::get_random()->random_uniform(
      static_cast<unsigned long>(Constants::DAYS_IN_YEAR)));
  auto simulation_time_birthday = TimeHelpers::get_simulation_time_birthday(
      days_to_next_birthday, p->get_age(), Model::get_scheduler()->calendar_date);
  p->set_birthday(simulation_time_birthday);

  if (simulation_time_birthday > 0) {
    spdlog::error("simulation_time_birthday have to be <= 0 when initializing population");
  }

  BirthdayEvent::schedule_event(Model::get_scheduler(), p, days_to_next_birthday);
  // RaptEvent::schedule_event(Model::get_scheduler(), p, days_to_next_birthday);

  // set immune component at 6 months
  if (simulation_time_birthday + Constants::DAYS_IN_YEAR / 2 >= 0) {
    if (p->get_age() > 0) {
      spdlog::error("Error in calculating simulation_time_birthday");
    }
    p->get_immune_system()->set_immune_component(new InfantImmuneComponent());
    // schedule for switch
    SwitchImmuneComponentEvent::schedule_for_switch_immune_component_event(
        model_->get_scheduler(), p,
        simulation_time_birthday + Constants::DAYS_IN_YEAR / 2);
  } else {
    // LOG(INFO) << "Adult: " << p->age() << " - " << simulation_time_birthday;
    p->get_immune_system()->
        set_immune_component(new NonInfantImmuneComponent());
  }

  auto immune_value = model_->get_random()->random_beta(
    model_->get_config()->get_immune_system_parameters().alpha_immune,
    model_->get_config()->get_immune_system_parameters().beta_immune);
  p->get_immune_system()->immune_component()->set_latest_value(immune_value);
  p->get_immune_system()->set_increase(false);


  p->set_innate_relative_biting_rate(Person::draw_random_relative_biting_rate(
    model_->get_random(), model_->get_config()));
  p->update_relative_biting_rate();

  p->set_moving_level(model_->get_config()->get_movement_settings().get_moving_level_generator()
    .draw_random_level(model_->get_random()));

  p->set_latest_update_time(0);

  int time = model_->get_random()->random_uniform(model_->get_config()->get_epidemiological_parameters().get_update_frequency()) + 1;
  p->generate_prob_present_at_mda_by_age();

  add_person(p);
  // spdlog::info("Population::initialize: person {} age {} location {} moving level {}",
  //   i, p->get_age(), loc, p->get_moving_level());

  individual_relative_biting_by_location[location].push_back(p->get_current_relative_biting_rate());
  individual_relative_moving_by_location[location].push_back(
      model_->get_config()->get_movement_settings().get_v_moving_level_value()[p->get_moving_level()]);

  sum_relative_biting_by_location[location] += p->get_current_relative_biting_rate();
  sum_relative_moving_by_location[location] +=
      model_->get_config()->get_movement_settings().get_v_moving_level_value()[p->get_moving_level()];

  all_alive_persons_by_location[location].push_back(p);
}

void Population::initialize_person_indices() {
  const int number_of_location = model_->get_config()->get_spatial_settings().get_number_of_locations();
  const int number_of_host_states = Person::NUMBER_OF_STATE;
  const int number_of_age_classes = model_->get_config()->get_population_demographic().get_number_of_age_classes();

  auto p_index_by_l_s_a =
      new PersonIndexByLocationStateAgeClass(number_of_location, number_of_host_states, number_of_age_classes);
  person_index_list_->push_back(p_index_by_l_s_a);

  auto p_index_location_moving_level = new PersonIndexByLocationMovingLevel(
      number_of_location, model_->get_config()->get_movement_settings().get_circulation_info().get_number_of_moving_levels());
  person_index_list_->push_back(p_index_location_moving_level);
}

void Population::introduce_initial_cases() {
  if (model_ != nullptr) {
    // std::cout << Model::CONFIG->initial_parasite_info().size() << std::endl;
    for (const auto p_info : Model::get_config()->get_genotype_parameters().get_initial_parasite_info()) {
      auto num_of_infections = Model::get_random()->random_poisson(std::round(size_at(p_info.location) * p_info.prevalence));
      num_of_infections = num_of_infections <= 0 ? 1 : num_of_infections;

      auto* genotype = Model::get_config()->get_genotype_parameters().genotype_db->at(p_info.parasite_type_id);
      spdlog::info("Introducing genotype {} with prevalence: {} : {} infections at location {}",
                p_info.parasite_type_id, p_info.prevalence, num_of_infections, p_info.location);
      introduce_parasite(p_info.location, genotype, num_of_infections);
    }
    // update current foi
    update_current_foi();

    // update force of infection for N days
    for (auto d = 0; d < Model::get_config()->get_epidemiological_parameters().get_number_of_tracking_days(); d++) {
      for (auto loc = 0; loc < Model::get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        force_of_infection_for_N_days_by_location[d][loc] = current_force_of_infection_by_location[loc];
      }
      Model::get_mosquito()->infect_new_cohort_in_PRMC(Model::get_config(), Model::get_random(), this, d);
    }
  }
}

void Population::introduce_parasite(const int& location, Genotype* parasite_type, const int& num_of_infections) {
  if (model_ != nullptr) {
    auto persons_bitten_today = model_->get_random()->roulette_sampling<Person>(
        num_of_infections, model_->get_population()->individual_relative_biting_by_location[location],
        model_->get_population()->all_alive_persons_by_location[location], false);

    for (auto* person : persons_bitten_today) {
      initial_infection(person, parasite_type);
    }
  }
}

void Population::initial_infection(Person* person, Genotype* parasite_type) const {
  person->get_immune_system()->set_increase(true);
  person->set_host_state(Person::ASYMPTOMATIC);

  auto* blood_parasite = person->add_new_parasite_to_blood(parasite_type);

  const auto size =
      model_->get_random()->random_flat(model_->get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_from_liver(),
                                    model_->get_config()->get_parasite_parameters().get_parasite_density_levels().get_log_parasite_density_clinical());

  blood_parasite->set_gametocyte_level(model_->get_config()->get_epidemiological_parameters().get_gametocyte_level_full());
  blood_parasite->set_last_update_log10_parasite_density(size);

  const auto p_clinical = person->get_probability_progress_to_clinical();
  const auto p = model_->get_random()->random_flat(0.0, 1.0);

  if (p < p_clinical) {
    // progress to clinical after several days
    blood_parasite->set_update_function(model_->progress_to_clinical_update_function());
    person->schedule_progress_to_clinical_event_by(blood_parasite);
  } else {
    // only progress to clearance by Immune system
    // progress to clearance
    blood_parasite->set_update_function(model_->immunity_clearance_update_function());
  }
}


void Population::perform_birth_event() {
  //    std::cout << "Birth Event" << std::endl;
  for (auto loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    auto poisson_means = size(loc) * model_->get_config()->get_population_demographic().get_birth_rate() / Constants::DAYS_IN_YEAR;
    const auto number_of_births = model_->get_random()->random_poisson(poisson_means);
    for (auto i = 0; i < number_of_births; i++) {
      give_1_birth(loc);
      model_->get_mdc()->update_person_days_by_years(
          loc, Constants::DAYS_IN_YEAR - model_->get_scheduler()->current_day_in_year());
    }
  }
  //    std::cout << "End Birth Event" << std::endl;
}

void Population::give_1_birth(const int& location) {
  auto p = new Person();
  p->initialize();
  p->set_age(0);
  p->set_host_state(Person::SUSCEPTIBLE);
  p->set_age_class(0);
  p->set_location(location);
  p->set_residence_location(location);
  p->get_immune_system()->set_immune_component(new InfantImmuneComponent());
  p->get_immune_system()->set_latest_immune_value(1.0);
  p->get_immune_system()->set_increase(false);

  p->set_latest_update_time(model_->get_scheduler()->current_time());
  //                    p->draw_random_immune();

  // set_relative_biting_rate
  p->set_innate_relative_biting_rate(Person::draw_random_relative_biting_rate(model_->get_random(), model_->get_config()));
  p->update_relative_biting_rate();

  p->set_moving_level(model_->get_config()->get_movement_settings().get_moving_level_generator().draw_random_level(model_->get_random()));

  p->set_birthday(model_->get_scheduler()->current_time());
  const auto number_of_days_to_next_birthday =
      TimeHelpers::number_of_days_to_next_year(model_->get_scheduler()->calendar_date);
  BirthdayEvent::schedule_event(model_->get_scheduler(), p,
                                model_->get_scheduler()->current_time() + number_of_days_to_next_birthday);
  // RaptEvent::schedule_event(Model::get_scheduler(), p, days_to_next_birthday);

  // schedule for switch
  SwitchImmuneComponentEvent::schedule_for_switch_immune_component_event(
      model_->get_scheduler(), p, model_->get_scheduler()->current_time() + Constants::DAYS_IN_YEAR / 2);

  //    p->startLivingTime = (Global::startTreatmentDay > Global::scheduler->currentTime) ? Global::startTreatmentDay :
  //    Global::scheduler->currentTime;
  p->generate_prob_present_at_mda_by_age();

  add_person(p);
}

void Population::perform_death_event() {
  //    std::cout << "Death Event" << std::endl;
  // simply change state to dead and release later
  auto pi = get_person_index<PersonIndexByLocationStateAgeClass>();
  if (pi == nullptr) return;

  for (auto loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    for (auto hs = 0; hs < Person::NUMBER_OF_STATE - 1; hs++) {
      if (hs == Person::DEAD) continue;
      for (auto ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
        const int size = pi->vPerson()[loc][hs][ac].size();
        if (size == 0) continue;
        auto poisson_means = size * model_->get_config()->get_population_demographic().get_death_rate_by_age_class()[ac] / Constants::DAYS_IN_YEAR;

        assert(model_->get_config()->get_population_demographic().get_death_rate_by_age_class().size() == model_->get_config()->get_population_demographic().get_number_of_age_classes());
        const auto number_of_deaths = model_->get_random()->random_poisson(poisson_means);
        if (number_of_deaths == 0) continue;

        //                std::cout << numberOfDeaths << std::endl;
        for (int i = 0; i < number_of_deaths; i++) {
          // change state to Death;
          const int index = model_->get_random()->random_uniform(size);
          //                    std::cout << index << "-" << pi->vPerson()[loc][hs][ac].size() << std::endl;
          auto* p = pi->vPerson()[loc][hs][ac][index];
          p->cancel_all_events_except(nullptr);
          p->set_host_state(Person::DEAD);
        }
      }
    }
  }
  clear_all_dead_state_individual();
}


void Population::clear_all_dead_state_individual() {
  // return all Death to object pool and clear vPersonIndex[l][dead][ac] for all location and ac
  auto pi = get_person_index<PersonIndexByLocationStateAgeClass>();
  PersonPtrVector removePersons;

  for (int loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    for (int ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
      for (auto *person : pi->vPerson()[loc][Person::DEAD][ac]) {
        removePersons.push_back(person);
      }
    }
  }

  for (Person* p : removePersons) {
    remove_dead_person(p);
  }
}

void Population::perform_circulation_event() {
  // for each location
  //  get number of circulations based on size * circulation_percent
  //  distributes that number into others location based of other location size
  //  for each number in that list select an individual, and schedule a movement event on next day
  PersonPtrVector today_circulations;

  std::vector<int> v_number_of_residents_by_location(model_->get_config()->get_spatial_settings().get_number_of_locations(), 0);

  for (auto location = 0; location < model_->get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    //        v_number_of_residents_by_location[target_location] = (size(target_location));
    v_number_of_residents_by_location[location] = model_->get_mdc()->popsize_residence_by_location()[location];
    //        std::cout << v_original_pop_size_by_location[target_location] << std::endl;
  }

  for (int from_location = 0; from_location < model_->get_config()->get_spatial_settings().get_number_of_locations(); from_location++) {
    auto poisson_means = size(from_location) * model_->get_config()->get_movement_settings().get_circulation_info().get_circulation_percent();
    if (poisson_means == 0) continue;
    const auto number_of_circulating_from_this_location = model_->get_random()->random_poisson(poisson_means);
    if (number_of_circulating_from_this_location == 0) continue;

    DoubleVector v_relative_outmovement_to_destination(model_->get_config()->get_spatial_settings().get_number_of_locations(), 0);
    v_relative_outmovement_to_destination = model_->get_config()->get_movement_settings().get_spatial_model()->get_v_relative_out_movement_to_destination(
        from_location, model_->get_config()->get_spatial_settings().get_number_of_locations(), model_->get_config()->get_spatial_settings().get_spatial_distance_matrix()[from_location],
        v_number_of_residents_by_location);

    std::vector<unsigned int> v_num_leavers_to_destination(
        static_cast<unsigned long long int>(model_->get_config()->get_spatial_settings().get_number_of_locations()));

    model_->get_random()->random_multinomial(static_cast<int>(v_relative_outmovement_to_destination.size()),
                                      static_cast<unsigned int>(number_of_circulating_from_this_location),
                                      v_relative_outmovement_to_destination, v_num_leavers_to_destination);

    for (int target_location = 0; target_location < model_->get_config()->get_spatial_settings().get_number_of_locations(); target_location++) {
      // spdlog::info("target_location individual_relative_moving_by_location[{}] size: {} sum: {}",target_location,
      //              individual_relative_moving_by_location[target_location].size(),
      //              sum_relative_moving_by_location[target_location]);
      // std::cout << "num_leave: " << v_num_leavers_to_destination[target_location] << std::endl;
      if (v_num_leavers_to_destination[target_location] == 0) continue;
      // std::cout << " time: " << model_->get_scheduler()->current_time() << "\t from " << from_location << "\t to " << target_location <<
      // "\t" << v_num_leavers_to_destination[target_location] << std::endl;
      perform_circulation_for_1_location(from_location, target_location, v_num_leavers_to_destination[target_location],
                                         today_circulations);
    }
  }

  for (auto* p : today_circulations) {
    p->randomly_choose_target_location();
  }

  today_circulations.clear();
}

void Population::perform_circulation_for_1_location(const int& from_location, const int& target_location,
                                                    const int& number_of_circulations,
                                                    std::vector<Person*>& today_circulations) {
  // spdlog::info("perform_circulation_for_1_location individual_relative_moving_by_location[{}] size: {} sum: {}",target_location,
  //              individual_relative_moving_by_location[target_location].size(),
  //              sum_relative_moving_by_location[target_location]);

  auto persons_moving_today = model_->get_random()->roulette_sampling<Person>(
      number_of_circulations, individual_relative_moving_by_location[from_location],
      all_alive_persons_by_location[from_location], false, sum_relative_moving_by_location[from_location]);

  for (auto* person : persons_moving_today) {
    assert(person->get_host_state() != Person::DEAD);

    person->get_today_target_locations()->push_back(target_location);
    today_circulations.push_back(person);
  }

}

bool Population::has_0_case() {
  auto pi = get_person_index<PersonIndexByLocationStateAgeClass>();
  for (int loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    for (int hs = Person::EXPOSED; hs <= Person::CLINICAL; hs++) {
      for (int ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
        if (!pi->vPerson()[loc][hs][ac].empty()) {
          return false;
        }
      }
    }
  }
  return true;
}

void Population::update_all_individuals() {
  // update all individuals
  auto pi = get_person_index<PersonIndexByLocationStateAgeClass>();
  for (int loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    for (int hs = 0; hs < Person::DEAD; hs++) {
      for (int ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
        for (auto* person : pi->vPerson()[loc][hs][ac]) {
          person->update();
        }
      }
    }
  }
}

void Population::persist_current_force_of_infection_to_use_N_days_later() {
  for (auto loc = 0; loc < model_->get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
    force_of_infection_for_N_days_by_location[model_->get_scheduler()->current_time()
                                              % model_->get_config()->get_epidemiological_parameters().get_number_of_tracking_days()][loc] =
        current_force_of_infection_by_location[loc];
  }
}

void Population::update_current_foi() {
  auto pi = get_person_index<PersonIndexByLocationStateAgeClass>();
  for (int location = 0; location < model_->get_config()->get_spatial_settings().get_number_of_locations(); location++) {
    // reset force of infection for each location
    current_force_of_infection_by_location[location] = 0.0;
    sum_relative_biting_by_location[location] = 0.0;
    sum_relative_moving_by_location[location] = 0.0;

    // using clear so as system will not reallocate memory slot for vector
    individual_foi_by_location[location].clear();
    individual_relative_biting_by_location[location].clear();
    individual_relative_moving_by_location[location].clear();
    all_alive_persons_by_location[location].clear();

    for (int hs = 0; hs < Person::DEAD; hs++) {
      for (int ac = 0; ac < model_->get_config()->get_population_demographic().get_number_of_age_classes(); ac++) {
        // spdlog::info("There are {} individuals in location {} with host state {} and age class {}", pi->vPerson()[location][hs][ac].size(), location, hs, ac);
        // for (std::size_t i = 0; i < pi->vPerson()[location][hs][ac].size(); i++) {
          // Person* person = pi->vPerson()[location][hs][ac][i];
        for (auto* person : (pi->vPerson()[location][hs][ac])) {
          double log_10_total_infectious_density =
              person->get_all_clonal_parasite_populations()->log10_total_infectious_denstiy;

          auto individual_foi = log_10_total_infectious_density == ClonalParasitePopulation::LOG_ZERO_PARASITE_DENSITY
                                    ? 0.0
                                    : person->get_current_relative_biting_rate()
                                          * Person::relative_infectivity(log_10_total_infectious_density);

          individual_foi_by_location[location].push_back(individual_foi);
          individual_relative_biting_by_location[location].push_back(person->get_current_relative_biting_rate());
          individual_relative_moving_by_location[location].push_back(
              model_->get_config()->get_movement_settings().get_v_moving_level_value()[person->get_moving_level()]);

          sum_relative_biting_by_location[location] += person->get_current_relative_biting_rate();
          sum_relative_moving_by_location[location] +=
              model_->get_config()->get_movement_settings().get_v_moving_level_value()[person->get_moving_level()];
          current_force_of_infection_by_location[location] += individual_foi;
          all_alive_persons_by_location[location].push_back(person);
          // spdlog::info("person {} individual_relative_moving_by_location[{}] {}",person->get_id(), location,
          //              individual_relative_moving_by_location[location].back());
        }
      }
    }
    // spdlog::info("update_current_foi individual_relative_moving_by_location[{}] size: {} sum: {}",location,
    //                individual_relative_moving_by_location[location].size(),
    //                sum_relative_moving_by_location[location]);
  }
}

/*
 * NEW KIEN
*/

// void Population::update(int current_time) {
//   for (const auto& person : persons_) {
//     // spdlog::info("Updating person {} with events {}", person->get_id(), person->event_queue.size());
//     person->update(current_time);
//   }
// }