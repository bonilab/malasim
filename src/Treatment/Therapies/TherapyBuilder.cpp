#include "TherapyBuilder.h"

#include <spdlog/spdlog.h>

#include "Therapy.h"
#include "SCTherapy.h"
#include "MACTherapy.h"

TherapyBuilder::TherapyBuilder() = default;

TherapyBuilder::~TherapyBuilder() = default;

Therapy* TherapyBuilder::build(const YAML::Node &ns, const int &t_id) {
  Therapy* t = nullptr;

  if (ns["drug_ids"]) {
    t = new SCTherapy();
    if (ns["name"]){
      t->set_name(ns["name"].as<std::string>());
    }
    t->set_id(t_id);
    spdlog::info("Processing TherapyInfo for SCTherapy {} {}", t->get_name(), t_id);
    for (auto i = 0; i < ns["drug_ids"].size(); i++) {
      const auto drug_id = ns["drug_ids"][i].as<int>();
      //        std::cout << therapy_id << "-" << drug_id << std::endl;
      dynamic_cast<SCTherapy*>(t)->add_drug(drug_id);
    }
    for (auto i = 0; i < ns["dosing_days"].size(); i++) {
      const auto dosing_days = ns["dosing_days"][i].as<int>();
      dynamic_cast<SCTherapy*>(t)->dosing_day.push_back(dosing_days);
    }
  } else {
    if (ns["therapy_ids"]) {
      t = new MACTherapy();
      if (ns["name"]){
        t->set_name(ns["name"].as<std::string>());
      }
      t->set_id(t_id);
      spdlog::info("Processing TherapyInfo for MACTherapy {} {}", t->get_name(), t_id);

      for (int i = 0; i < ns["therapy_ids"].size(); i++) {
        auto therapy_id = ns["therapy_ids"][i].as<int>();
        //        std::cout << therapy_id << "-" << drug_id << std::endl;
        dynamic_cast<MACTherapy*>(t)->add_therapy_id(therapy_id);
      }
      for (int i = 0; i < ns["regimen"].size(); i++) {
        auto starting_day = ns["regimen"][i].as<int>();
        //        std::cout << therapy_id << "-" << drug_id << std::endl;
        dynamic_cast<MACTherapy*>(t)->add_schedule(starting_day);
      }
    }
  }

  return t;
}
