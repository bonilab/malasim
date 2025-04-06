#include "DrugDatabase.h"

#include <memory>

DrugDatabase::DrugDatabase() = default;

DrugDatabase::~DrugDatabase() { this->clear(); }

void DrugDatabase::add(std::unique_ptr<DrugType> dt) {
    if (dt->id() >= this->size()) {
        this->resize(dt->id() + 1);
    }

    this->at(dt->id()) = std::move(dt);
}

