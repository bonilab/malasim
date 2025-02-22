#ifndef PERSONINDEX_H
#define    PERSONINDEX_H

#include "Population/Person/Person.h"

class PersonIndex {
  //disallow copy and assignment
  PersonIndex(const PersonIndex &) = delete;
  void operator=(const PersonIndex &) = delete;

 public:
  PersonIndex();

  virtual ~PersonIndex();

  virtual void add(Person *p) = 0;

  virtual void remove(Person *p) = 0;

  virtual std::size_t size() const = 0;

  virtual void update() = 0;

  virtual void
  notify_change(Person *p, const Person::Property &property, const void *oldValue, const void *newValue) = 0;

 private:

};

#endif    /* PERSONINDEX_H */

