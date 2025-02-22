#ifndef IMMUNITYCLEARANCEUPDATEFUNCTION_H
#define    IMMUNITYCLEARANCEUPDATEFUNCTION_H

#include "Population/ParasiteDensity/ParasiteDensityUpdateFunction.h"

class Model;

class ImmunityClearanceUpdateFunction : public ParasiteDensityUpdateFunction {

  ImmunityClearanceUpdateFunction(const ImmunityClearanceUpdateFunction&) = delete;
  void operator=(const ImmunityClearanceUpdateFunction&) = delete;

public:
  [[nodiscard]] Model* model() const {
    return model_;
  }
  void set_model(Model *value) {
    model_ = value;
  }

private:
  Model* model_;
public:
  explicit ImmunityClearanceUpdateFunction(Model *model = nullptr);

  //    ImmunityClearanceUpdateFunction(const ImmunityClearanceUpdateFunction& orig);
  virtual ~ImmunityClearanceUpdateFunction();

  double get_current_parasite_density(ClonalParasitePopulation *parasite, int duration) override;

};

#endif    /* IMMUNITYCLEARANCEUPDATEFUNCTION_H */
