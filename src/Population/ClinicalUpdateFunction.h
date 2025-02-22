#ifndef CLINICALUPDATEFUNCTION_H
#define    CLINICALUPDATEFUNCTION_H

#include "ParasiteDensity/ParasiteDensityUpdateFunction.h"

class Model;

class ClinicalUpdateFunction : public ParasiteDensityUpdateFunction {
public:
  //disallow copy, assign
  ClinicalUpdateFunction(const ClinicalUpdateFunction&) = delete;
  void operator=(const ClinicalUpdateFunction&) = delete;

private:
  Model* model_;

 public:
  explicit ClinicalUpdateFunction(Model *model = nullptr);
  //    ClinicalUpdateFunction(const ClinicalUpdateFunction& orig);

  virtual ~ClinicalUpdateFunction();

  double get_current_parasite_density(ClonalParasitePopulation *parasite, int duration) override;

};

#endif    /* CLINICALUPDATEFUNCTION_H */
