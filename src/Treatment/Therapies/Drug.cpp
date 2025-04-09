#include "Drug.h"

#include <cmath>

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Helpers/NumberHelpers.h"
#include "Parasites/Genotype.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"
#include "Treatment/Therapies/DrugType.h"

Drug::Drug(DrugType *drug_type)
    : dosing_days_(0),
      start_time_(0),
      end_time_(0),
      last_update_value_(1.0),
      last_update_time_(0),
      starting_value_(1.0),
      drug_type_(drug_type),
      person_drugs_(nullptr) {}

Drug::~Drug() = default;

void Drug::update() {
  const auto current_time = Model::get_scheduler()->current_time();
  last_update_value_ = get_current_drug_concentration(current_time);
  last_update_time_ = current_time;
}

double Drug::get_current_drug_concentration(int currentTime) {
  const auto days = currentTime - start_time_;
  if (days == 0) {
    return 0;
  }

  if (days <= dosing_days_) {
    if (drug_type()->id() == 0) {
      // drug is artemisinin
      const auto starting_value_temp_ = Model::get_random()->random_uniform<double>(-0.2, 0.2);

      // std::cout << "days: " << days << " start: " << starting_value_ << " uniform: " << starting_value_temp_ << " = " << starting_value_ + starting_value_temp_ << std::endl;
      return starting_value_ + starting_value_temp_;
      //       return  Model::get_random()->random_normal(starting_value_, Model::CONFIG->as_iov());

      // starting_value_ += Model::get_random()->random_uniform<double>(0, 0.2);
      // return starting_value_;
      //            return starting_value_;
    }

    starting_value_ += days >= 1 ? Model::get_random()->random_uniform<double>(0, 0.1) : 0;
    //        return starting_value_ + Model::get_random()->random_uniform<double>(-0.1, 0.1);
    return starting_value_;
  }
  else
    {
    const auto temp = NumberHelpers::is_equal(drug_type_->drug_half_life(), 0.0)
                          ? -100
                          : -(days - dosing_days_) * log(2) / drug_type_->drug_half_life();  //-ai*t = - t* ln2 / tstar
    if (exp(temp) <= (10.0 / 100.0)) {
      return 0;
    }
    return starting_value_ * exp(temp);
  }
}

double Drug::get_mutation_probability(double currentDrugConcentration) const {
  double P = 0;
  double mutation_prob_by_locus = Model::get_config()->get_genotype_parameters().get_mutation_probability_per_locus();
  if (currentDrugConcentration <= 0) return 0;
  if (currentDrugConcentration < (0.5))
    P = 2 * mutation_prob_by_locus * drug_type_->k() * currentDrugConcentration;

  else if (currentDrugConcentration >= (0.5) && currentDrugConcentration < 1.0) {
    P = mutation_prob_by_locus * (2 * (1 - drug_type_->k()) * currentDrugConcentration + (2 * drug_type_->k() - 1));
  } else if (currentDrugConcentration >= 1.0) {
    P = mutation_prob_by_locus;
  }
  //    cout << P << endl;
  return P;
}

double Drug::get_mutation_probability() const {
  return get_mutation_probability(last_update_value_);
}

void Drug::set_number_of_dosing_days(int dosingDays) {
  dosing_days_ = dosingDays;

  last_update_value_ = 1.0;
  last_update_time_ = Model::get_scheduler()->current_time();

  start_time_ = last_update_time_;
  end_time_ = last_update_time_ + drug_type_->get_total_duration_of_drug_activity(dosingDays);
}

double Drug::get_parasite_killing_rate(const int &genotype_id) const {
  return drug_type_->get_parasite_killing_rate_by_concentration(
      last_update_value_, Model::get_genotype_db()->at(genotype_id)->get_EC50_power_n(drug_type_));
}
