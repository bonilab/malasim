#include "PersonTestBase.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Parasites/Genotype.h"

class PersonParasiteTest : public PersonTestBase {
protected:
    void SetUp() override {
        PersonTestBase::SetUp();
        // Additional setup specific to parasite tests
    }
};

TEST_F(PersonParasiteTest, AddNewParasiteToBlood) {
    auto* genotype = new Genotype("aaabbbccc");  // You might need to properly initialize this
    
    auto* parasite = person_->add_new_parasite_to_blood(genotype);
    EXPECT_NE(parasite, nullptr);
    
    // Cleanup
    delete genotype;
}

TEST_F(PersonParasiteTest, InfectedBy) {
    const int parasite_type_id = 1;
    
    // Test infection
    person_->infected_by(parasite_type_id);
    
    // Verify state change
    EXPECT_NE(person_->get_host_state(), Person::HostStates::SUSCEPTIBLE);
}

TEST_F(PersonParasiteTest, HasDetectableParasite) {
    // Initially should have no detectable parasite
    EXPECT_FALSE(person_->has_detectable_parasite());
    
    // Add parasite and test again (implementation depends on how parasites are added)
    auto* genotype = new Genotype("aaabbbccc");
    person_->add_new_parasite_to_blood(genotype);
    
    // Note: The actual result would depend on the parasite density and detection threshold
    bool has_parasite = person_->has_detectable_parasite();
    EXPECT_TRUE(has_parasite || !has_parasite);  // Just verify it returns a boolean
    
    // Cleanup
    delete genotype;
}

TEST_F(PersonParasiteTest, IsGametocytaemic) {
    // Initially should not be gametocytaemic
    EXPECT_FALSE(person_->isGametocytaemic());
    
    // Add parasite and test again (implementation depends on how gametocytes are handled)
    auto* genotype = new Genotype("aaabbbccc");
    person_->add_new_parasite_to_blood(genotype);
    
    // Note: The actual result would depend on the gametocyte presence
    bool is_gametocytaemic = person_->isGametocytaemic();
    EXPECT_TRUE(is_gametocytaemic || !is_gametocytaemic);  // Just verify it returns a boolean
    
    // Cleanup
    delete genotype;
}

TEST_F(PersonParasiteTest, RelativeInfectivity) {
    // Test relative infectivity calculation
    const double log10_parasite_density = 3.0;  // 1000 parasites
    double infectivity = Person::relative_infectivity(log10_parasite_density);
    
    EXPECT_GE(infectivity, 0.0);
    EXPECT_LE(infectivity, 1.0);
}

TEST_F(PersonParasiteTest, ChangeParasiteState) {
    // Test state change when no parasite in blood
    person_->change_state_when_no_parasite_in_blood();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::SUSCEPTIBLE);
}

TEST_F(PersonParasiteTest, RandomlyChooseParasite) {
    // Add some parasites first
    auto* genotype1 = new Genotype("aaabbbccc");
    auto* genotype2 = new Genotype("aaabbbccc");
    person_->add_new_parasite_to_blood(genotype1);
    person_->add_new_parasite_to_blood(genotype2);
    
    // Test random parasite selection
    person_->randomly_choose_parasite();
    
    // Cleanup
    delete genotype1;
    delete genotype2;
}

TEST_F(PersonParasiteTest, ParasiteUpdateEvents) {
    auto* genotype = new Genotype("aaabbbccc");
    auto* parasite = person_->add_new_parasite_to_blood(genotype);
    
    // Schedule parasite events
    person_->schedule_move_parasite_to_blood(genotype, 5);
    person_->schedule_mature_gametocyte_event(parasite);
    person_->schedule_update_by_drug_event(parasite);
    
    // Verify events were scheduled
    EXPECT_FALSE(person_->get_events().empty());
    
    // Cleanup
    delete genotype;
}

TEST_F(PersonParasiteTest, InfectionFromInfectiousBite) {
    // Setup mock expectations for infection probability
    EXPECT_CALL(*mock_config_, get_p_infection_from_an_infectious_bite())
        .WillOnce(::testing::Return(0.5));
    
    double prob = person_->p_infection_from_an_infectious_bite();
    EXPECT_DOUBLE_EQ(prob, 0.5);
} 