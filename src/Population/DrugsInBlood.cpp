#include "DrugsInBlood.h"
#include "Treatment/Therapies/Drug.h"
#include "Events/Event.h"
#include "Treatment/Therapies/DrugType.h"
#include "Population/Person/Person.h"
#include "Utils/Helpers/ObjectHelpers.h"
#include "Utils/TypeDef.hxx"

#ifndef DRUG_CUT_OFF_VALUE
#define DRUG_CUT_OFF_VALUE 0.1
#endif

DrugsInBlood::DrugsInBlood(Person *person) : person_(person), drugs_{} {}

void DrugsInBlood::init() {
  drugs_.clear();
}

DrugsInBlood::~DrugsInBlood() {
  if (!drugs_.empty()) {
    clear();
  }
}

Drug *DrugsInBlood::add_drug(Drug *drug) {
  int typeID = drug->drug_type()->id();
  if (!contains(typeID)) {
    drug->set_person_drugs(this);
    drugs_.insert(std::pair<int, std::unique_ptr<Drug>>(typeID, std::unique_ptr<Drug>(drug)));
  } else {
    //already have it
    Drug* existing_drug = at(typeID);
    existing_drug->set_starting_value(drug->starting_value());
    existing_drug->set_dosing_days(drug->dosing_days());
    existing_drug->set_last_update_value(drug->last_update_value());
    existing_drug->set_last_update_time(drug->last_update_time());
    existing_drug->set_start_time(drug->start_time());
    existing_drug->set_end_time(drug->end_time());

    delete drug;
  }

  return at(typeID);
}

std::size_t DrugsInBlood::size() const {
  return drugs_.size();
}

void DrugsInBlood::clear() {
  if (drugs_.empty()) return;
  drugs_.clear();
}

void DrugsInBlood::update() {
  for (auto& drug : *this) {
    drug.second->update();
  }
}

void DrugsInBlood::clear_cut_off_drugs() {
  if (!drugs_.empty()) {
    for (auto pos = drugs_.begin(); pos != drugs_.end();) {
      if (pos->second->last_update_value() <= DRUG_CUT_OFF_VALUE) {
        drugs_.erase(pos++);
      } else {
        ++pos;
      }
    }
  }
}
