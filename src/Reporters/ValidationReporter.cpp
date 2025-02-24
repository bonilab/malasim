#include "ValidationReporter.h"

#include <date/date.h>
#include <iomanip>

#include "Utils/Constants.h"
#include "Configuration/Config.h"
#include "Core/Scheduler/Scheduler.h"
#include "Utils/Random.h"
#include "MDC/ModelDataCollector.h"
#include "Simulation/Model.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "Utility/ReporterUtils.h"
#include "Mosquito/Mosquito.h"
#include "Parasites/Genotype.h"
#include "Treatment/ITreatmentCoverageModel.h"
#include "Treatment/Therapies/SCTherapy.h"
#include "Utils/Cli.hxx"

ValidationReporter::ValidationReporter() = default;

ValidationReporter::~ValidationReporter() = default;

void ValidationReporter::initialize(int job_number, const std::string &path) {
    monthly_data_file.open(fmt::format("{}/validation_monthly_data_{}.txt", path, job_number));
    summary_data_file.open(fmt::format("{}/validation_summary_{}.txt", path, job_number));
    gene_freq_file.open(fmt::format("{}/validation_gene_freq_{}.txt", path, job_number));
    gene_db_file.open(fmt::format("{}/validation_gene_db_{}.txt", path, job_number));
    prmc_freq_file.open(fmt::format("{}/validation_prmc_freq_{}.txt", path, job_number));
    prmc_db_file.open(fmt::format("{}/validation_prmc_db_{}.txt", path, job_number));
    monthly_mutation_file.open(fmt::format("{}/validation_monthly_mutation_{}.txt", path, job_number));
    mosquito_res_count_file.open(fmt::format("{}/validation_mosquito_res_count_{}.txt", path, job_number));
}

void ValidationReporter::before_run() {}

void ValidationReporter::begin_time_step() {}

void ValidationReporter::monthly_report() {
    std::stringstream ss;

    ss << Model::get_instance().get_scheduler()->current_time() << sep;
    ss << std::chrono::system_clock::to_time_t(Model::get_instance().get_scheduler()->calendar_date) << sep;
    ss << date::format("%Y\t%m\t%d", Model::get_instance().get_scheduler()->calendar_date) << sep;
    ss << Model::get_instance().get_config()->get_seasonality_settings().get_seasonal_factor(Model::get_instance().get_scheduler()->calendar_date, 0) << sep;
    ss << Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(0, 1) << sep;
    ss << Model::get_instance().get_treatment_coverage()->get_probability_to_be_treated(0, 10) << sep;
    ss << Model::get_instance().get_population()->size() << sep;
    ss << group_sep;//9 - index 0
    print_EIR_PfPR_by_location(ss);//11
    ss << group_sep;//15
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->monthly_number_of_new_infections_by_location()[loc] << sep;
        ss << group_sep;//17
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->monthly_number_of_treatment_by_location()[loc] << sep; //Incidence
        ss << group_sep;//19
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->monthly_number_of_clinical_episode_by_location()[loc] << sep;
        ss << group_sep;///21
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for(auto moi : Model::get_instance().get_mdc()->multiple_of_infection_by_location()[loc]){
            ss << moi << sep;
        }
        ss << group_sep;///32
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (auto ac = 0ul; ac < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ac++){
            ss << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location_age_group()[loc][ac] << sep;
        }
        ss << group_sep;//48
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (int age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location_age()[loc][age] << sep;
        }
        ss << group_sep;///129
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->current_TF_by_location()[loc] << sep;
    }
    ss << group_sep;//131
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->monthly_number_of_mutation_events_by_location()[loc] << sep;
        ss << group_sep;//133
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (auto ac = 0ul; ac < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ac++){
            ss << Model::get_instance().get_mdc()->number_of_treatments_by_location_age_year()[loc][ac] << sep;
        }
        ss << group_sep;//149
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->total_number_of_bites_by_location()[loc] << sep;
    }
    ss << group_sep;//151
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->total_number_of_bites_by_location_year()[loc] << sep;
        ss << group_sep;//153
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->today_number_of_treatments_by_location()[loc] << sep;
        ss << group_sep;//155
    }
    ss << Model::get_instance().get_mdc()->current_number_of_mutation_events_in_this_year() << sep;
    ss << group_sep;//157
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        if ((Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::ASYMPTOMATIC] + Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::CLINICAL]) == 0){
            ss << 0 << sep;
        }
        else{
            ss << Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::ASYMPTOMATIC] /
                  (Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::ASYMPTOMATIC] + Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::CLINICAL])  << sep;
        }
        ss << group_sep;//159
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (auto age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->popsize_by_location_age()[loc][age] << sep;
        }
        ss << group_sep;//240
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (auto age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->monthly_number_of_clinical_episode_by_location_age()[loc][age] << sep;
        }
        ss << group_sep;//321
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (auto ac = 0; ac < Model::get_instance().get_config()->get_population_demographic().get_number_of_age_classes(); ac++){
            int all_infected_pop = Model::get_instance().get_mdc()->popsize_by_location_hoststate_age_class()[loc][Person::ASYMPTOMATIC][ac]
                                 + Model::get_instance().get_mdc()->popsize_by_location_hoststate_age_class()[loc][Person::CLINICAL][ac];
            if (all_infected_pop == 0){
                ss << 0 << sep;
            }
            else{
                ss << std::setprecision(6) << Model::get_instance().get_mdc()->number_of_clinical_by_location_age_group()[loc][ac] / (double) all_infected_pop<< sep;
            }
        }
        ss << group_sep;//337
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->total_immune_by_location()[loc] / Model::get_instance().get_population()->size_at(loc) << sep;
        ss << group_sep;//339
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        int all_infected_pop = Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::ASYMPTOMATIC]
                               + Model::get_instance().get_mdc()->popsize_by_location_hoststate()[loc][Person::CLINICAL];
        if (all_infected_pop == 0){
            ss << 0 << sep;
        }
        else{
            ss << std::setprecision(6) << Model::get_instance().get_mdc()->monthly_number_of_clinical_episode_by_location()[loc] / (double) all_infected_pop<< sep;
        }
        ss << group_sep;//341
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_NTF_by_location()[loc] << sep;
        ss << group_sep;//343
    }
    for (auto tf_by_therapy : Model::get_instance().get_mdc()->current_tf_by_therapy()) {
        ss << tf_by_therapy << sep;
    }
    ss << group_sep;//358
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->monthly_number_of_TF_by_location()[loc] << sep;
        ss << group_sep;//360
    }
    for (int t_id = 0; t_id < Model::get_instance().get_config()->get_therapy_parameters().therapy_db.size(); t_id++) {
        int nTreaments = Model::get_instance().get_mdc()->number_of_treatments_with_therapy_ID()[t_id];
        int nSuccess = Model::get_instance().get_mdc()->number_of_treatments_success_with_therapy_ID()[t_id];
        int nFail = Model::get_instance().get_mdc()->number_of_treatments_fail_with_therapy_ID()[t_id];
        double pSuccess = (nTreaments == 0) ? 0 : nSuccess * 100.0 / nTreaments;
        ss << nTreaments << sep << nSuccess << sep << nFail << sep << pSuccess << sep;
    }
    ss << group_sep;//417
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_number_treatments_by_location()[loc] << sep;
        ss << group_sep;//419
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_TF_by_location()[loc] << sep;
        ss << group_sep;//421
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location()[loc] << sep;
        ss << group_sep;//423
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (int age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->number_of_untreated_cases_by_location_age_year()[loc][age] << sep;
        }
        ss << group_sep;///504
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (int age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->number_of_deaths_by_location_age_year()[loc][age] << sep;
        }
        ss << group_sep;///585
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (int age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->number_of_malaria_deaths_treated_by_location_age_year()[loc][age] << sep;
        }
        ss << group_sep;///666
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        for (int age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->number_of_malaria_deaths_non_treated_by_location_age_year()[loc][age] << sep;
        }
        ss << group_sep;///747
    }
    monthly_data_file << ss.str() << std::endl;

    std::stringstream gene_freq_ss;
//    ReporterUtils::output_genotype_frequency3(gene_freq_ss, Model::get_instance().get_config()->get_genotype_parameters().genotype_db.size(),
//                                              Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());
    std::stringstream prmc_freq_ss;
    ReporterUtils::output_genotype_frequency4(gene_freq_ss, prmc_freq_ss, Model::get_instance().get_config()->get_genotype_parameters().genotype_db.size(),
                                              Model::get_instance().get_population()->get_person_index<PersonIndexByLocationStateAgeClass>());

    gene_freq_file << gene_freq_ss.str() << std::endl;
    prmc_freq_file << prmc_freq_ss.str() << std::endl;

    ss.str("");
    int sum = 0;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        sum += Model::get_instance().get_mdc()->mutation_tracker[loc].size();
        for (int i = 0; i < Model::get_instance().get_mdc()->mutation_tracker[loc].size(); i++) {
            ss << std::get<0>(Model::get_instance().get_mdc()->mutation_tracker[loc][i]) << sep;
            ss << std::get<1>(Model::get_instance().get_mdc()->mutation_tracker[loc][i]) << sep;
            ss << std::get<2>(Model::get_instance().get_mdc()->mutation_tracker[loc][i]) << sep;
            ss << std::get<3>(Model::get_instance().get_mdc()->mutation_tracker[loc][i]) << sep;
            ss << std::get<4>(Model::get_instance().get_mdc()->mutation_tracker[loc][i]) << sep;
            ss << std::get<5>(Model::get_instance().get_mdc()->mutation_tracker[loc][i]) << '\n';
        }
    }
    if(sum > 0){
        monthly_mutation_file << ss.str() << std::endl;
        for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
            Model::get_instance().get_mdc()->mutation_tracker[loc].clear();
        }
    }

    ss.str("");
    sum = 0;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        sum += Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc].size();
        for (int i = 0; i < Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc].size(); i++) {
            ss << std::get<0>(Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc][i]) << sep;
            ss << std::get<1>(Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc][i]) << sep;
            ss << std::get<2>(Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc][i]) << sep;
            ss << std::get<3>(Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc][i]) << '\n';
        }
    }
    if(sum > 0){
        mosquito_res_count_file << ss.str() << std::endl;
        for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
            Model::get_instance().get_mdc()->mosquito_recombined_resistant_genotype_tracker[loc].clear();
        }
    }
}

void ValidationReporter::after_run() {
    std::stringstream ss;

    ss.str("");
    ss << Model::get_instance().get_random()->get_seed() << sep << Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations() << sep;
    ss << Model::get_instance().get_config()->get_spatial_settings().location_db[0].beta << sep;
    ss << Model::get_instance().get_config()->get_spatial_settings().location_db[0].population_size << sep;
    print_EIR_PfPR_by_location(ss);
    ss << group_sep;//9
    // output last strategy information
    ss << Model::get_instance().get_treatment_strategy()->id << sep;//10
    // output NTF
    const auto total_time_in_years = (Model::get_instance().get_scheduler()->current_time()
                                     - Model::get_instance().get_config()->get_simulation_timeframe().get_start_of_comparison_period())
                                     / static_cast<double>(Constants::DAYS_IN_YEAR);
    auto sum_ntf = 0.0;
    ul pop_size = 0;
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        sum_ntf += Model::get_instance().get_mdc()->cumulative_NTF_by_location()[loc];
        pop_size += Model::get_instance().get_mdc()->popsize_by_location()[loc];
    }
    ss << (sum_ntf * 100 / pop_size) / total_time_in_years << sep;
    ss << group_sep;//12
    for (int loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location()[loc] << sep;
//        std::cout << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location()[loc] << "\t";
        ss << group_sep;//14
    }
    for (int loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->percentage_bites_on_top_20_by_location()[loc] * 100 << "%"  << sep;
//        std::cout << Model::get_instance().get_mdc()->percentage_bites_on_top_20_by_location()[loc] * 100 << "%" << "\t";
        ss << group_sep;//16
    }
    for (int loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        double location_NTF = Model::get_instance().get_mdc()->cumulative_NTF_by_location()[loc] * 100 /
                              (double) Model::get_instance().get_mdc()->popsize_by_location()[loc];
        location_NTF /= total_time_in_years;
        ss << location_NTF << sep;
//        std::cout << location_NTF << "\t";
        ss << group_sep;//18
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++)
    {
        for (auto age = 0; age < 80; age++){
            ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location_age()[loc][age]/total_time_in_years/Model::get_instance().get_mdc()->popsize_by_location_age()[loc][age] << sep;
        }
        ss << group_sep;//99
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_number_treatments_by_location()[loc] << sep;
        ss << group_sep;//101
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_TF_by_location()[loc] << sep;
        ss << group_sep;//103
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->cumulative_clinical_episodes_by_location()[loc] << sep;
        ss << group_sep;//105
    }
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); loc++) {
        ss << Model::get_instance().get_mdc()->mosquito_recombination_events_count()[loc][0] << sep;
        ss << Model::get_instance().get_mdc()->mosquito_recombination_events_count()[loc][1] << sep;
        ss << group_sep;//107
    }
    summary_data_file << ss.str() << std::endl;

    for (auto [g_id, genotype] : Model::get_instance().get_config()->get_genotype_parameters().genotype_db) {
        gene_db_file << g_id << sep << genotype->aa_sequence << std::endl;
        prmc_db_file << g_id << sep << genotype->aa_sequence << std::endl;
    }

    for (auto [g_id, genotype] : Model::get_instance().get_config()->get_genotype_parameters().genotype_db) {
        spdlog::debug("{}:{}",genotype->aa_sequence,genotype->daily_fitness_multiple_infection);
    }
    for(int resistant_drug_pair_id = 0; resistant_drug_pair_id < Model::get_instance().get_mosquito()->resistant_drug_list.size(); resistant_drug_pair_id++){
        auto drugs = Model::get_instance().get_mosquito()->resistant_drug_list[resistant_drug_pair_id].second;
        for (auto [g_id, genotype] : Model::get_instance().get_config()->get_genotype_parameters().genotype_db) {
            if(resistant_drug_pair_id < 3){
                spdlog::debug(fmt::format("resistant_drug_pair_id: {} {}\tR-0: {}\tR-1: {}\tEC50-0: {}\tEC50-1: {}\tminEC50-0: {}\tminEC50-1: {}",
                                       resistant_drug_pair_id,
                                       genotype->aa_sequence,
                                       genotype->resist_to(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[0])),
                                       genotype->resist_to(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])),
                                       genotype->EC50_power_n[drugs[0]],
                                       genotype->EC50_power_n[drugs[1]],
                                       pow(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[0])->base_EC50, Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[0])->n()),
                                       pow(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])->base_EC50, Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])->n())));
            }
            else{
              spdlog::debug(fmt::format("resistant_drug_pair_id: {} {}\tR-0: {}\tR-1: {}\tR-2: {}\tEC50-0: {}\tEC50-1: {}\tEC50-2: {}\tminEC50-0: {}\tminEC50-1: {}\tminEC50-2: {}",
                                       resistant_drug_pair_id,
                                       genotype->aa_sequence,
                                       genotype->resist_to(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[0])),
                                       genotype->resist_to(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])),
                                       genotype->resist_to(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[2])),
                                       genotype->EC50_power_n[drugs[0]],
                                       genotype->EC50_power_n[drugs[1]],
                                       genotype->EC50_power_n[drugs[2]],
                                       pow(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[0])->base_EC50, Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[0])->n()),
                                       pow(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])->base_EC50, Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])->n()),
                                       pow(Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[1])->base_EC50, Model::get_instance().get_config()->get_drug_parameters().drug_db->at(drugs[2])->n())));
            }
        }
        spdlog::debug("###############");
    }

    gene_db_file.close();
    gene_freq_file.close();
    prmc_db_file.close();
    prmc_freq_file.close();
    monthly_data_file.close();
    summary_data_file.close();
    monthly_mutation_file.close();
    mosquito_res_count_file.close();
}

void ValidationReporter::print_EIR_PfPR_by_location(std::stringstream& ss) {
    for (auto loc = 0; loc < Model::get_instance().get_config()->get_spatial_settings().get_number_of_locations(); ++loc) {
        //
        // EIR
        if (Model::get_instance().get_mdc()->EIR_by_location_year()[loc].empty()) {
            ss << 0 << sep;
        } else {
            ss << Model::get_instance().get_mdc()->EIR_by_location_year()[loc].back() << sep;
        }
        ss << group_sep;//11
        // pfpr <5 , 2-10 and all
        ss << Model::get_instance().get_mdc()->get_blood_slide_prevalence(loc, 2, 10) * 100 << sep;
        ss << Model::get_instance().get_mdc()->get_blood_slide_prevalence(loc, 0, 5) * 100 << sep;
        ss << Model::get_instance().get_mdc()->blood_slide_prevalence_by_location()[loc] * 100 << sep;
    }
}