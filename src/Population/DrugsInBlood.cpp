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
  if (!is_drug_in_blood(typeID)) {
    drug->set_person_drugs(this);
    drugs_.insert(std::pair<int, Drug *>(typeID, drug));

  } else {
    //already have it
    drugs_.at(typeID)->set_starting_value(drug->starting_value());
    drugs_.at(typeID)->set_dosing_days(drug->dosing_days());
    drugs_.at(typeID)->set_last_update_value(drug->last_update_value());
    drugs_.at(typeID)->set_last_update_time(drug->last_update_time());
    drugs_.at(typeID)->set_start_time(drug->start_time());
    drugs_.at(typeID)->set_end_time(drug->end_time());

    delete drug;
  }

  return drugs_.at(typeID).get();

}

bool DrugsInBlood::is_drug_in_blood(DrugType *drug_type) const {
  return is_drug_in_blood(drug_type->id());
}

bool DrugsInBlood::is_drug_in_blood(const int drugTypeID) const {
  return drugs_.find(drugTypeID)!=drugs_.end();
}

void DrugsInBlood::remove_drug(Drug *drug) {
  remove_drug(drug->drug_type()->id());
}

void DrugsInBlood::remove_drug(const int &drug_type_id) {
  auto it = drugs_.find(drug_type_id);
  if (it==drugs_.end()) {
    throw std::invalid_argument("Cannot remove drug, drug not found");
  }
  drugs_.erase(it);
}

Drug *DrugsInBlood::get_drug(const int &type_id) const {
  if (!is_drug_in_blood(type_id))
    return nullptr;

  return drugs_.at(type_id).get();
}

std::size_t DrugsInBlood::size() const {
  return drugs_.size();
}

void DrugsInBlood::clear() {
  if (drugs_.empty()) return;
  // simply clear the map, the unique_ptr will be deleted automatically
  drugs_.clear();
}

void DrugsInBlood::update() {
  for (auto &drug : drugs_) {
    drug.second->update();
  }
}

void DrugsInBlood::clear_cut_off_drugs_by_event(Event *event) {
  if (!drugs_.empty()) {
    for (auto pos = drugs_.begin(); pos!=drugs_.end();) {
      //if (pos->second->lastUpdateValue <= 0.1) {
      //Cut off at 10%
      if (pos->second->last_update_value() <= DRUG_CUT_OFF_VALUE) {
        drugs_.erase(pos++);
      } else {
        ++pos;
      }
    }
  }
}
