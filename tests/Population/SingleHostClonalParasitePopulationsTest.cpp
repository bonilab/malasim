#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Population/ClonalParasitePopulation.h"
#include "Parasites/Genotype.h"
#include "Population/Person/Person.h"
#include "Population/DrugsInBlood.h"
#include "Configuration/Config.h"
#include <memory>
#include <set>

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
        destroyed_parasites_.clear();
    }

    void TearDown() override {
        delete populations;
        delete person;
        delete genotype;
    }

    MockPerson* person;
    SingleHostClonalParasitePopulations* populations;
    Genotype* genotype;
    static std::set<int> destroyed_parasites_;

    // Helper class to track parasite destruction
    class TrackedParasite : public ClonalParasitePopulation {
    public:
        TrackedParasite(int tracking_id) : tracking_id_(tracking_id) {}
        
        ~TrackedParasite() override {
            destroyed_parasites_.insert(tracking_id_);
        }
        
        int tracking_id() const { return tracking_id_; }
        
    private:
        int tracking_id_;
    };

    // Helper method to create a trackable parasite
    TrackedParasite* create_tracked_parasite(int tracking_id) {
        auto* parasite = new TrackedParasite(tracking_id);
        // Set any necessary default values
        parasite->set_last_update_log10_parasite_density(2.0); // Above cured threshold
        return parasite;
    }

    static bool is_destroyed(int tracking_id) {
        return destroyed_parasites_.find(tracking_id) != destroyed_parasites_.end();
    }
};

std::set<int> SingleHostClonalParasitePopulationsTest::destroyed_parasites_;

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

TEST_F(SingleHostClonalParasitePopulationsTest, MemoryManagementAdd) {
    const int tracking_id = 100;
    {
        TrackedParasite* parasite = create_tracked_parasite(tracking_id);
        populations->add(parasite);
        EXPECT_FALSE(is_destroyed(tracking_id));
    }
    // Parasite should still exist in populations
    EXPECT_FALSE(is_destroyed(tracking_id));
    EXPECT_EQ(populations->size(), 1);
    
    // Clear should destroy the parasite
    populations->clear();
    EXPECT_TRUE(is_destroyed(tracking_id));
    EXPECT_EQ(populations->size(), 0);
}

TEST_F(SingleHostClonalParasitePopulationsTest, MemoryManagementRemove) {
    const int tracking_id1 = 101;
    const int tracking_id2 = 102;
    
    TrackedParasite* parasite1 = create_tracked_parasite(tracking_id1);
    TrackedParasite* parasite2 = create_tracked_parasite(tracking_id2);
    
    populations->add(parasite1);
    populations->add(parasite2);
    EXPECT_EQ(populations->size(), 2);
    
    // Remove first parasite
    populations->remove(0);
    EXPECT_TRUE(is_destroyed(tracking_id1));
    EXPECT_FALSE(is_destroyed(tracking_id2));
    EXPECT_EQ(populations->size(), 1);
}

TEST_F(SingleHostClonalParasitePopulationsTest, MemoryManagementDestructor) {
    const int tracking_id1 = 103;
    const int tracking_id2 = 104;
    
    {
        auto* local_populations = new SingleHostClonalParasitePopulations(person);
        local_populations->add(create_tracked_parasite(tracking_id1));
        local_populations->add(create_tracked_parasite(tracking_id2));
        
        EXPECT_FALSE(is_destroyed(tracking_id1));
        EXPECT_FALSE(is_destroyed(tracking_id2));
        
        delete local_populations;
        
        // Both parasites should be destroyed when populations is destroyed
        EXPECT_TRUE(is_destroyed(tracking_id1));
        EXPECT_TRUE(is_destroyed(tracking_id2));
    }
}

TEST_F(SingleHostClonalParasitePopulationsTest, MemoryManagementClearCuredParasites) {
    const int staying_id = 105;
    const int cleared_id = 106;
    
    TrackedParasite* staying_parasite = create_tracked_parasite(staying_id);
    TrackedParasite* cleared_parasite = create_tracked_parasite(cleared_id);
    
    staying_parasite->set_last_update_log10_parasite_density(2.0); // Above threshold
    cleared_parasite->set_last_update_log10_parasite_density(-4.0); // Below threshold
    
    populations->add(staying_parasite);
    populations->add(cleared_parasite);
    
    populations->clear_cured_parasites(-2.0); // Set threshold between the two parasites
    
    // Cleared parasite should be destroyed, staying parasite should remain
    EXPECT_FALSE(is_destroyed(staying_id));
    EXPECT_TRUE(is_destroyed(cleared_id));
    EXPECT_EQ(populations->size(), 1);
}

TEST_F(SingleHostClonalParasitePopulationsTest, MemoryManagementVectorReallocation) {
    std::vector<int> tracking_ids;
    const int num_parasites = 10;
    
    // Add enough parasites to force vector reallocation
    for (int i = 0; i < num_parasites; i++) {
        tracking_ids.push_back(i);
        populations->add(create_tracked_parasite(tracking_ids.back()));
        EXPECT_FALSE(is_destroyed(tracking_ids.back()));
    }
    
    EXPECT_EQ(populations->size(), num_parasites);
    
    // When removing index 3 repeatedly:
    // 1. parasite[3] is replaced by parasite[9], parasite[3] is destroyed
    // 2. parasite[3] (which was parasite[9]) is replaced by parasite[8], parasite[9] is destroyed
    // 3. parasite[3] (which was parasite[8]) is replaced by parasite[7], parasite[8] is destroyed
    // 4. parasite[3] (which was parasite[7]) is replaced by parasite[6], parasite[7] is destroyed

    std::vector<int> expected_destroyed_parasites{3, 9, 8, 7};
    for (int i = 0; i < 4; i++) {
        populations->remove(3);
        
        EXPECT_EQ(populations->size(), num_parasites - i - 1);

        // destroyed_parasites_ should contain 3, 9, 8, 7 iteratively
        for (int j =0; j < std::min(i,4); j++) {
            EXPECT_TRUE(is_destroyed(expected_destroyed_parasites[j]));
        }
    }
    
    EXPECT_EQ(populations->size(), num_parasites - 4);
    
    // Final state verification:
    // - First 3 parasites (0,1,2) should be alive
    for (int i = 0; i < 3; i++) {
        EXPECT_FALSE(is_destroyed(i));
    }

    // parasites 3, 7,8,9 should be destroyed
    EXPECT_TRUE(is_destroyed(3));
    EXPECT_TRUE(is_destroyed(7));
    EXPECT_TRUE(is_destroyed(8));
    EXPECT_TRUE(is_destroyed(9));

    // parasites 4,5,6 should be alive
    EXPECT_FALSE(is_destroyed(4));
    EXPECT_FALSE(is_destroyed(5));
    EXPECT_FALSE(is_destroyed(6));
} 