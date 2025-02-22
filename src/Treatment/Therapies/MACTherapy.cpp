#include "MACTherapy.h"

MACTherapy::MACTherapy() = default;

MACTherapy::~MACTherapy() = default;

void MACTherapy::add_therapy_id(const int &therapy_id) {
  therapy_ids_.push_back(therapy_id);
}

void MACTherapy::add_schedule(const int &start_at_day) {
  start_at_days_.push_back(start_at_day);
}