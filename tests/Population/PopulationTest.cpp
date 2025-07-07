#include <gtest/gtest.h>
#include "Population/Population.h"
#include "Population/Person/Person.h"
#include <memory>

class PopulationTest : public ::testing::Test {
protected:
    void SetUp() override {
        population = std::make_unique<Population>();
        population->set_popsize_by_location({0,0});

    }
    std::unique_ptr<Population> population;
};

TEST_F(PopulationTest, ConstructionAndDestruction) {
    EXPECT_NE(population, nullptr);
}


TEST_F(PopulationTest, AddAndSize) {
    EXPECT_EQ(population->size(), 0u);
    auto person = std::make_unique<Person>();
    Person* person_ptr = person.get();
    population->add_person(std::move(person));
    EXPECT_EQ(population->size(), 1u);
    population->remove_person(person_ptr);
    EXPECT_EQ(population->size(), 0u);
}

TEST_F(PopulationTest, AddAndRemoveDeadPerson) {
    auto person = std::make_unique<Person>();
    Person* person_ptr = person.get();
    population->add_person(std::move(person));
    EXPECT_EQ(population->size(), 1u);
    population->remove_dead_person(person_ptr);
    EXPECT_EQ(population->size(), 0u);
}

TEST_F(PopulationTest, SizeAtLocation) {
    // By default, size_at should be 0 for any location
    EXPECT_EQ(population->size_at(0), 0u);
    EXPECT_EQ(population->size_at(1), 0u);
}

TEST_F(PopulationTest, SizeWithLocationAndAgeClass) {
    // By default, should be 0
    EXPECT_EQ(population->size(-1, -1), 0u);
    EXPECT_EQ(population->size(0, -1), 0u);
    EXPECT_EQ(population->size(0, 0), 0u);
}
