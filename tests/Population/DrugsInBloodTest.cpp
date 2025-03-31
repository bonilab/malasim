#include <gtest/gtest.h>
#include "Population/DrugsInBlood.h"
#include "Treatment/Therapies/Drug.h"
#include "Treatment/Therapies/DrugType.h"
#include "Population/Person/Person.h"
#include "Configuration/Config.h"
#include "Events/Event.h"

class DrugsInBloodTest : public ::testing::Test {
protected:
    void SetUp() override {
        person_ = new Person();
        drugs_in_blood_ = new DrugsInBlood(person_);
    }

    void TearDown() override {
        delete drugs_in_blood_;
        delete person_;
    }

    Person* person_;
    DrugsInBlood* drugs_in_blood_;

    // Helper method to create a drug with specific values
    Drug* create_test_drug(int type_id, double starting_value = 1.0, int dosing_days = 1) {
        auto* drug_type = new DrugType();
        drug_type->set_id(type_id);
        
        auto* drug = new Drug(drug_type);
        drug->set_starting_value(starting_value);
        drug->set_dosing_days(dosing_days);
        drug->set_last_update_value(starting_value);
        return drug;
    }
};

TEST_F(DrugsInBloodTest, InitialState) {
    EXPECT_EQ(drugs_in_blood_->size(), 0);
    EXPECT_EQ(drugs_in_blood_->person(), person_);
}

TEST_F(DrugsInBloodTest, AddDrug) {
    Drug* drug = create_test_drug(1);
    Drug* added_drug = drugs_in_blood_->add_drug(drug);
    
    EXPECT_EQ(drugs_in_blood_->size(), 1);
    EXPECT_TRUE(drugs_in_blood_->contains(1));
    EXPECT_EQ(added_drug, drugs_in_blood_->at(1));
}

TEST_F(DrugsInBloodTest, AddDuplicateDrug) {
    Drug* drug1 = create_test_drug(1, 1.0, 3);
    Drug* drug2 = create_test_drug(1, 2.0, 5);
    
    drugs_in_blood_->add_drug(drug1);
    Drug* updated_drug = drugs_in_blood_->add_drug(drug2);
    
    EXPECT_EQ(drugs_in_blood_->size(), 1);
    EXPECT_EQ(updated_drug->starting_value(), 2.0);
    EXPECT_EQ(updated_drug->dosing_days(), 5);
}

TEST_F(DrugsInBloodTest, Clear) {
    drugs_in_blood_->add_drug(create_test_drug(1));
    drugs_in_blood_->add_drug(create_test_drug(2));
    
    EXPECT_EQ(drugs_in_blood_->size(), 2);
    
    drugs_in_blood_->clear();
    EXPECT_EQ(drugs_in_blood_->size(), 0);
}

TEST_F(DrugsInBloodTest, Update) {
    Drug* drug1 = create_test_drug(1);
    Drug* drug2 = create_test_drug(2);
    
    drugs_in_blood_->add_drug(drug1);
    drugs_in_blood_->add_drug(drug2);
    
    drugs_in_blood_->update();
    
    // Note: The actual update logic would depend on Drug::update() implementation
    // This test verifies that update() can be called without errors
    EXPECT_EQ(drugs_in_blood_->size(), 2);
}

TEST_F(DrugsInBloodTest, ClearCutOffDrugs) {
    Drug* staying_drug = create_test_drug(1, 0.2); // Above cut-off
    Drug* cut_off_drug = create_test_drug(2, 0.05); // Below cut-off
    
    drugs_in_blood_->add_drug(staying_drug);
    drugs_in_blood_->add_drug(cut_off_drug);
    
    drugs_in_blood_->clear_cut_off_drugs();
    
    EXPECT_EQ(drugs_in_blood_->size(), 1);
    EXPECT_TRUE(drugs_in_blood_->contains(1));
    EXPECT_FALSE(drugs_in_blood_->contains(2));
}

TEST_F(DrugsInBloodTest, IteratorAccess) {
    drugs_in_blood_->add_drug(create_test_drug(1));
    drugs_in_blood_->add_drug(create_test_drug(2));
    
    std::size_t count = 0;
    for (const auto& drug : *drugs_in_blood_) {
        EXPECT_TRUE(drug.first == 1 || drug.first == 2);
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST_F(DrugsInBloodTest, ConstIteratorAccess) {
    drugs_in_blood_->add_drug(create_test_drug(1));
    
    const DrugsInBlood* const_drugs = drugs_in_blood_;
    std::size_t count = 0;
    for (const auto& drug : *const_drugs) {
        EXPECT_EQ(drug.first, 1);
        count++;
    }
    EXPECT_EQ(count, 1);
} 