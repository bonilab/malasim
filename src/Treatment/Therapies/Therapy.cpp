#include <Simulation/Model.h>
#include "Therapy.h"
#include "DrugDatabase.h"
#include "Configuration/Config.h"
#include "DrugType.h"

Therapy::Therapy() : id_{-1}, testing_day_{-1}, drug_ids{}, name_{""} {
}

Therapy::~Therapy() = default;

void Therapy::add_drug(int drug_id) {
  drug_ids.push_back(drug_id);
}

std::ostream &operator<<(std::ostream &os, const Therapy &therapy) {
  if ( !therapy.name_.empty() ){
    os << therapy.name_;
  } else {
    os << Model::get_config()->get_drug_parameters().drug_db->at(therapy.drug_ids[0])->name();
    for (int i = 1; i < therapy.drug_ids.size(); ++i) {
      os << "+" << Model::get_config()->get_drug_parameters().drug_db->at(therapy.drug_ids[i])->name();
    }
  }

  return os;
}


//int Therapy::get_therapy_duration(int dosing_day) {
//    int result = 0;
//
//    for (int i = 0; i < drug_ids_.size(); i++) {
//        DrugType* dt = Model::CONFIG->drug_db()->get(drug_ids_[i]);
//        if (!dt->is_artemisinin()) {
//            result = std::max<int>(dt->get_total_duration_of_drug_activity(dosing_day), result);
//        }
//    }
//    return result;
//}