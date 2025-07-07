#include "gtest/gtest.h"
#include "Parasites/GenotypeDatabase.h"
#include "Parasites/Genotype.h"
#include <memory>

class GenotypeDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_unique<GenotypeDatabase>();
    }
    std::unique_ptr<GenotypeDatabase> db;
};

TEST_F(GenotypeDatabaseTest, AddAndRetrieveGenotype) {
    std::string aa_seq = "||||NY1||TTHFIMG,x||||||FNCMYRIPRPCRA|1";
    auto genotype = std::make_unique<Genotype>(aa_seq);
    int id = 42;
    genotype->set_genotype_id(id);
    db->add(std::move(genotype));
    EXPECT_EQ(db->get_genotype(aa_seq)->genotype_id(), id);
}

TEST_F(GenotypeDatabaseTest, GetIdFromSequence) {
    std::string aa_seq = "||||NY1||TTHFIMG,x||||||FNCMYRIPRPCRA|1";
    auto genotype = std::make_unique<Genotype>(aa_seq);
    int id = 7;
    genotype->set_genotype_id(id);
    db->add(std::move(genotype));
    EXPECT_EQ(db->get_id(aa_seq), id);
}

TEST_F(GenotypeDatabaseTest, GetGenotypeFromAllelesStructure) {
    std::string aa_seq = "||||NY1||TTHFIMG,x||||||FNCMYRIPRPCRA|1";
    auto genotype = std::make_unique<Genotype>(aa_seq);
    int id = 5;
    genotype->set_genotype_id(id);
    db->set_weight({1, 2});
    db->add(std::move(genotype));
    IntVector alleles = {5, 0};
    auto* result = db->get_genotype_from_alleles_structure(alleles);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->genotype_id(), id);
}

TEST_F(GenotypeDatabaseTest, GetWeightSetWeight) {
    std::vector<int> weights = {1, 2, 3};
    db->set_weight(weights);
    auto w = db->get_weight();
    EXPECT_EQ(w, weights);
}

// Add more tests for get_min_ec50, at, etc. as needed
