#include <gtest/gtest.h>

#include "Configuration/Config.h"
#include "Utils/Random.h"
#include "Mosquito/Mosquito.h"
#include "Population/Person/Person.h"

class MosquitoTest : public ::testing::Test {
protected:
  void SetUp() override {}
  // void TearDown() override {}
};


TEST_F(MosquitoTest, PrmcSample) {
  Mosquito m;
  utils::Random r;
  r.set_seed(1);

  int prmc_size { 10 };
  int pop_size { 100 };

  // repeat the process 10 times
  for (int n = 0; n < 10; ++n) {
    std::vector<double> foi_distribution(pop_size, 1.0);

    // even id person will have no selection
    for (int i = 0; i < pop_size; i += 2) {
      foi_distribution[i] = 0;
    }

    std::vector<std::unique_ptr<Person>> all_person;
    std::vector<Person*> all_person_ptr;
    for (int i = 0; i < pop_size; ++i) {
      auto p = std::make_unique<Person>();
      p->set_last_therapy_id(i);
      all_person.push_back(std::move(p));
      all_person_ptr.push_back(all_person.back().get());
    }

    auto samples = r.multinomial_sampling<Person>(prmc_size, foi_distribution, all_person_ptr, true);

    EXPECT_EQ(samples.size(), prmc_size);

    for (int i = 0; i < prmc_size; ++i) {
      EXPECT_EQ(samples[i]->get_last_therapy_id() % 2, 1)
          << fmt::format("failed with p_id: {}", samples[i]->get_last_therapy_id());
    }

    all_person.clear();
    all_person_ptr.clear();
  }
}

TEST_F(MosquitoTest, PrmcSampleWhenThereIsNoFOI) {
  Mosquito m;
  utils::Random r;
  r.set_seed(1);

  int prmc_size { 10 };
  int pop_size { 100 };

  // repeat the process 10 times
  for (int n = 0; n < 1; ++n) {
    std::vector<double> foi_distribution(pop_size, 0);

    std::vector<std::unique_ptr<Person>> all_person;
    std::vector<Person*> all_person_ptr;
    for (int i = 0; i < pop_size; ++i) {
      auto p = std::make_unique<Person>();
      p->set_last_therapy_id(i);
      all_person.push_back(std::move(p));
      all_person_ptr.push_back(all_person.back().get());
    }

    auto samples = r.multinomial_sampling<Person>(prmc_size, foi_distribution, all_person_ptr, true);
    EXPECT_EQ(samples.size(), prmc_size);

    for (int i = 0; i < prmc_size; ++i) {
      EXPECT_EQ(samples[i], nullptr);
    }

    all_person.clear();
    all_person_ptr.clear();
  }
}
