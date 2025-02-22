#ifndef PERSONINDEXALL_H
#define    PERSONINDEXALL_H

#include "PersonIndex.h"

class PersonIndexAll : public PersonIndex {
public:
  //disallow copy and assign
  PersonIndexAll(const PersonIndexAll &) = delete;
  void operator=(const PersonIndexAll &) = delete;

public:
  PersonPtrVector vPerson() {
   return vPerson_;
  }
  void set_vPerson(const PersonPtrVector &value) {
    vPerson_ = value;
  }

private:
  PersonPtrVector vPerson_;

 public:
  PersonIndexAll();

  virtual ~PersonIndexAll();

  virtual void add(Person *p);

  virtual void remove(Person *p);

  virtual std::size_t size() const;

  virtual void update();

  virtual void notify_change(Person *p, const Person::Property &property, const void *oldValue, const void *newValue);

 private:

};

#endif    /* PERSONINDEXALL_H */

