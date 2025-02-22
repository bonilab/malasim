#ifndef DRUGDATABASE_H
#define    DRUGDATABASE_H

#include "DrugType.h"
#include <map>

typedef std::map<int, DrugType *> DrugTypePtrMap;

class DrugDatabase : public DrugTypePtrMap {
  //Dsiallow copy and assign
  DrugDatabase(const DrugDatabase&) = delete;
  void operator=(const DrugDatabase&) = delete;

 public:
  DrugDatabase();

  //    DrugDatabase(const DrugDatabase& orig);
  virtual ~DrugDatabase();

  void add(DrugType *dt);


 private:

};

#endif    /* DRUGDATABASE_H */

