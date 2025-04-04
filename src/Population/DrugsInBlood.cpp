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
  drug->set_person_drugs(this);
  
  // Create unique_ptr from raw pointer
  std::unique_ptr<Drug> new_drug(drug);
  
  auto [it, inserted] = drugs_.insert_or_assign(typeID, std::move(new_drug));
  return it->second.get();
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
