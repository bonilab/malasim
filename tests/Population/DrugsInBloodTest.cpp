#include <gtest/gtest.h>
#include "Population/DrugsInBlood.h"
#include "Treatment/Therapies/Drug.h"
#include "Treatment/Therapies/DrugType.h"
#include "Population/Person/Person.h"
#include "Configuration/Config.h"
#include "Events/Event.h"
#include <memory>
#include <set>

class DrugsInBloodTest : public ::testing::Test {
protected:
    void SetUp() override {
        person_ = new Person();
        drugs_in_blood_ = new DrugsInBlood(person_);
        destroyed_drugs_.clear();
    }

    void TearDown() override {
        delete drugs_in_blood_;
        delete person_;
    }

    Person* person_;
    DrugsInBlood* drugs_in_blood_;
    static std::set<int> destroyed_drugs_;

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

    // Helper class to track drug destruction
    class TrackedDrug : public Drug {
    public:
        TrackedDrug(DrugType* type, int tracking_id) 
            : Drug(type), tracking_id_(tracking_id) {}
        
        ~TrackedDrug() override {
            destroyed_drugs_.insert(tracking_id_);
        }
        
        int tracking_id() const { return tracking_id_; }
        
    private:
        int tracking_id_;
    };

    // Helper method to create a trackable drug
    TrackedDrug* create_tracked_drug(int type_id, int tracking_id) {
        auto* drug_type = new DrugType();
        drug_type->set_id(type_id);
        return new TrackedDrug(drug_type, tracking_id);
    }

    static bool is_destroyed(int tracking_id) {
        return destroyed_drugs_.find(tracking_id) != destroyed_drugs_.end();
    }
};

std::set<int> DrugsInBloodTest::destroyed_drugs_;

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
    Drug* staying_drug2 = create_test_drug(3, 0.2); // Above cut-off
    Drug* staying_drug3 = create_test_drug(4, 0.2); // Above cut-off
    
    drugs_in_blood_->add_drug(staying_drug);
    drugs_in_blood_->add_drug(cut_off_drug);
    drugs_in_blood_->add_drug(staying_drug2);
    drugs_in_blood_->add_drug(staying_drug3);
    
    drugs_in_blood_->clear_cut_off_drugs();
    
    EXPECT_EQ(drugs_in_blood_->size(), 3);
    EXPECT_TRUE(drugs_in_blood_->contains(1));
    EXPECT_FALSE(drugs_in_blood_->contains(2));
    EXPECT_TRUE(drugs_in_blood_->contains(3));
    EXPECT_TRUE(drugs_in_blood_->contains(4));
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

TEST_F(DrugsInBloodTest, MemoryManagementAddDrug) {
    const int tracking_id = 100;
    {
        TrackedDrug* drug = create_tracked_drug(1, tracking_id);
        drugs_in_blood_->add_drug(drug);
        // Drug should not be destroyed yet
        EXPECT_FALSE(is_destroyed(tracking_id));
    }
    // Drug should still exist in drugs_in_blood_
    EXPECT_FALSE(is_destroyed(tracking_id));
    EXPECT_TRUE(drugs_in_blood_->contains(1));
    
    // Clear should destroy the drug
    drugs_in_blood_->clear();
    EXPECT_TRUE(is_destroyed(tracking_id));
}

TEST_F(DrugsInBloodTest, MemoryManagementReplaceDrug) {
    const int first_tracking_id = 101;
    const int second_tracking_id = 102;
    
    TrackedDrug* drug1 = create_tracked_drug(1, first_tracking_id);
    drugs_in_blood_->add_drug(drug1);
    
    // Adding second drug with same ID should destroy first drug
    TrackedDrug* drug2 = create_tracked_drug(1, second_tracking_id);
    drugs_in_blood_->add_drug(drug2);
    
    EXPECT_TRUE(is_destroyed(first_tracking_id));
    EXPECT_FALSE(is_destroyed(second_tracking_id));
    EXPECT_EQ(drugs_in_blood_->size(), 1);
}

TEST_F(DrugsInBloodTest, MemoryManagementDestructor) {
    const int first_tracking_id = 103;
    const int second_tracking_id = 104;
    
    {
        DrugsInBlood* local_drugs = new DrugsInBlood(person_);
        local_drugs->add_drug(create_tracked_drug(1, first_tracking_id));
        local_drugs->add_drug(create_tracked_drug(2, second_tracking_id));
        
        EXPECT_FALSE(is_destroyed(first_tracking_id));
        EXPECT_FALSE(is_destroyed(second_tracking_id));
        
        delete local_drugs;
        
        // Both drugs should be destroyed when DrugsInBlood is destroyed
        EXPECT_TRUE(is_destroyed(first_tracking_id));
        EXPECT_TRUE(is_destroyed(second_tracking_id));
    }
}

TEST_F(DrugsInBloodTest, MemoryManagementClearCutOff) {
    const int staying_tracking_id = 105;
    const int cut_off_tracking_id = 106;
    
    TrackedDrug* staying_drug = create_tracked_drug(1, staying_tracking_id);
    TrackedDrug* cut_off_drug = create_tracked_drug(2, cut_off_tracking_id);
    
    staying_drug->set_last_update_value(0.2); // Above cut-off
    cut_off_drug->set_last_update_value(0.05); // Below cut-off
    
    drugs_in_blood_->add_drug(staying_drug);
    drugs_in_blood_->add_drug(cut_off_drug);
    
    drugs_in_blood_->clear_cut_off_drugs();
    
    // Cut-off drug should be destroyed, staying drug should remain
    EXPECT_FALSE(is_destroyed(staying_tracking_id));
    EXPECT_TRUE(is_destroyed(cut_off_tracking_id));
    EXPECT_EQ(drugs_in_blood_->size(), 1);
} 