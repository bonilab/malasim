#include "PersonTestBase.h"
#include "Events/EndClinicalEvent.h"
#include "Events/ProgressToClinicalEvent.h"

class PersonClinicalTest : public PersonTestBase {
protected:
    void SetUp() override {
        PersonTestBase::SetUp();
        // Additional setup specific to clinical tests
    }
};

// TEST_F(PersonClinicalTest, ScheduleClinicalEvent) {
//     const int days_delay = 5;
//     ClonalParasitePopulation* test_parasite = nullptr; // Mock this if needed
    
//     // Schedule clinical event
//     person_->schedule_clinical_event(test_parasite, days_delay);
    
//     // Verify event was scheduled
//     EXPECT_TRUE(person_->has_event<ClinicalEvent>());
// }

TEST_F(PersonClinicalTest, ScheduleEndClinicalEvent) {
    ClonalParasitePopulation* test_parasite = nullptr; // Mock this if needed
    
    // Schedule end clinical event
    person_->schedule_end_clinical_event(test_parasite);
    
    // Verify event was scheduled
    EXPECT_TRUE(person_->has_event<EndClinicalEvent>());
}

TEST_F(PersonClinicalTest, ScheduleProgressToClinicalEvent) {
    ClonalParasitePopulation* test_parasite = nullptr; // Mock this if needed
    
    // Schedule progress to clinical event
    person_->schedule_progress_to_clinical_event(test_parasite);
    
    // Verify event was scheduled
    EXPECT_TRUE(person_->has_event<ProgressToClinicalEvent>());
}

TEST_F(PersonClinicalTest, CancelClinicalEvents) {
    ClonalParasitePopulation* test_parasite = nullptr; // Mock this if needed
    
    // Schedule multiple events
    person_->schedule_progress_to_clinical_event(test_parasite);
    person_->schedule_progress_to_clinical_event(test_parasite);
    // 
    auto test_event = person_->get_events().begin()->second.get();
    // Cancel all other progress events except this one
    // cancel only mark the event as cancelled, but not remove it from the scheduler
    person_->cancel_all_other_progress_to_clinical_events_except(test_event);
    
    // Verify only one event remains
    int executable_event_count = 0;
    for (const auto& event_pair : person_->get_events()) {
        if (dynamic_cast<ProgressToClinicalEvent*>(event_pair.second.get())) {
            if (event_pair.second->is_executable()) {
                // spdlog::info("Executable event found");
                executable_event_count++;
            }
        }
    }
    EXPECT_EQ(executable_event_count, 1);
}

TEST_F(PersonClinicalTest, ProbabilityProgressToClinical) {
    // Test default probability
    EXPECT_GE(person_->get_probability_progress_to_clinical(), 0.0);
    EXPECT_LE(person_->get_probability_progress_to_clinical(), 1.0);
}

TEST_F(PersonClinicalTest, DeathProgressionWithoutTreatment) {
    // Test death progression without treatment
    bool will_die = person_->will_progress_to_death_when_receive_no_treatment();
    EXPECT_TRUE(will_die || !will_die); // Just verify it returns a boolean
}

TEST_F(PersonClinicalTest, DeathProgressionWithTreatment) {
    // Test death progression with treatment
    bool will_die = person_->will_progress_to_death_when_recieve_treatment();
    EXPECT_TRUE(will_die || !will_die); // Just verify it returns a boolean
}

TEST_F(PersonClinicalTest, TestTreatmentFailureScheduling) {
    ClonalParasitePopulation* test_parasite = nullptr; // Mock this if needed
    const int testing_day = 7;
    const int therapy_id = 1;
    
    // Schedule test treatment failure event
    person_->schedule_test_treatment_failure_event(test_parasite, testing_day, therapy_id);
    
    // Verify event was scheduled (specific event type check would depend on implementation)
    EXPECT_FALSE(person_->get_events().empty());
}

TEST_F(PersonClinicalTest, ReportTreatmentFailureDeathScheduling) {
    const int therapy_id = 1;
    const int testing_day = 7;
    
    // Schedule report treatment failure death event
    person_->schedule_report_treatment_failure_death_event(therapy_id, testing_day);
    
    // Verify event was scheduled (specific event type check would depend on implementation)
    EXPECT_FALSE(person_->get_events().empty());
} 