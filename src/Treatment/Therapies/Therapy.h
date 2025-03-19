#ifndef THERAPY_H
#define    THERAPY_H

#include <vector>
#include <string>
#include <ostream>

class DrugType;

class Therapy {
//disallow copy and assign
Therapy(const Therapy &orig) = delete;
void operator=(const Therapy &orig) = delete;

public:
  [[nodiscard]] int get_id() const { return id_; }
  void set_id(int value) { id_ = value; }

  [[nodiscard]] int get_testing_day() const { return testing_day_; }
  void set_testing_day(int value) { testing_day_ = value; }

  [[nodiscard]] const std::string& get_name() const { return name_; }
  void set_name(const std::string& value) { name_ = value; }

private:
  int id_;
  int testing_day_;
  std::string name_ = "Therapy";
public:
  std::vector<int> drug_ids;

public:
  Therapy();

  //    Therapy(const Therapy& orig);
  virtual ~Therapy();

  virtual void add_drug(int drug_id);

  friend std::ostream &operator<<(std::ostream &os, const Therapy &therapy);

private:

};

#endif    /* THERAPY_H */

