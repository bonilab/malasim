#ifndef MULTINOMIALDISTRIBUTIONGENERATOR_H
#define MULTINOMIALDISTRIBUTIONGENERATOR_H

#include "Utils/TypeDef.h"

namespace utils {
class Random;
}

class MultinomialDistributionGenerator {
  // DELETE_COPY_AND_MOVE(MultinomialDistributionGenerator)
  //

public:
  UIntVector data;
  DoubleVector level_density;

  MultinomialDistributionGenerator() = default;

  MultinomialDistributionGenerator(
      const MultinomialDistributionGenerator &orig) = default;

  MultinomialDistributionGenerator &operator=(
      const MultinomialDistributionGenerator &) = default;

  MultinomialDistributionGenerator(MultinomialDistributionGenerator &&) =
      default;

  MultinomialDistributionGenerator &operator=(
      MultinomialDistributionGenerator &&) = default;

  virtual ~MultinomialDistributionGenerator() = default;

  int draw_random_level(utils::Random* random);

  void allocate(utils::Random* random);
};

#endif /* MULTINOMIALDISTRIBUTIONGENERATOR_H */
