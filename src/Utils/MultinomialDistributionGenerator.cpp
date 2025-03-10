#include "MultinomialDistributionGenerator.h"

#include <cassert>

#include "Utils/Random.h"

// TODO: writing test
int MultinomialDistributionGenerator::draw_random_level(utils::Random* random) {
  if (data.empty()) { allocate(random); }

  const int temp = data.back();
  data.pop_back();
  return temp;
}

void MultinomialDistributionGenerator::allocate(utils::Random* random) {
  const auto chunk_size = 100000;
  const auto size = level_density.size();
  UIntVector n(size);
  random->random_multinomial(size, chunk_size, level_density, n);
  //
  //    for(int i :  n){
  //        std::cout << i << "\t";
  //    }
  //    std::cout<< std::endl;
  data.clear();
  for (auto i = 0u; i < size; i++) {
    for (auto j = 0u; j < n[i]; j++) { data.push_back(i); }
  }
  assert(data.size() == chunk_size);
  random->shuffle(data);
}
