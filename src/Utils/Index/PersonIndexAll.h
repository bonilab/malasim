#ifndef PERSONINDEXALL_H
#define PERSONINDEXALL_H

#include "PersonIndex.h"

class PersonIndexAll : public PersonIndex {
public:
  // disallow copy and assign
  PersonIndexAll(const PersonIndexAll &) = delete;
  void operator=(const PersonIndexAll &) = delete;
  PersonIndexAll(PersonIndexAll &&) = delete;
  PersonIndexAll &operator=(PersonIndexAll &&) = delete;

  PersonIndexAll();
  explicit PersonIndexAll(PersonPtrVector v_person) : v_person_(std::move(v_person)) {}
  ~PersonIndexAll() override;
  PersonPtrVector &v_person() { return v_person_; }
  void set_v_person(const PersonPtrVector &value) { v_person_ = value; }

  void add(Person* person) override;

  void remove(Person* person) override;

  [[nodiscard]] std::size_t size() const override;

  void update() override;

  void notify_change(Person* person, const Person::Property &property, const void* old_value,
                     const void* new_value) override;

private:
  PersonPtrVector v_person_;
};

#endif /* PERSONINDEXALL_H */

