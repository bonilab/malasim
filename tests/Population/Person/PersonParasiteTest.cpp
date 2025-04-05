#include "PersonTestBase.h"
#include "Population/SingleHostClonalParasitePopulations.h"
#include "Parasites/Genotype.h"
#include "Events/MoveParasiteToBloodEvent.h"
#include "Events/MatureGametocyteEvent.h"
#include "Events/UpdateWhenDrugIsPresentEvent.h"

class PersonParasiteTest : public PersonTestBase {
protected:
    int current_time_ = 30;
    void SetUp() override {
        PersonTestBase::SetUp();
        mock_scheduler_->set_current_time(current_time_);
        // Additional setup specific to parasite tests
    }
};

TEST_F(PersonParasiteTest, AddNewParasiteToBlood) {
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    
    auto parasite = person_->add_new_parasite_to_blood(genotype);
    EXPECT_NE(parasite, nullptr);    
    // Cleanup
}

TEST_F(PersonParasiteTest, InfectedByWhenLiverParasiteTypeIsNotSet) {
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(1);

    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);

    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    // Test infection
    person_->infected_by(1);
    
    // Verify stage changed to Exposed
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);

    // check if just liver parasite type is set, parasite is not set to blood
    EXPECT_EQ(person_->liver_parasite_type(), genotype);

    // a parasite is scheduled to be moved to blood in next 7 days
    EXPECT_EQ(person_->get_events().size(), 1);

    // cast to MoveParasiteToBloodEvent
    auto event = dynamic_cast<MoveParasiteToBloodEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->infection_genotype(), genotype);
    EXPECT_EQ(event->get_time(), current_time_ + 7);
}

TEST_F(PersonParasiteTest, InfectedByWhenLiverParasiteTypeIsSet) {
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(1);

    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));

    person_->infected_by(1);
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);
    EXPECT_EQ(person_->liver_parasite_type(), genotype);

    auto new_genotype = new Genotype("abcabcabc");
    new_genotype->set_genotype_id(2);
    Model::get_genotype_db()->insert(std::make_pair(2, new_genotype));
    person_->infected_by(2);
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);

    // new genotype is not set to liver parasite type
    EXPECT_EQ(person_->liver_parasite_type(), genotype);

    // a parasite is scheduled to be moved to blood in next 7 days by the old genotype
    EXPECT_EQ(person_->get_events().size(), 1);

    // cast to MoveParasiteToBloodEvent
    auto event = dynamic_cast<MoveParasiteToBloodEvent*>(person_->get_events().begin()->second.get());
    // infection genotype is the old genotype
    EXPECT_EQ(event->infection_genotype(), genotype);
    EXPECT_EQ(event->get_time(), current_time_ + 7);
    
}

TEST_F(PersonParasiteTest, HasDetectableParasiteWhenNoParasiteInBlood) {
    bool has_detectable_parasite = person_->has_detectable_parasite();
    EXPECT_FALSE(has_detectable_parasite);
}

TEST_F(PersonParasiteTest, HasDetectableParasiteWhenParasiteInBlood) {
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    auto parasite = person_->add_new_parasite_to_blood(genotype);
    parasite->set_last_update_log10_parasite_density(2.0);
    bool has_detectable_parasite = person_->has_detectable_parasite();
    EXPECT_TRUE(has_detectable_parasite);
}

TEST_F(PersonParasiteTest, HasParasiteInBloodsButUnderDetectionThreshold) {
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    auto parasite = person_->add_new_parasite_to_blood(genotype);
    parasite->set_last_update_log10_parasite_density(-2.0);
    bool has_detectable_parasite = person_->has_detectable_parasite();
    EXPECT_FALSE(has_detectable_parasite);
}

TEST_F(PersonParasiteTest, IsGametocytaemicWhenParasiteInBloodAndCanProduceGametocytes) {
    // Initially should not be gametocytaemic
    EXPECT_FALSE(person_->isGametocytaemic());
    
    // Add parasite and test again (implementation depends on how gametocytes are handled)
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    // have a parasite in blood, and parasite that can produce gametocytes
    auto parasite = person_->add_new_parasite_to_blood(genotype);
    parasite->set_gametocyte_level(1.0);

    bool is_gametocytaemic = person_->isGametocytaemic();
    EXPECT_TRUE(is_gametocytaemic);
}

TEST_F(PersonParasiteTest, NotGametocytaemicWhenNoParasiteInBlood) {
    EXPECT_FALSE(person_->isGametocytaemic());
}

TEST_F(PersonParasiteTest, NotGametocytaemicWhenParasiteInBloodButNoGametocytes) {
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    auto parasite = person_->add_new_parasite_to_blood(genotype);
    parasite->set_gametocyte_level(0.0);
    EXPECT_FALSE(person_->isGametocytaemic());
}

// TODO: Implement this test later
TEST_F(PersonParasiteTest, RelativeInfectivity) {
    GTEST_SKIP() << "This test is marked for improvement later.";
    // Test relative infectivity calculation
    const double log10_parasite_density = 3.0;  // 1000 parasites
    double infectivity = Person::relative_infectivity(log10_parasite_density);
    
    EXPECT_GE(infectivity, 0.0);
    EXPECT_LE(infectivity, 1.0);
    /// make it fail
    EXPECT_FALSE(true);
}

TEST_F(PersonParasiteTest, ChangeStateWhenNoParasiteInBloodWhenNoParasiteInLiver) {
    // Test state change when no parasite in blood
    // there is 2 possible states, SUSCEPTIBLE and EXPOSED
    // if there is no parasite in liver, the state should be SUSCEPTIBLE
    // if there is a parasite in liver, the state should be EXPOSED

    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(2);

    // fake the state to be CLINICAL
    person_->set_host_state(Person::HostStates::CLINICAL);
    person_->change_state_when_no_parasite_in_blood();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::SUSCEPTIBLE);
}

TEST_F(PersonParasiteTest, ChangeStateWhenNoParasiteInBloodWhenParasiteInLiver) {
    // Test state change when no parasite in blood
    // there is 2 possible states, SUSCEPTIBLE and EXPOSED
    // if there is no parasite in liver, the state should be SUSCEPTIBLE
    // if there is a parasite in liver, the state should be EXPOSED
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(2);
    
     // fake the state to be CLINICAL
    person_->set_host_state(Person::HostStates::CLINICAL);
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    person_->set_liver_parasite_type(genotype);
    person_->change_state_when_no_parasite_in_blood();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);
}

TEST_F(PersonParasiteTest, RandomlyChooseParasiteFrom2Infections) {
    EXPECT_CALL(*mock_random_, random_uniform(2)).WillOnce(::testing::Return(1));
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(1);

    // choose parasite from today infections
    auto genotype1 = new Genotype("aaabbbccc");
    genotype1->set_genotype_id(1);
    auto genotype2 = new Genotype("aaabbbccc");
    genotype2->set_genotype_id(2);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype1));
    Model::get_genotype_db()->insert(std::make_pair(2, genotype2));

    person_->get_today_infections().push_back(1);
    person_->get_today_infections().push_back(2);

    person_->randomly_choose_parasite();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);
    EXPECT_EQ(person_->liver_parasite_type(), genotype2);
    EXPECT_EQ(person_->get_today_infections().size(), 0);

    // should have 1 event scheduled
    EXPECT_EQ(person_->get_events().size(), 1);
    auto event = dynamic_cast<MoveParasiteToBloodEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->infection_genotype(), genotype2);
    EXPECT_EQ(event->get_time(), current_time_ + 7);
}

TEST_F(PersonParasiteTest, RandomlyChooseParasiteFrom1Infection) {
    EXPECT_CALL(*mock_random_, random_uniform(1)).Times(0); // no neeed to randomly choose
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(1);

    // choose parasite from today infections
    auto genotype1 = new Genotype("aaabbbccc");
    genotype1->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype1));
    person_->get_today_infections().push_back(1);
    person_->randomly_choose_parasite();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);
    EXPECT_EQ(person_->liver_parasite_type(), genotype1);
    EXPECT_EQ(person_->get_today_infections().size(), 0);

    // should have 1 event scheduled
    EXPECT_EQ(person_->get_events().size(), 1);
    auto event = dynamic_cast<MoveParasiteToBloodEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->infection_genotype(), genotype1);
    EXPECT_EQ(event->get_time(), current_time_ + 7);
}

TEST_F(PersonParasiteTest, RandomlyChooseParasiteFrom0Infections) {
    EXPECT_CALL(*mock_random_, random_uniform(0)).Times(0); // no neeed to randomly choose
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(0);

    person_->randomly_choose_parasite();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::SUSCEPTIBLE);
    EXPECT_EQ(person_->get_today_infections().size(), 0);
    EXPECT_EQ(person_->get_events().size(), 0);
}

TEST_F(PersonParasiteTest, RandomlyChooseParasiteFrom4Infections) {
    EXPECT_CALL(*mock_random_, random_uniform(4)).WillOnce(::testing::Return(2));
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::HOST_STATE, _, _)).Times(1);

    // choose parasite from today infections
    auto genotype1 = new Genotype("aaabbbccc");
    genotype1->set_genotype_id(1);
    auto genotype2 = new Genotype("aaabbbccc");
    genotype2->set_genotype_id(2);
    auto genotype3 = new Genotype("aaabbbccc");
    genotype3->set_genotype_id(3);
    auto genotype4 = new Genotype("aaabbbccc");
    genotype4->set_genotype_id(4);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype1));
    Model::get_genotype_db()->insert(std::make_pair(2, genotype2));
    Model::get_genotype_db()->insert(std::make_pair(3, genotype3));
    Model::get_genotype_db()->insert(std::make_pair(4, genotype4)); 

    person_->get_today_infections().push_back(1);
    person_->get_today_infections().push_back(2);
    person_->get_today_infections().push_back(3);
    person_->get_today_infections().push_back(4);

    person_->randomly_choose_parasite();
    EXPECT_EQ(person_->get_host_state(), Person::HostStates::EXPOSED);
    EXPECT_EQ(person_->liver_parasite_type(), genotype3);
    EXPECT_EQ(person_->get_today_infections().size(), 0);

    // should have 1 event scheduled
    EXPECT_EQ(person_->get_events().size(), 1);
    auto event = dynamic_cast<MoveParasiteToBloodEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->infection_genotype(), genotype3);
    EXPECT_EQ(event->get_time(), current_time_ + 7);
}


TEST_F(PersonParasiteTest, ScheduleMoveParasiteToBloodEvent) {
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    person_->schedule_move_parasite_to_blood(genotype, 5);
    EXPECT_EQ(person_->get_events().size(), 1);

    auto event = dynamic_cast<MoveParasiteToBloodEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->infection_genotype(), genotype);
    EXPECT_EQ(event->get_time(), current_time_ + 5);
}

TEST_F(PersonParasiteTest, ScheduleMatureGametocyteEventOver5) {
    // change age
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::AGE, _, _)).Times(1);
    // change age class
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::AGE_CLASS, _, _)).Times(1);

    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    
    auto parasite = person_->add_new_parasite_to_blood(genotype);

    person_->set_age(10);
    
    person_->schedule_mature_gametocyte_event(parasite);
    EXPECT_EQ(person_->get_events().size(), 1);

    auto event = dynamic_cast<MatureGametocyteEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->blood_parasite(), parasite);
    EXPECT_EQ(event->get_time(), current_time_ + Model::get_config()->get_epidemiological_parameters().get_days_mature_gametocyte_over_five());
}

TEST_F(PersonParasiteTest, ScheduleMatureGametocyteEventUnder5) {
    // change age
    EXPECT_CALL(*mock_population_, notify_change(_, Person::Property::AGE, _, _)).Times(1);

    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    
    auto parasite = person_->add_new_parasite_to_blood(genotype);


    person_->set_age(4);
    person_->schedule_mature_gametocyte_event(parasite);
    EXPECT_EQ(person_->get_events().size(), 1);

    auto event = dynamic_cast<MatureGametocyteEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->blood_parasite(), parasite);
    EXPECT_EQ(event->get_time(), current_time_ + Model::get_config()->get_epidemiological_parameters().get_days_mature_gametocyte_under_five());
}

TEST_F(PersonParasiteTest, ScheduleUpdateByDrugEvent) {
    auto genotype = new Genotype("aaabbbccc");
    genotype->set_genotype_id(1);
    Model::get_genotype_db()->insert(std::make_pair(1, genotype));
    auto parasite = person_->add_new_parasite_to_blood(genotype);
    person_->schedule_update_by_drug_event(parasite);

    EXPECT_EQ(person_->get_events().size(), 1);
    auto event = dynamic_cast<UpdateWhenDrugIsPresentEvent*>(person_->get_events().begin()->second.get());
    EXPECT_EQ(event->clinical_caused_parasite(), parasite);
    EXPECT_EQ(event->get_time(), current_time_ + 1);
}

TEST_F(PersonParasiteTest, InfectionFromInfectiousBite) {
    // immune system will be
    EXPECT_CALL(*mock_immune_system_, get_current_value()).WillOnce(::testing::Return(0.5));

    // Setup mock expectations for infection probability
    // EXPECT_CALL(*mock_config_, get_p_infection_from_an_infectious_bite())
    //     .WillOnce(::testing::Return(0.5));
    
    double prob = person_->p_infection_from_an_infectious_bite();

    //(1 - immune_system_->get_current_value()) / 8.333 + 0.04;
    auto expected_prob = (1 - 0.5) / 8.333 + 0.04;
    EXPECT_DOUBLE_EQ(prob, expected_prob);
} 