#ifndef PARASITEDENSITYUPDATEFUNCTION_H
#define    PARASITEDENSITYUPDATEFUNCTION_H

class ClonalParasitePopulation;

class ParasiteDensityUpdateFunction {
  //disallow copy and assign
  ParasiteDensityUpdateFunction(const ParasiteDensityUpdateFunction &) = delete;
  void operator=(const ParasiteDensityUpdateFunction &) = delete;

public:
  ParasiteDensityUpdateFunction();

  //    ParasiteUpdateFunction(const ParasiteUpdateFunction& orig);
  virtual ~ParasiteDensityUpdateFunction();

  virtual double get_current_parasite_density(ClonalParasitePopulation *parasite, int duration) = 0;

 private:

};

#endif    /* PARASITEDENSITYUPDATEFUNCTION_H */

