#ifndef DRUGSINBLOOD_H
#define    DRUGSINBLOOD_H

#include "Utils/TypeDef.hxx"

class Person;

class Drug;

class Event;

class DrugType;

class DrugsInBlood {
  // OBJECTPOOL(DrugsInBlood)
  //disallow copy and assign
  DrugsInBlood(const DrugsInBlood&) = delete;
  void operator=(const DrugsInBlood&) = delete;
  DrugsInBlood(const DrugsInBlood&&) = delete;
  DrugsInBlood& operator=(const DrugsInBlood&&) = delete;

 private:
 Person *person_{nullptr};
 DrugUniquePtrMap drugs_{};

 public:
 Person *person() const {
   return person_;
 }
 void set_person(Person *value) {
   person_ = value;
 }

 // Getter for drugs map
 const DrugUniquePtrMap& get_drugs() const {
   return drugs_;
 }

//  // Add a single drug to the map
//  void add_drug_to_map(int drug_id, Drug* drug) {
//    drugs_[drug_id] = std::unique_ptr<Drug>(drug);
//  }

//  // Remove a single drug from the map
//  void remove_drug_from_map(int drug_id) {
//    drugs_.erase(drug_id);
//  }

//  // Get a specific drug by ID
//  Drug* get_drug_from_map(int drug_id) const {
//    auto it = drugs_.find(drug_id);
//    return it != drugs_.end() ? it->second.get() : nullptr;
//  }

 public:
  explicit DrugsInBlood(Person *person = nullptr);

  //    DrugsInBlood(const DrugsInBlood& orig);
  virtual ~DrugsInBlood();

  void init();

  Drug *add_drug(Drug *drug);

  bool is_drug_in_blood(DrugType *drug_type) const;

  bool is_drug_in_blood(int drug_type_id) const;

  void remove_drug(Drug *drug);

  void remove_drug(const int &drug_type_id);

  Drug *get_drug(const int &type_id) const;

  std::size_t size() const;

  void clear();

  void update();

  void clear_cut_off_drugs_by_event(Event *event);

};

#endif    /* DRUGSINBLOOD_H */
