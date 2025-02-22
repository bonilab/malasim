#ifndef VALIDATIONREPORTER_H
#define VALIDATIONREPORTER_H

#include <fstream>
#include <sstream>
#include "Reporter.h"

class ValidationReporter : public Reporter {
  //disallow copy, assign and move
  ValidationReporter(const ValidationReporter &) = delete;
  void operator=(const ValidationReporter &) = delete;
  ValidationReporter(ValidationReporter &&) = delete;
  void operator=(ValidationReporter &&) = delete;

public:
    const std::string group_sep = "-1111\t";
    const std::string sep = "\t";
    std::ofstream monthly_data_file;
    std::ofstream summary_data_file;
    std::ofstream gene_db_file;
    std::ofstream gene_freq_file;
    std::ofstream prmc_db_file;
    std::ofstream prmc_freq_file;
    std::ofstream monthly_mutation_file;
    std::ofstream mosquito_res_count_file;

public:
    ValidationReporter();

    ~ValidationReporter() override;

    void initialize() override;

    void before_run() override;

    void after_run() override;

    void begin_time_step() override;

    void monthly_report() override;

    void print_EIR_PfPR_by_location(std::stringstream& ss);

    //  void print_monthly_incidence_by_location();
};


#endif //VALIDATIONREPORTER_H
