#ifndef DRUGDATABASE_H
#define DRUGDATABASE_H

#include "DrugType.h"

using DrugTypePtrVector = std::vector<std::unique_ptr<DrugType>>;

class DrugDatabase : public DrugTypePtrVector {
public:
  // Dsiallow copy and assign
  DrugDatabase(const DrugDatabase &) = delete;
  void operator=(const DrugDatabase &) = delete;
  DrugDatabase(DrugDatabase &&) = delete;
  DrugDatabase &operator=(DrugDatabase &&) = delete;

  DrugDatabase();
  virtual ~DrugDatabase();

  void add(std::unique_ptr<DrugType> dt);

private:
};

#endif /* DRUGDATABASE_H */

