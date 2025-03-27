/* 
 * File:   Statistic.h
 * Author: Merlin
 *
 * Created on July 9, 2013, 2:28 PM
 */

#ifndef MODELDATACOLLECTOR_H
#define MODELDATACOLLECTOR_H

#include "Utils/TypeDef.hxx"

class Model;

class Genotype;

class Person;

class Therapy;

class ClonalParasitePopulation;

class ModelDataCollector {
  //disallow copy and assign
  ModelDataCollector(const ModelDataCollector&) = delete;
  void operator=(const ModelDataCollector&) = delete;
  static const int NUMBER_OF_REPORTED_MOI = 8;

// POINTER_PROPERTY(Model, model)
//
// PROPERTY_REF(DoubleVector, total_immune_by_location)
//
// PROPERTY_REF(DoubleVector2, total_immune_by_location_age_class)
//
// PROPERTY_REF(IntVector, popsize_by_location)
//
// PROPERTY_REF(IntVector, popsize_residence_by_location)
//
// PROPERTY_REF(IntVector2, popsize_by_location_age_class)
//
// PROPERTY_REF(IntVector2, popsize_by_location_age_class_by_5)
//
// PROPERTY_REF(IntVector2, popsize_by_location_hoststate)
//
// PROPERTY_REF(IntVector3, popsize_by_location_hoststate_age_class)
//
// PROPERTY_REF(DoubleVector, blood_slide_prevalence_by_location)
//
// PROPERTY_REF(DoubleVector2, blood_slide_number_by_location_age_group)
//
// PROPERTY_REF(DoubleVector2, blood_slide_prevalence_by_location_age_group)
//
// PROPERTY_REF(DoubleVector2, blood_slide_number_by_location_age_group_by_5)
//
// PROPERTY_REF(DoubleVector2, blood_slide_prevalence_by_location_age_group_by_5)
//
// PROPERTY_REF(DoubleVector2, blood_slide_prevalence_by_location_age)
//
// PROPERTY_REF(DoubleVector2, blood_slide_number_by_location_age)
//
// PROPERTY_REF(DoubleVector, fraction_of_positive_that_are_clinical_by_location)
//
// PROPERTY_REF(LongVector, total_number_of_bites_by_location)
//
// PROPERTY_REF(LongVector, total_number_of_bites_by_location_year)
//
// PROPERTY_REF(LongVector, person_days_by_location_year)
//
// PROPERTY_REF(DoubleVector2, EIR_by_location_year)
//
// PROPERTY_REF(DoubleVector, EIR_by_location)
//
// PROPERTY_REF(LongVector, cumulative_clinical_episodes_by_location)
//
// PROPERTY_REF(LongVector2, cumulative_clinical_episodes_by_location_age)
//
// PROPERTY_REF(LongVector2, cumulative_clinical_episodes_by_location_age_group)
//
// PROPERTY_REF(DoubleVector2, average_number_biten_by_location_person)
//
// PROPERTY_REF(DoubleVector, percentage_bites_on_top_20_by_location)
//
// PROPERTY_REF(DoubleVector, cumulative_discounted_NTF_by_location)
//
// PROPERTY_REF(DoubleVector, cumulative_NTF_by_location)
//
// PROPERTY_REF(LongVector, cumulative_TF_by_location)
//
// PROPERTY_REF(LongVector, cumulative_number_treatments_by_location)
//
//
// PROPERTY_REF(IntVector, today_TF_by_location)
//
// PROPERTY_REF(IntVector, today_number_of_treatments_by_location)
//
// PROPERTY_REF(IntVector, today_RITF_by_location)
//
// PROPERTY_REF(IntVector2, total_number_of_treatments_60_by_location)
//
// PROPERTY_REF(IntVector2, total_RITF_60_by_location)
//
// PROPERTY_REF(IntVector2, total_TF_60_by_location)
//
// PROPERTY_REF(DoubleVector, current_RITF_by_location)
//
// PROPERTY_REF(DoubleVector, current_TF_by_location)
//
// PROPERTY_REF(IntVector, cumulative_mutants_by_location)
//
// PROPERTY_REF(int, current_utl_duration)
//
// PROPERTY_REF(IntVector, UTL_duration)
//
// PROPERTY_REF(IntVector, number_of_treatments_with_therapy_ID)
//
// PROPERTY_REF(IntVector, number_of_treatments_success_with_therapy_ID)
//
// PROPERTY_REF(IntVector, number_of_treatments_fail_with_therapy_ID)
//
// PROPERTY_REF(double, AMU_per_parasite_pop)
//
// PROPERTY_REF(double, AMU_per_person)
//
// PROPERTY_REF(double, AMU_for_clinical_caused_parasite)
//
// PROPERTY_REF(double, AFU)
//
// PROPERTY_REF(double, discounted_AMU_per_parasite_pop)
//
// PROPERTY_REF(double, discounted_AMU_per_person)
//
// PROPERTY_REF(double, discounted_AMU_for_clinical_caused_parasite)
//
// PROPERTY_REF(double, discounted_AFU)
//
// PROPERTY_REF(IntVector2, multiple_of_infection_by_location)
//
// PROPERTY_REF(DoubleVector, current_EIR_by_location)
//
// PROPERTY_REF(LongVector, last_update_total_number_of_bites_by_location)
//
// PROPERTY_REF(DoubleVector2, last_10_blood_slide_prevalence_by_location)
//
// PROPERTY_REF(DoubleVector2, last_10_blood_slide_prevalence_by_location_age_class)
//
// PROPERTY_REF(DoubleVector2, last_10_fraction_positive_that_are_clinical_by_location)
//
// PROPERTY_REF(DoubleVector3, last_10_fraction_positive_that_are_clinical_by_location_age_class)
//
// PROPERTY_REF(DoubleVector3, last_10_fraction_positive_that_are_clinical_by_location_age_class_by_5)
//
// PROPERTY_REF(IntVector, total_parasite_population_by_location)
//
// PROPERTY_REF(IntVector, number_of_positive_by_location)
//
// PROPERTY_REF(IntVector2, total_parasite_population_by_location_age_group)
//
// PROPERTY_REF(IntVector2, number_of_positive_by_location_age_group)
//
// PROPERTY_REF(IntVector2, number_of_clinical_by_location_age_group)
//
// PROPERTY_REF(IntVector2, number_of_clinical_by_location_age_group_by_5)
//
// PROPERTY_REF(IntVector2, number_of_death_by_location_age_group)
//
// PROPERTY_REF(IntVector2, number_of_untreated_cases_by_location_age_year)
//
// PROPERTY_REF(IntVector2, number_of_treatments_by_location_age_year)
//
// PROPERTY_REF(IntVector2, number_of_deaths_by_location_age_year)
//
// PROPERTY_REF(IntVector2, number_of_malaria_deaths_treated_by_location_age_year)
//
// PROPERTY_REF(IntVector2, number_of_malaria_deaths_non_treated_by_location_age_year)
//
// PROPERTY_REF(IntVector, monthly_number_of_treatment_by_location);
//
// PROPERTY_REF(IntVector, monthly_number_of_TF_by_location);
//
// PROPERTY_REF(IntVector, monthly_number_of_new_infections_by_location);
//
// PROPERTY_REF(IntVector, monthly_number_of_clinical_episode_by_location);
//
// PROPERTY_REF(IntVector2, monthly_number_of_clinical_episode_by_location_age);
//
// PROPERTY_REF(IntVector, monthly_number_of_mutation_events_by_location);
//
// PROPERTY_REF(IntVector2, popsize_by_location_age)
//
// PROPERTY_REF(double, tf_at_15)
//
// PROPERTY_REF(double, single_resistance_frequency_at_15)
//
// PROPERTY_REF(double, double_resistance_frequency_at_15)
//
// PROPERTY_REF(double, triple_resistance_frequency_at_15)
//
// PROPERTY_REF(double, quadruple_resistance_frequency_at_15)
//
// PROPERTY_REF(double, quintuple_resistance_frequency_at_15)
//
// PROPERTY_REF(double, art_resistance_frequency_at_15)
//
// PROPERTY_REF(double, total_resistance_frequency_at_15)
//
// PROPERTY_REF(IntVector, today_tf_by_therapy)
//
// PROPERTY_REF(IntVector, today_number_of_treatments_by_therapy)
//
// PROPERTY_REF(DoubleVector, current_tf_by_therapy)
//
// PROPERTY_REF(IntVector2, total_number_of_treatments_60_by_therapy)
//
// PROPERTY_REF(IntVector2, total_tf_60_by_therapy)
//
// PROPERTY_REF(double, mean_moi)
//
// PROPERTY_REF(LongVector, number_of_mutation_events_by_year)
//
// PROPERTY_REF(long, current_number_of_mutation_events_in_this_year)
//
// PROPERTY_REF(LongVector2, mosquito_recombination_events_count)

  // Pointer Property
private:
    Model* model_;
public:
    Model* model() const { return model_; }
    void set_model(Model* value) { model_ = value; }

// Property References
private:
    DoubleVector total_immune_by_location_;
public:
    DoubleVector& total_immune_by_location() { return total_immune_by_location_; }
    void set_total_immune_by_location(const DoubleVector& value) { total_immune_by_location_ = value; }

private:
    DoubleVector2 total_immune_by_location_age_class_;
public:
    DoubleVector2& total_immune_by_location_age_class() { return total_immune_by_location_age_class_; }
    void set_total_immune_by_location_age_class(const DoubleVector2& value) { total_immune_by_location_age_class_ = value; }

private:
    IntVector popsize_by_location_;
public:
    IntVector& popsize_by_location() { return popsize_by_location_; }
    void set_popsize_by_location(const IntVector& value) { popsize_by_location_ = value; }

private:
    IntVector popsize_residence_by_location_;
public:
    IntVector& popsize_residence_by_location() { return popsize_residence_by_location_; }
    void set_popsize_residence_by_location(const IntVector& value) { popsize_residence_by_location_ = value; }

private:
    IntVector2 popsize_by_location_age_class_;
public:
    IntVector2& popsize_by_location_age_class() { return popsize_by_location_age_class_; }
    void set_popsize_by_location_age_class(const IntVector2& value) { popsize_by_location_age_class_ = value; }

private:
    IntVector2 popsize_by_location_age_class_by_5_;
public:
    IntVector2& popsize_by_location_age_class_by_5() { return popsize_by_location_age_class_by_5_; }
    void set_popsize_by_location_age_class_by_5(const IntVector2& value) { popsize_by_location_age_class_by_5_ = value; }

private:
    IntVector2 popsize_by_location_hoststate_;
public:
    IntVector2& popsize_by_location_hoststate() { return popsize_by_location_hoststate_; }
    void set_popsize_by_location_hoststate(const IntVector2& value) { popsize_by_location_hoststate_ = value; }

private:
    IntVector3 popsize_by_location_hoststate_age_class_;
public:
    IntVector3& popsize_by_location_hoststate_age_class() { return popsize_by_location_hoststate_age_class_; }
    void set_popsize_by_location_hoststate_age_class(const IntVector3& value) { popsize_by_location_hoststate_age_class_ = value; }

private:
    DoubleVector blood_slide_prevalence_by_location_;
public:
    DoubleVector& blood_slide_prevalence_by_location() { return blood_slide_prevalence_by_location_; }
    void set_blood_slide_prevalence_by_location(const DoubleVector& value) { blood_slide_prevalence_by_location_ = value; }

private:
    DoubleVector2 blood_slide_number_by_location_age_group_;
public:
    DoubleVector2& blood_slide_number_by_location_age_group() { return blood_slide_number_by_location_age_group_; }
    void set_blood_slide_number_by_location_age_group(const DoubleVector2& value) { blood_slide_number_by_location_age_group_ = value; }

private:
    DoubleVector2 blood_slide_prevalence_by_location_age_group_;
public:
    DoubleVector2& blood_slide_prevalence_by_location_age_group() { return blood_slide_prevalence_by_location_age_group_; }
    void set_blood_slide_prevalence_by_location_age_group(const DoubleVector2& value) { blood_slide_prevalence_by_location_age_group_ = value; }

private:
    DoubleVector2 blood_slide_number_by_location_age_group_by_5_;
public:
    DoubleVector2& blood_slide_number_by_location_age_group_by_5() { return blood_slide_number_by_location_age_group_by_5_; }
    void set_blood_slide_number_by_location_age_group_by_5(const DoubleVector2& value) { blood_slide_number_by_location_age_group_by_5_ = value; }

private:
    DoubleVector2 blood_slide_prevalence_by_location_age_group_by_5_;
public:
    DoubleVector2& blood_slide_prevalence_by_location_age_group_by_5() { return blood_slide_prevalence_by_location_age_group_by_5_; }
    void set_blood_slide_prevalence_by_location_age_group_by_5(const DoubleVector2& value) { blood_slide_prevalence_by_location_age_group_by_5_ = value; }

private:
    DoubleVector2 blood_slide_prevalence_by_location_age_;
public:
    DoubleVector2& blood_slide_prevalence_by_location_age() { return blood_slide_prevalence_by_location_age_; }
    void set_blood_slide_prevalence_by_location_age(const DoubleVector2& value) { blood_slide_prevalence_by_location_age_ = value; }

private:
    DoubleVector2 blood_slide_number_by_location_age_;
public:
    DoubleVector2& blood_slide_number_by_location_age() { return blood_slide_number_by_location_age_; }
    void set_blood_slide_number_by_location_age(const DoubleVector2& value) { blood_slide_number_by_location_age_ = value; }

private:
    DoubleVector fraction_of_positive_that_are_clinical_by_location_;
public:
    DoubleVector& fraction_of_positive_that_are_clinical_by_location() { return fraction_of_positive_that_are_clinical_by_location_; }
    void set_fraction_of_positive_that_are_clinical_by_location(const DoubleVector& value) { fraction_of_positive_that_are_clinical_by_location_ = value; }

private:
    LongVector total_number_of_bites_by_location_;
public:
    LongVector& total_number_of_bites_by_location() { return total_number_of_bites_by_location_; }
    void set_total_number_of_bites_by_location(const LongVector& value) { total_number_of_bites_by_location_ = value; }

private:
    LongVector total_number_of_bites_by_location_year_;
public:
    LongVector& total_number_of_bites_by_location_year() { return total_number_of_bites_by_location_year_; }
    void set_total_number_of_bites_by_location_year(const LongVector& value) { total_number_of_bites_by_location_year_ = value; }

private:
    LongVector person_days_by_location_year_;
public:
    LongVector& person_days_by_location_year() { return person_days_by_location_year_; }
    void set_person_days_by_location_year(const LongVector& value) { person_days_by_location_year_ = value; }

private:
    DoubleVector2 EIR_by_location_year_;
public:
    DoubleVector2& EIR_by_location_year() { return EIR_by_location_year_; }
    void set_EIR_by_location_year(const DoubleVector2& value) { EIR_by_location_year_ = value; }

private:
    DoubleVector EIR_by_location_;
public:
    DoubleVector& EIR_by_location() { return EIR_by_location_; }
    void set_EIR_by_location(const DoubleVector& value) { EIR_by_location_ = value; }

private:
    LongVector cumulative_clinical_episodes_by_location_;
public:
    LongVector& cumulative_clinical_episodes_by_location() { return cumulative_clinical_episodes_by_location_; }
    void set_cumulative_clinical_episodes_by_location(const LongVector& value) { cumulative_clinical_episodes_by_location_ = value; }

private:
    LongVector2 cumulative_clinical_episodes_by_location_age_;
public:
    LongVector2& cumulative_clinical_episodes_by_location_age() { return cumulative_clinical_episodes_by_location_age_; }
    void set_cumulative_clinical_episodes_by_location_age(const LongVector2& value) { cumulative_clinical_episodes_by_location_age_ = value; }

private:
    LongVector2 cumulative_clinical_episodes_by_location_age_group_;
public:
    LongVector2& cumulative_clinical_episodes_by_location_age_group() { return cumulative_clinical_episodes_by_location_age_group_; }
    void set_cumulative_clinical_episodes_by_location_age_group(const LongVector2& value) { cumulative_clinical_episodes_by_location_age_group_ = value; }

private:
    DoubleVector2 average_number_biten_by_location_person_;
public:
    DoubleVector2& average_number_biten_by_location_person() { return average_number_biten_by_location_person_; }
    void set_average_number_biten_by_location_person(const DoubleVector2& value) { average_number_biten_by_location_person_ = value; }

private:
    DoubleVector percentage_bites_on_top_20_by_location_;
public:
    DoubleVector& percentage_bites_on_top_20_by_location() { return percentage_bites_on_top_20_by_location_; }
    void set_percentage_bites_on_top_20_by_location(const DoubleVector& value) { percentage_bites_on_top_20_by_location_ = value; }

private:
    DoubleVector cumulative_discounted_NTF_by_location_;
public:
    DoubleVector& cumulative_discounted_NTF_by_location() { return cumulative_discounted_NTF_by_location_; }
    void set_cumulative_discounted_NTF_by_location(const DoubleVector& value) { cumulative_discounted_NTF_by_location_ = value; }

private:
    DoubleVector cumulative_NTF_by_location_;
public:
    DoubleVector& cumulative_NTF_by_location() { return cumulative_NTF_by_location_; }
    void set_cumulative_NTF_by_location(const DoubleVector& value) { cumulative_NTF_by_location_ = value; }

private:
    LongVector cumulative_TF_by_location_;
public:
    LongVector& cumulative_TF_by_location() { return cumulative_TF_by_location_; }
    void set_cumulative_TF_by_location(const LongVector& value) { cumulative_TF_by_location_ = value; }

private:
    LongVector cumulative_number_treatments_by_location_;
public:
    LongVector& cumulative_number_treatments_by_location() { return cumulative_number_treatments_by_location_; }
    void set_cumulative_number_treatments_by_location(const LongVector& value) { cumulative_number_treatments_by_location_ = value; }

private:
    IntVector today_TF_by_location_;
public:
    IntVector& today_TF_by_location() { return today_TF_by_location_; }
    void set_today_TF_by_location(const IntVector& value) { today_TF_by_location_ = value; }

private:
    IntVector today_number_of_treatments_by_location_;
public:
    IntVector& today_number_of_treatments_by_location() { return today_number_of_treatments_by_location_; }
    void set_today_number_of_treatments_by_location(const IntVector& value) { today_number_of_treatments_by_location_ = value; }

private:
    IntVector today_RITF_by_location_;
public:
    IntVector& today_RITF_by_location() { return today_RITF_by_location_; }
    void set_today_RITF_by_location(const IntVector& value) { today_RITF_by_location_ = value; }

private:
    IntVector2 total_number_of_treatments_60_by_location_;
public:
    IntVector2& total_number_of_treatments_60_by_location() { return total_number_of_treatments_60_by_location_; }
    void set_total_number_of_treatments_60_by_location(const IntVector2& value) { total_number_of_treatments_60_by_location_ = value; }

private:
    IntVector2 total_RITF_60_by_location_;
public:
    IntVector2& total_RITF_60_by_location() { return total_RITF_60_by_location_; }
    void set_total_RITF_60_by_location(const IntVector2& value) { total_RITF_60_by_location_ = value; }

private:
    IntVector2 total_TF_60_by_location_;
public:
    IntVector2& total_TF_60_by_location() { return total_TF_60_by_location_; }
    void set_total_TF_60_by_location(const IntVector2& value) { total_TF_60_by_location_ = value; }

private:
    DoubleVector current_RITF_by_location_;
public:
    DoubleVector& current_RITF_by_location() { return current_RITF_by_location_; }
    void set_current_RITF_by_location(const DoubleVector& value) { current_RITF_by_location_ = value; }

private:
    DoubleVector current_TF_by_location_;
public:
    DoubleVector& current_TF_by_location() { return current_TF_by_location_; }
    void set_current_TF_by_location(const DoubleVector& value) { current_TF_by_location_ = value; }

private:
    IntVector cumulative_mutants_by_location_;
public:
    IntVector& cumulative_mutants_by_location() { return cumulative_mutants_by_location_; }
    void set_cumulative_mutants_by_location(const IntVector& value) { cumulative_mutants_by_location_ = value; }

private:
    int current_utl_duration_;
public:
    int current_utl_duration() const { return current_utl_duration_; }
    void set_current_utl_duration(int value) { current_utl_duration_ = value; }

private:
    IntVector UTL_duration_;
public:
    IntVector& UTL_duration() { return UTL_duration_; }
    void set_UTL_duration(const IntVector& value) { UTL_duration_ = value; }

private:
    IntVector number_of_treatments_with_therapy_ID_;
public:
    IntVector& number_of_treatments_with_therapy_ID() { return number_of_treatments_with_therapy_ID_; }
    void set_number_of_treatments_with_therapy_ID(const IntVector& value) { number_of_treatments_with_therapy_ID_ = value; }

private:
    IntVector number_of_treatments_success_with_therapy_ID_;
public:
    IntVector& number_of_treatments_success_with_therapy_ID() { return number_of_treatments_success_with_therapy_ID_; }
    void set_number_of_treatments_success_with_therapy_ID(const IntVector& value) { number_of_treatments_success_with_therapy_ID_ = value; }

private:
    IntVector number_of_treatments_fail_with_therapy_ID_;
public:
    IntVector& number_of_treatments_fail_with_therapy_ID() { return number_of_treatments_fail_with_therapy_ID_; }
    void set_number_of_treatments_fail_with_therapy_ID(const IntVector& value) { number_of_treatments_fail_with_therapy_ID_ = value; }

private:
    double AMU_per_parasite_pop_;
public:
    double AMU_per_parasite_pop() const { return AMU_per_parasite_pop_; }
    void set_AMU_per_parasite_pop(double value) { AMU_per_parasite_pop_ = value; }

private:
    double AMU_per_person_;
public:
    double AMU_per_person() const { return AMU_per_person_; }
    void set_AMU_per_person(double value) { AMU_per_person_ = value; }

private:
    double AMU_for_clinical_caused_parasite_;
public:
    double AMU_for_clinical_caused_parasite() const { return AMU_for_clinical_caused_parasite_; }
    void set_AMU_for_clinical_caused_parasite(double value) { AMU_for_clinical_caused_parasite_ = value; }

private:
    double AFU_;
public:
    double AFU() const { return AFU_; }
    void set_AFU(double value) { AFU_ = value; }

private:
    double discounted_AMU_per_parasite_pop_;
public:
    double discounted_AMU_per_parasite_pop() const { return discounted_AMU_per_parasite_pop_; }
    void set_discounted_AMU_per_parasite_pop(double value) { discounted_AMU_per_parasite_pop_ = value; }

private:
    double discounted_AMU_per_person_;
public:
    double discounted_AMU_per_person() const { return discounted_AMU_per_person_; }
    void set_discounted_AMU_per_person(double value) { discounted_AMU_per_person_ = value; }

private:
    double discounted_AMU_for_clinical_caused_parasite_;
public:
    double discounted_AMU_for_clinical_caused_parasite() const { return discounted_AMU_for_clinical_caused_parasite_; }
    void set_discounted_AMU_for_clinical_caused_parasite(double value) { discounted_AMU_for_clinical_caused_parasite_ = value; }

private:
    double discounted_AFU_;
public:
    double discounted_AFU() const { return discounted_AFU_; }
    void set_discounted_AFU(double value) { discounted_AFU_ = value; }

private:
    IntVector2 multiple_of_infection_by_location_;
public:
    IntVector2& multiple_of_infection_by_location() { return multiple_of_infection_by_location_; }
    void set_multiple_of_infection_by_location(const IntVector2& value) { multiple_of_infection_by_location_ = value; }

private:
    DoubleVector current_EIR_by_location_;
public:
    DoubleVector& current_EIR_by_location() { return current_EIR_by_location_; }
    void set_current_EIR_by_location(const DoubleVector& value) { current_EIR_by_location_ = value; }

private:
    LongVector last_update_total_number_of_bites_by_location_;
public:
    LongVector& last_update_total_number_of_bites_by_location() { return last_update_total_number_of_bites_by_location_; }
    void set_last_update_total_number_of_bites_by_location(const LongVector& value) { last_update_total_number_of_bites_by_location_ = value; }

private:
    DoubleVector2 last_10_blood_slide_prevalence_by_location_;
public:
    DoubleVector2& last_10_blood_slide_prevalence_by_location() { return last_10_blood_slide_prevalence_by_location_; }
    void set_last_10_blood_slide_prevalence_by_location(const DoubleVector2& value) { last_10_blood_slide_prevalence_by_location_ = value; }

private:
    DoubleVector2 last_10_blood_slide_prevalence_by_location_age_class_;
public:
    DoubleVector2& last_10_blood_slide_prevalence_by_location_age_class() { return last_10_blood_slide_prevalence_by_location_age_class_; }
    void set_last_10_blood_slide_prevalence_by_location_age_class(const DoubleVector2& value) { last_10_blood_slide_prevalence_by_location_age_class_ = value; }

private:
    DoubleVector2 last_10_fraction_positive_that_are_clinical_by_location_;
public:
    DoubleVector2& last_10_fraction_positive_that_are_clinical_by_location() { return last_10_fraction_positive_that_are_clinical_by_location_; }
    void set_last_10_fraction_positive_that_are_clinical_by_location(const DoubleVector2& value) { last_10_fraction_positive_that_are_clinical_by_location_ = value; }

private:
    DoubleVector3 last_10_fraction_positive_that_are_clinical_by_location_age_class_;
public:
    DoubleVector3& last_10_fraction_positive_that_are_clinical_by_location_age_class() { return last_10_fraction_positive_that_are_clinical_by_location_age_class_; }
    void set_last_10_fraction_positive_that_are_clinical_by_location_age_class(const DoubleVector3& value) { last_10_fraction_positive_that_are_clinical_by_location_age_class_ = value; }

private:
    DoubleVector3 last_10_fraction_positive_that_are_clinical_by_location_age_class_by_5_;
public:
    DoubleVector3& last_10_fraction_positive_that_are_clinical_by_location_age_class_by_5() { return last_10_fraction_positive_that_are_clinical_by_location_age_class_by_5_; }
    void set_last_10_fraction_positive_that_are_clinical_by_location_age_class_by_5(const DoubleVector3& value) { last_10_fraction_positive_that_are_clinical_by_location_age_class_by_5_ = value; }

private:
    IntVector total_parasite_population_by_location_;
public:
    IntVector& total_parasite_population_by_location() { return total_parasite_population_by_location_; }
    void set_total_parasite_population_by_location(const IntVector& value) { total_parasite_population_by_location_ = value; }

private:
    IntVector number_of_positive_by_location_;
public:
    IntVector& number_of_positive_by_location() { return number_of_positive_by_location_; }
    void set_number_of_positive_by_location(const IntVector& value) { number_of_positive_by_location_ = value; }

private:
    IntVector2 total_parasite_population_by_location_age_group_;
public:
    IntVector2& total_parasite_population_by_location_age_group() { return total_parasite_population_by_location_age_group_; }
    void set_total_parasite_population_by_location_age_group(const IntVector2& value) { total_parasite_population_by_location_age_group_ = value; }

private:
    IntVector2 number_of_positive_by_location_age_group_;
public:
    IntVector2& number_of_positive_by_location_age_group() { return number_of_positive_by_location_age_group_; }
    void set_number_of_positive_by_location_age_group(const IntVector2& value) { number_of_positive_by_location_age_group_ = value; }

private:
    IntVector2 number_of_clinical_by_location_age_group_;
public:
    IntVector2& number_of_clinical_by_location_age_group() { return number_of_clinical_by_location_age_group_; }
    void set_number_of_clinical_by_location_age_group(const IntVector2& value) { number_of_clinical_by_location_age_group_ = value; }

private:
    IntVector2 number_of_clinical_by_location_age_group_by_5_;
public:
    IntVector2& number_of_clinical_by_location_age_group_by_5() { return number_of_clinical_by_location_age_group_by_5_; }
    void set_number_of_clinical_by_location_age_group_by_5(const IntVector2& value) { number_of_clinical_by_location_age_group_by_5_ = value; }

private:
    IntVector2 number_of_death_by_location_age_group_;
public:
    IntVector2& number_of_death_by_location_age_group() { return number_of_death_by_location_age_group_; }
    void set_number_of_death_by_location_age_group(const IntVector2& value) { number_of_death_by_location_age_group_ = value; }

private:
    IntVector2 number_of_untreated_cases_by_location_age_year_;
public:
    IntVector2& number_of_untreated_cases_by_location_age_year() { return number_of_untreated_cases_by_location_age_year_; }
    void set_number_of_untreated_cases_by_location_age_year(const IntVector2& value) { number_of_untreated_cases_by_location_age_year_ = value; }

private:
    IntVector2 number_of_treatments_by_location_age_year_;
public:
    IntVector2& number_of_treatments_by_location_age_year() { return number_of_treatments_by_location_age_year_; }
    void set_number_of_treatments_by_location_age_year(const IntVector2& value) { number_of_treatments_by_location_age_year_ = value; }

private:
    IntVector2 number_of_deaths_by_location_age_year_;
public:
    IntVector2& number_of_deaths_by_location_age_year() { return number_of_deaths_by_location_age_year_; }
    void set_number_of_deaths_by_location_age_year(const IntVector2& value) { number_of_deaths_by_location_age_year_ = value; }

private:
    IntVector2 number_of_malaria_deaths_treated_by_location_age_year_;
public:
    IntVector2& number_of_malaria_deaths_treated_by_location_age_year() { return number_of_malaria_deaths_treated_by_location_age_year_; }
    void set_number_of_malaria_deaths_treated_by_location_age_year(const IntVector2& value) { number_of_malaria_deaths_treated_by_location_age_year_ = value; }

private:
    IntVector2 number_of_malaria_deaths_non_treated_by_location_age_year_;
public:
    IntVector2& number_of_malaria_deaths_non_treated_by_location_age_year() { return number_of_malaria_deaths_non_treated_by_location_age_year_; }
    void set_number_of_malaria_deaths_non_treated_by_location_age_year(const IntVector2& value) { number_of_malaria_deaths_non_treated_by_location_age_year_ = value; }

private:
    IntVector monthly_number_of_treatment_by_location_;
public:
    IntVector& monthly_number_of_treatment_by_location() { return monthly_number_of_treatment_by_location_; }
    void set_monthly_number_of_treatment_by_location(const IntVector& value) { monthly_number_of_treatment_by_location_ = value; }

private:
    IntVector monthly_number_of_TF_by_location_;
public:
    IntVector& monthly_number_of_TF_by_location() { return monthly_number_of_TF_by_location_; }
    void set_monthly_number_of_TF_by_location(const IntVector& value) { monthly_number_of_TF_by_location_ = value; }

private:
    IntVector monthly_number_of_new_infections_by_location_;
public:
    IntVector& monthly_number_of_new_infections_by_location() { return monthly_number_of_new_infections_by_location_; }
    void set_monthly_number_of_new_infections_by_location(const IntVector& value) { monthly_number_of_new_infections_by_location_ = value; }

private:
    IntVector monthly_number_of_recrudescence_treatment_by_location_;
public:
    IntVector& monthly_number_of_recrudescence_treatment_by_location() { return monthly_number_of_recrudescence_treatment_by_location_; }
    void set_monthly_number_of_recrudescence_treatment_by_location(const IntVector& value) { monthly_number_of_recrudescence_treatment_by_location_ = value; }

private:
    IntVector monthly_number_of_clinical_episode_by_location_;
public:
    IntVector& monthly_number_of_clinical_episode_by_location() { return monthly_number_of_clinical_episode_by_location_; }
    void set_monthly_number_of_clinical_episode_by_location(const IntVector& value) { monthly_number_of_clinical_episode_by_location_ = value; }

private:
    IntVector2 monthly_number_of_clinical_episode_by_location_age_;
public:
    IntVector2& monthly_number_of_clinical_episode_by_location_age() { return monthly_number_of_clinical_episode_by_location_age_; }
    void set_monthly_number_of_clinical_episode_by_location_age(const IntVector2& value) { monthly_number_of_clinical_episode_by_location_age_ = value; }

private:
    IntVector monthly_number_of_mutation_events_by_location_;
public:
    IntVector& monthly_number_of_mutation_events_by_location() { return monthly_number_of_mutation_events_by_location_; }
    void set_monthly_number_of_mutation_events_by_location(const IntVector& value) { monthly_number_of_mutation_events_by_location_ = value; }

private:
    IntVector2 popsize_by_location_age_;
public:
    IntVector2& popsize_by_location_age() { return popsize_by_location_age_; }
    void set_popsize_by_location_age(const IntVector2& value) { popsize_by_location_age_ = value; }

private:
    double tf_at_15_;
public:
    double tf_at_15() const { return tf_at_15_; }
    void set_tf_at_15(double value) { tf_at_15_ = value; }

private:
    double single_resistance_frequency_at_15_;
public:
    double single_resistance_frequency_at_15() const { return single_resistance_frequency_at_15_; }
    void set_single_resistance_frequency_at_15(double value) { single_resistance_frequency_at_15_ = value; }
private:
    double double_resistance_frequency_at_15_;
public:
    double double_resistance_frequency_at_15() const { return double_resistance_frequency_at_15_; }
    void set_double_resistance_frequency_at_15(double value) { double_resistance_frequency_at_15_ = value; }

private:
    double triple_resistance_frequency_at_15_;
public:
    double triple_resistance_frequency_at_15() const { return triple_resistance_frequency_at_15_; }
    void set_triple_resistance_frequency_at_15(double value) { triple_resistance_frequency_at_15_ = value; }

private:
    double quadruple_resistance_frequency_at_15_;
public:
    double quadruple_resistance_frequency_at_15() const { return quadruple_resistance_frequency_at_15_; }
    void set_quadruple_resistance_frequency_at_15(double value) { quadruple_resistance_frequency_at_15_ = value; }

private:
    double quintuple_resistance_frequency_at_15_;
public:
    double quintuple_resistance_frequency_at_15() const { return quintuple_resistance_frequency_at_15_; }
    void set_quintuple_resistance_frequency_at_15(double value) { quintuple_resistance_frequency_at_15_ = value; }

private:
    double art_resistance_frequency_at_15_;
public:
    double art_resistance_frequency_at_15() const { return art_resistance_frequency_at_15_; }
    void set_art_resistance_frequency_at_15(double value) { art_resistance_frequency_at_15_ = value; }

private:
    double total_resistance_frequency_at_15_;
public:
    double total_resistance_frequency_at_15() const { return total_resistance_frequency_at_15_; }
    void set_total_resistance_frequency_at_15(double value) { total_resistance_frequency_at_15_ = value; }

private:
    IntVector today_tf_by_therapy_;
public:
    IntVector& today_tf_by_therapy() { return today_tf_by_therapy_; }
    void set_today_tf_by_therapy(const IntVector& value) { today_tf_by_therapy_ = value; }

private:
    IntVector today_number_of_treatments_by_therapy_;
public:
    IntVector& today_number_of_treatments_by_therapy() { return today_number_of_treatments_by_therapy_; }
    void set_today_number_of_treatments_by_therapy(const IntVector& value) { today_number_of_treatments_by_therapy_ = value; }

private:
    DoubleVector current_tf_by_therapy_;
public:
    DoubleVector& current_tf_by_therapy() { return current_tf_by_therapy_; }
    void set_current_tf_by_therapy(const DoubleVector& value) { current_tf_by_therapy_ = value; }

private:
    IntVector2 total_number_of_treatments_60_by_therapy_;
public:
    IntVector2& total_number_of_treatments_60_by_therapy() { return total_number_of_treatments_60_by_therapy_; }
    void set_total_number_of_treatments_60_by_therapy(const IntVector2& value) { total_number_of_treatments_60_by_therapy_ = value; }

private:
    IntVector2 total_tf_60_by_therapy_;
public:
    IntVector2& total_tf_60_by_therapy() { return total_tf_60_by_therapy_; }
    void set_total_tf_60_by_therapy(const IntVector2& value) { total_tf_60_by_therapy_ = value; }

private:
    double mean_moi_;
public:
    double mean_moi() const { return mean_moi_; }
    void set_mean_moi(double value) { mean_moi_ = value; }

private:
    LongVector number_of_mutation_events_by_year_;
public:
    LongVector& number_of_mutation_events_by_year() { return number_of_mutation_events_by_year_; }
    void set_number_of_mutation_events_by_year(const LongVector& value) { number_of_mutation_events_by_year_ = value; }

private:
    long current_number_of_mutation_events_in_this_year_;
public:
    long current_number_of_mutation_events_in_this_year() const { return current_number_of_mutation_events_in_this_year_; }
    void set_current_number_of_mutation_events_in_this_year(long value) { current_number_of_mutation_events_in_this_year_ = value; }

private:
    LongVector2 mosquito_recombination_events_count_;
public:
    LongVector2& mosquito_recombination_events_count() { return mosquito_recombination_events_count_; }
    void set_mosquito_recombination_events_count(const LongVector2& value) { mosquito_recombination_events_count_ = value; }

static const int number_of_reported_MOI = 10;

typedef std::tuple<int, int, int, int, int, int> mutation_tracker_info;
std::vector<std::vector<mutation_tracker_info>> mutation_tracker;

//typedef std::tuple<int, int, int, int, int, int, int, int, int, int, int> recombined_resistant_genotype_info;
//std::vector<std::vector<recombined_resistant_genotype_info>> mosquito_recombined_resistant_genotype_tracker;

typedef std::tuple<int, int, int, int> recombined_resistant_genotype_info;
std::vector<std::vector<recombined_resistant_genotype_info>> mosquito_recombined_resistant_genotype_tracker;

public:
  explicit ModelDataCollector(Model* model = nullptr);

  //    Statistic(const Statistic& orig);
  virtual ~ModelDataCollector();

  void initialize();

  void perform_population_statistic();

  void monthly_update();

  virtual void collect_number_of_bites(const int& location, const int& number_of_bites);

  virtual void collect_1_clinical_episode(const int& location, const int& age, const int& age_class);

  virtual void update_person_days_by_years(const int& location, const int& days);

  void calculate_eir();

  void
  record_1_death(
      const int& location, const int& birthday, const int& number_of_times_bitten, const int& age_group,
      const int& age
  );

  void record_1_malaria_death(const int& location, const int& age, bool treated);

  void calculate_percentage_bites_on_top_20();

  void record_1_TF(const int& location, const bool& by_drug);

  void record_1_treatment(const int& location, const int& age, const int& age_class, const int& therapy_id);

  // Record that one treatment has been given
  void record_1_recrudescence_treatment(const int &location,
                                        const int &age,
                                        const int &age_class,
                                        const int &therapy_id);


  void record_1_non_treated_case(const int& location,  const int& age, const int& age_class);

  void record_1_mutation(const int& location, Genotype* from, Genotype* to);

  void record_1_mutation_by_drug(const int& location, Genotype* from, Genotype* to, int drug_id);

  void begin_time_step();

  void end_of_time_step();

  void update_UTL_vector();

  //    void collect_1_non_resistant_treatment(const int& therapy_id);
  void record_1_treatment_failure_by_therapy(const int& location, const int& age, const int& therapy_id);
  void record_1_treatment_success_by_therapy(const int& location, const int& age, const int& therapy_id);

  void update_after_run();


  void record_AMU_AFU(Person* person, Therapy* therapy, ClonalParasitePopulation* clinical_caused_parasite);

  double get_blood_slide_prevalence(const int& location, const int& age_from, const int& age_to);

  [[nodiscard]] bool recording_data() const { return recording; }

  /*
   * From Temple Malaria Simulation MainDataCollector
   */
  void zero_population_statistics();
  void record_1_malaria_death(const int &location,const int &age_class);
  void record_1_infection(const int &location);
  void record_1_death(const int &location, const int &birthday,
                                         const int &number_of_times_bitten,
                                         const int &age_group);
  void record_1_birth(const int &location);
  void yearly_update();
  void collect_1_clinical_episode(const int &location,
                                                   const int &age_class);

private:
  IntVector monthly_treatment_failure_by_location_;
public:
  [[nodiscard]] IntVector& monthly_treatment_failure_by_location() { return monthly_treatment_failure_by_location_; }
  void set_monthly_treatment_failure_by_location(const IntVector& value) { monthly_treatment_failure_by_location_ = value; }

private:
  IntVector monthly_nontreatment_by_location_;
public:
  [[nodiscard]] IntVector& monthly_nontreatment_by_location() { return monthly_nontreatment_by_location_; }
  void set_monthly_nontreatment_by_location(const IntVector& value) { monthly_nontreatment_by_location_ = value; }

private:
  IntVector2 monthly_number_of_treatment_by_location_age_class_;
public:
  [[nodiscard]] IntVector2& monthly_number_of_treatment_by_location_age_class() { return monthly_number_of_treatment_by_location_age_class_; }
    void set_monthly_number_of_treatment_by_location_age_class(const IntVector2& value) { monthly_number_of_treatment_by_location_age_class_ = value; }

private:
  IntVector2 monthly_number_of_clinical_episode_by_location_age_class_;
public:
  [[nodiscard]] IntVector2& monthly_number_of_clinical_episode_by_location_age_class() { return monthly_number_of_clinical_episode_by_location_age_class_; }
    void set_monthly_number_of_clinical_episode_by_location_age_class(const IntVector2& value) { monthly_number_of_clinical_episode_by_location_age_class_ = value; }
private:
  IntVector births_by_location_;
public:
  [[nodiscard]] IntVector& births_by_location() { return births_by_location_; }
    void set_births_by_location(const IntVector& value) { births_by_location_ = value; }

private:
  IntVector deaths_by_location_;
public:
  [[nodiscard]] IntVector& deaths_by_location() { return deaths_by_location_; }
    void set_deaths_by_location(const IntVector& value) { deaths_by_location_ = value; }

private:
  IntVector malaria_deaths_by_location_;
public:
  [[nodiscard]] IntVector& malaria_deaths_by_location() { return malaria_deaths_by_location_; }
    void set_malaria_deaths_by_location(const IntVector& value) { malaria_deaths_by_location_ = value; }

private:
  IntVector monthly_treatment_success_by_location_;
public:
  [[nodiscard]] IntVector& monthly_treatment_success_by_location() { return monthly_treatment_success_by_location_; }
    void set_monthly_treatment_success_by_location(const IntVector& value) { monthly_treatment_success_by_location_ = value; }

private:
  IntVector2 monthly_nontreatment_by_location_age_class_;
public:
  [[nodiscard]] IntVector2& monthly_nontreatment_by_location_age_class() { return monthly_nontreatment_by_location_age_class_; }
    void set_monthly_nontreatment_by_location_age_class(const IntVector2& value) { monthly_nontreatment_by_location_age_class_ = value; }

private:
  IntVector2 malaria_deaths_by_location_age_class_;
public:
  [[nodiscard]] IntVector2& malaria_deaths_by_location_age_class() { return malaria_deaths_by_location_age_class_; }
    void set_malaria_deaths_by_location_age_class(const IntVector2& value) { malaria_deaths_by_location_age_class_ = value; }

private:
  IntVector2 monthly_number_of_treatment_by_location_therapy_;
public:
  [[nodiscard]] IntVector2& monthly_number_of_treatment_by_location_therapy() { return monthly_number_of_treatment_by_location_therapy_; }
    void set_monthly_number_of_treatment_by_location_therapy(const IntVector2& value) { monthly_number_of_treatment_by_location_therapy_ = value; }

private:
  IntVector2 monthly_treatment_complete_by_location_therapy_;
public:
  [[nodiscard]] IntVector2& monthly_treatment_complete_by_location_therapy() { return monthly_treatment_complete_by_location_therapy_; }
    void set_monthly_treatment_complete_by_location_therapy(const IntVector2& value) { monthly_treatment_complete_by_location_therapy_ = value; }

private:
  IntVector2 monthly_treatment_failure_by_location_age_class_;
public:
  [[nodiscard]] IntVector2& monthly_treatment_failure_by_location_age_class() { return monthly_treatment_failure_by_location_age_class_; }
    void set_monthly_treatment_failure_by_location_age_class(const IntVector2& value) { monthly_treatment_failure_by_location_age_class_ = value; }

private:
  IntVector2 monthly_treatment_failure_by_location_therapy_;
public:
  [[nodiscard]] IntVector2& monthly_treatment_failure_by_location_therapy() { return monthly_treatment_failure_by_location_therapy_; }
    void set_monthly_treatment_failure_by_location_therapy(const IntVector2& value) { monthly_treatment_failure_by_location_therapy_ = value; }

private:
  IntVector2 monthly_treatment_success_by_location_age_class_;
public:
  [[nodiscard]] IntVector2& monthly_treatment_success_by_location_age_class() { return monthly_treatment_success_by_location_age_class_; }
    void set_monthly_treatment_success_by_location_age_class(const IntVector2& value) { monthly_treatment_success_by_location_age_class_ = value; }

private:
  IntVector2 monthly_treatment_success_by_location_therapy_;
public:
  [[nodiscard]] IntVector2& monthly_treatment_success_by_location_therapy() { return monthly_treatment_success_by_location_therapy_; }
    void set_monthly_treatment_success_by_location_therapy(const IntVector2& value) { monthly_treatment_success_by_location_therapy_ = value; }

private:
  long current_number_of_mutation_events_;
public:
  [[nodiscard]] long current_number_of_mutation_events() const { return current_number_of_mutation_events_; }
  void set_current_number_of_mutation_events(const long& value) { current_number_of_mutation_events_ = value; }

private:
  bool recording = false;
  void update_average_number_bitten(const int& location, const int& birthday, const int& number_of_times_bitten);


};

#endif /* MODELDATACOLLECTOR_H */

