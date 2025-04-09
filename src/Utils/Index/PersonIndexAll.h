#ifndef PERSONINDEXALL_H
#define PERSONINDEXALL_H

#include <memory>
#include <vector>

#include "Population/Person/Person.h"

using PersonUniquePtr = std::unique_ptr<Person>;
using PersonUniquePtrVector = std::vector<PersonUniquePtr>;

class PersonIndexAll {
public:
  // disallow copy and assign
  PersonIndexAll(const PersonIndexAll &) = delete;
  void operator=(const PersonIndexAll &) = delete;
  PersonIndexAll(PersonIndexAll &&) = delete;
  PersonIndexAll &operator=(PersonIndexAll &&) = delete;

  PersonIndexAll();
  ~PersonIndexAll();

  PersonUniquePtrVector &v_person() { return v_person_; }

  void add(PersonUniquePtr person);

  void remove(Person* person);

  void clear();

  [[nodiscard]] std::size_t size() const;

  void update();

private:
  PersonUniquePtrVector v_person_;
};

#endif /* PERSONINDEXALL_H */

