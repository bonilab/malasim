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
  // Iterator type definitions for proxy access
  using iterator = DrugUniquePtrMap::iterator;
  using const_iterator = DrugUniquePtrMap::const_iterator;

  // Iterator proxy methods
  iterator begin() { return drugs_.begin(); }
  iterator end() { return drugs_.end(); }
  const_iterator begin() const { return drugs_.begin(); }
  const_iterator end() const { return drugs_.end(); }
  const_iterator cbegin() const { return drugs_.cbegin(); }
  const_iterator cend() const { return drugs_.cend(); }

  // Map-like proxy methods
  Drug* at(const int& key) const { return drugs_.at(key).get(); }
  bool contains(const int& key) const { return drugs_.find(key) != drugs_.end(); }
  
  Person *person() const {
    return person_;
  }
  void set_person(Person *value) {
    person_ = value;
  }


 public:
  explicit DrugsInBlood(Person *person = nullptr);

  //    DrugsInBlood(const DrugsInBlood& orig);
  virtual ~DrugsInBlood();

  void init();

  Drug *add_drug(Drug *drug);

  std::size_t size() const;

  void clear();

  void update();

  void clear_cut_off_drugs();

};

#endif    /* DRUGSINBLOOD_H */
