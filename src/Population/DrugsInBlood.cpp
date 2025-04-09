#include "DrugsInBlood.h"

#include "Events/Event.h"
#include "Population/Person/Person.h"
#include "Treatment/Therapies/Drug.h"
#include "Treatment/Therapies/DrugType.h"
#include "Utils/TypeDef.h"

#ifndef DRUG_CUT_OFF_VALUE
#define DRUG_CUT_OFF_VALUE 0.1
#endif

DrugsInBlood::DrugsInBlood(Person* person) : person_(person) {}

void DrugsInBlood::init() { drugs_.clear(); }

DrugsInBlood::~DrugsInBlood() {
  if (!drugs_.empty()) { clear(); }
}

Drug* DrugsInBlood::add_drug(std::unique_ptr<Drug> drug) {
  int type_id = drug->drug_type()->id();
  drug->set_person_drugs(this);

  // Create unique_ptr from raw pointer
  auto [it, inserted] = drugs_.insert_or_assign(type_id, std::move(drug));
  return it->second.get();
}

std::size_t DrugsInBlood::size() const { return drugs_.size(); }

void DrugsInBlood::clear() {
  if (drugs_.empty()) return;
  drugs_.clear();
}

void DrugsInBlood::update() {
  for (auto &drug : *this) { drug.second->update(); }
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
