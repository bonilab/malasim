#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Population/ClonalParasitePopulation.h"
#include "Parasites/Genotype.h"
#include "Population/Person/Person.h"
#include "Population/DrugsInBlood.h"

// Mock Person class
class MockPerson : public Person {
public:
    MockPerson() : Person() {}
    MOCK_METHOD(int, get_latest_update_time, (), (const, override));
};

class SingleHostClonalParasitePopulationsTest : public ::testing::Test {
protected:
    void SetUp() override {
        person = new MockPerson();
        populations = new SingleHostClonalParasitePopulations(person);
        genotype = new Genotype("abcdef");
    }

    void TearDown() override {
        delete populations;
        delete person;
        delete genotype;
    }

    MockPerson* person;
    SingleHostClonalParasitePopulations* populations;
    Genotype* genotype;
};

TEST_F(SingleHostClonalParasitePopulationsTest, Initialization) {
    EXPECT_EQ(populations->size(), 0);
    EXPECT_TRUE(populations->empty());
    EXPECT_EQ(populations->person(), person);
    EXPECT_EQ(populations->log10_total_infectious_density(), SingleHostClonalParasitePopulations::DEFAULT_LOG_DENSITY);
}

TEST_F(SingleHostClonalParasitePopulationsTest, AddAndRemoveParasite) {
    auto* parasite = new ClonalParasitePopulation(genotype);
    
    // Add parasite
    populations->add(parasite);

    parasite = populations->at(0);

    EXPECT_EQ(populations->size(), 1);
    EXPECT_FALSE(populations->empty());
    EXPECT_EQ(populations->at(0), parasite);
    EXPECT_EQ(parasite->get_index(), 0);
    
    // Remove parasite
    populations->remove(parasite);
    EXPECT_EQ(populations->size(), 0);
    EXPECT_TRUE(populations->empty());
}

TEST_F(SingleHostClonalParasitePopulationsTest, RemoveByIndex) {
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    
    // Add two parasites
    populations->add(parasite1);
    populations->add(parasite2);
    EXPECT_EQ(populations->size(), 2);
    
    // Remove first parasite
    populations->remove(0);
    EXPECT_EQ(populations->size(), 1);
    EXPECT_EQ(populations->at(0), parasite2);
    EXPECT_EQ(parasite2->get_index(), 0);
}

TEST_F(SingleHostClonalParasitePopulationsTest, RemoveByIndexEdgeCases) {
    // Test removing from empty population
    EXPECT_THROW(populations->remove(0), std::out_of_range);
    
    // Test removing with negative index
    EXPECT_THROW(populations->remove(-1), std::out_of_range);
    
    // Test removing with index beyond size
    auto* parasite = new ClonalParasitePopulation(genotype);
    populations->add(parasite);
    EXPECT_THROW(populations->remove(1), std::out_of_range);
}

TEST_F(SingleHostClonalParasitePopulationsTest, ContainParasite) {
    auto* parasite = new ClonalParasitePopulation(genotype);
    auto* other_parasite = new ClonalParasitePopulation(genotype);
    
    populations->add(parasite);
    
    EXPECT_TRUE(populations->contain(parasite));
    EXPECT_FALSE(populations->contain(other_parasite));
    EXPECT_FALSE(populations->contain(nullptr));
    
    delete other_parasite;
}

TEST_F(SingleHostClonalParasitePopulationsTest, ClearCuredParasites) {
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    auto* parasite3 = new ClonalParasitePopulation(genotype);
    
    // Set up parasites with different densities
    parasite1->set_last_update_log10_parasite_density(5.0);
    parasite2->set_last_update_log10_parasite_density(-1111.0); // Cured level
    parasite3->set_last_update_log10_parasite_density(3.0);
    
    populations->add(parasite1);
    populations->add(parasite2);
    populations->add(parasite3);
    
    populations->clear_cured_parasites(-1111.0);
    
    EXPECT_EQ(populations->size(), 2);
    EXPECT_EQ(populations->at(0), parasite1);
    EXPECT_EQ(populations->at(1), parasite3);
}

TEST_F(SingleHostClonalParasitePopulationsTest, ClearCuredParasitesEdgeCases) {
    // Test with empty population
    populations->clear_cured_parasites(-1111.0);
    EXPECT_EQ(populations->size(), 0);
    
    // Test with all cured parasites
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    
    parasite1->set_last_update_log10_parasite_density(-1111.0);
    parasite2->set_last_update_log10_parasite_density(-1111.0);
    
    populations->add(parasite1);
    populations->add(parasite2);
    
    populations->clear_cured_parasites(-1111.0);
    EXPECT_EQ(populations->size(), 0);
}

TEST_F(SingleHostClonalParasitePopulationsTest, HasDetectableParasite) {
    auto* parasite = new ClonalParasitePopulation(genotype);
    
    // Test with undetectable parasite
    parasite->set_last_update_log10_parasite_density(-1111.0);
    populations->add(parasite);
    EXPECT_FALSE(populations->has_detectable_parasite(1.0));
    
    // Test with detectable parasite
    parasite->set_last_update_log10_parasite_density(1.0);
    EXPECT_TRUE(populations->has_detectable_parasite(1.0));
    
    // Test with empty population
    populations->clear();
    EXPECT_FALSE(populations->has_detectable_parasite(1.0));
}

TEST_F(SingleHostClonalParasitePopulationsTest, IsGametocytaemic) {
    auto* parasite = new ClonalParasitePopulation(genotype);
    
    // Test with no gametocytes
    parasite->set_gametocyte_level(0.0);
    populations->add(parasite);
    EXPECT_FALSE(populations->is_gametocytaemic());
    
    // Test with gametocytes
    parasite->set_gametocyte_level(0.5);
    EXPECT_TRUE(populations->is_gametocytaemic());
    
    // Test with empty population
    populations->clear();
    EXPECT_FALSE(populations->is_gametocytaemic());
}

TEST_F(SingleHostClonalParasitePopulationsTest, UpdateByDrugs) {
    auto* parasite = new ClonalParasitePopulation(genotype);
    auto* drugs_in_blood = new DrugsInBlood();
    
    populations->add(parasite);
    parasite->set_last_update_log10_parasite_density(5.0);
    // Test update with no drugs
    populations->update_by_drugs(drugs_in_blood);
    std::cout << "Test update with no drugs" << std::endl;
    EXPECT_DOUBLE_EQ(parasite->last_update_log10_parasite_density(), 5.0);
    
    delete drugs_in_blood;
}

TEST_F(SingleHostClonalParasitePopulationsTest, UpdateByDrugsEdgeCases) {
    // Test with empty population
    auto* drugs_in_blood = new DrugsInBlood();
    populations->update_by_drugs(drugs_in_blood);
    delete drugs_in_blood;

    std::cout << "Test with empty population" << std::endl;
    
    // Test with null drugs_in_blood
    auto* parasite = new ClonalParasitePopulation(genotype);
    populations->add(parasite);
    parasite->set_last_update_log10_parasite_density(5.0);
    
    EXPECT_THROW(populations->update_by_drugs(nullptr), std::invalid_argument);
}

TEST_F(SingleHostClonalParasitePopulationsTest, Clear) {
    auto* parasite = new ClonalParasitePopulation(genotype);
    
    populations->add(parasite);
    EXPECT_EQ(populations->size(), 1);
    
    populations->clear();
    EXPECT_EQ(populations->size(), 0);
    EXPECT_TRUE(populations->empty());
    
    // Test clearing already empty population
    populations->clear();
    EXPECT_EQ(populations->size(), 0);
    EXPECT_TRUE(populations->empty());
}

TEST_F(SingleHostClonalParasitePopulationsTest, LatestUpdateTime) {
    // Set up the expectation before calling the method
    EXPECT_CALL(*person, get_latest_update_time())
        .WillOnce(testing::Return(10));
    
    // Call the method and verify the result
    int result = populations->latest_update_time();
    EXPECT_EQ(result, 10);
}

TEST_F(SingleHostClonalParasitePopulationsTest, IteratorAccess) {
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    
    populations->add(parasite1);
    populations->add(parasite2);
    
    // Test iterator access
    auto it = populations->begin();
    EXPECT_EQ((*it).get(), parasite1);
    ++it;
    EXPECT_EQ((*it).get(), parasite2);
    
    // Test const iterator access
    const auto* const_populations = populations;
    auto const_it = const_populations->begin();
    EXPECT_EQ((*const_it).get(), parasite1);
    ++const_it;
    EXPECT_EQ((*const_it).get(), parasite2);
}

TEST_F(SingleHostClonalParasitePopulationsTest, IteratorEdgeCases) {
    // Test empty population iterators
    EXPECT_EQ(populations->begin(), populations->end());
    
    // Test iterator increment beyond end
    auto* parasite = new ClonalParasitePopulation(genotype);
    populations->add(parasite);
    auto it = populations->begin();
    ++it;
    EXPECT_EQ(it, populations->end());
}

TEST_F(SingleHostClonalParasitePopulationsTest, MultipleParasitesWithDifferentDensities) {
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    auto* parasite3 = new ClonalParasitePopulation(genotype);
    
    parasite1->set_last_update_log10_parasite_density(5.0);
    parasite1->set_gametocyte_level(1.0);
    parasite2->set_last_update_log10_parasite_density(3.0);
    parasite2->set_gametocyte_level(1.0);
    parasite3->set_last_update_log10_parasite_density(4.0);
    parasite3->set_gametocyte_level(1.0);
    
    populations->add(parasite1);
    populations->add(parasite2);
    populations->add(parasite3);
    
    // Test total infectious density calculation
    populations->clear_cured_parasites(-1111.0);
    EXPECT_GT(populations->log10_total_infectious_density(), 5.0);
}

TEST_F(SingleHostClonalParasitePopulationsTest, ParasiteIndexManagement) {
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    auto* parasite3 = new ClonalParasitePopulation(genotype);
    
    populations->add(parasite1);
    populations->add(parasite2);
    populations->add(parasite3);
    
    EXPECT_EQ(parasite1->get_index(), 0);
    EXPECT_EQ(parasite2->get_index(), 1);
    EXPECT_EQ(parasite3->get_index(), 2);
    
    // Remove middle parasite and verify indices are updated
    populations->remove(1);
    EXPECT_EQ(parasite1->get_index(), 0);
    EXPECT_EQ(parasite3->get_index(), 1);
} 


TEST_F(SingleHostClonalParasitePopulationsTest, RemoveByParasitePtr) {
    auto* parasite1 = new ClonalParasitePopulation(genotype);
    auto* parasite2 = new ClonalParasitePopulation(genotype);
    auto* parasite3 = new ClonalParasitePopulation(genotype);
    
    populations->add(parasite1);
    populations->add(parasite2);
    populations->add(parasite3);
    
    EXPECT_EQ(parasite1->get_index(), 0);
    EXPECT_EQ(parasite2->get_index(), 1);
    EXPECT_EQ(parasite3->get_index(), 2);
    
    // Remove middle parasite and verify indices are updated
    populations->remove(parasite2);
    EXPECT_EQ(parasite1->get_index(), 0);
    EXPECT_EQ(parasite3->get_index(), 1);
    EXPECT_EQ(populations->size(), 2);
    
    // Check if parasite2 is no longer in the population
    EXPECT_FALSE(populations->contain(parasite2));
    
    // No need to delete parasite2 as unique_ptr will handle it
    // will get segfault if delete parasite2 even though the pointer is still point to the object
    // delete parasite2;
} 