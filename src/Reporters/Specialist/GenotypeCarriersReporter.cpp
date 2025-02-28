#include "GenotypeCarriersReporter.h"

#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include "Configuration/Config.h"
#include "Parasites/Genotype.h"
#include "Simulation/Model.h"
#include "Population/Person/Person.h"
#include "Population/Population.h"
#include "Utils/Index/PersonIndexByLocationStateAgeClass.h"
#include "spdlog/spdlog.h"

GenotypeCarriersReporter::GenotypeCarriersReporter() : db(nullptr) {}

void GenotypeCarriersReporter::initialize(int job_number, const std::string &path) {
    std::string db_path = fmt::format("{}/genotype_report_{}.db", path, job_number);

    // Open SQLite3 database
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open SQLite database: " + std::string(sqlite3_errmsg(db)));
    }

    // Create the table if it doesn't exist
    char* err_msg = nullptr;
    if (sqlite3_exec(db, CREATE_TABLE.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string error_msg = "Failed to create table: " + std::string(err_msg);
        sqlite3_free(err_msg);
        throw std::runtime_error(error_msg);
    }

    spdlog::info("Initialized GenotypeCarriersReporter with SQLite3 at {}", db_path);
}

void GenotypeCarriersReporter::monthly_report() {
    if (!db) {
        spdlog::error("Database not initialized!");
        return;
    }

    std::vector<int> genotype_count(Model::get_config()->get_spatial_settings().get_number_of_locations(), 0);

    // Cache relevant data
    auto* index = Model::get_population()->get_person_index<PersonIndexByLocationStateAgeClass>();
    auto age_classes = index->vPerson()[0][0].size();

    // Count genotype carriers
    for (auto location = 0; location < index->vPerson().size(); location++) {
        int count = 0;
        for (auto hs = 0; hs < Person::NUMBER_OF_STATE - 1; hs++) {
            for (unsigned int ac = 0; ac < age_classes; ac++) {
                auto& age_class = index->vPerson()[location][hs][ac];
                for (auto &person : age_class) {
                    auto parasites = person->get_all_clonal_parasite_populations()->parasites();
                    for (auto &parasite_population : *parasites) {
                        if (parasite_population->genotype()->get_aa_sequence()[LOCUS] == ALLELE) {
                            count++;
                            break;
                        }
                    }
                }
            }
        }
        genotype_count[location] = count;
    }

    // Insert or update data
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, INSERT_GENOTYPE_CARRIERS.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        spdlog::error("Failed to prepare SQLite statement: {}", sqlite3_errmsg(db));
        return;
    }

    int monthly_data_id = 0;
    if (sqlite3_prepare_v2(db, SELECT_LAST_ID.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            monthly_data_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    for (size_t i = 0; i < genotype_count.size(); i++) {
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1, monthly_data_id);
        sqlite3_bind_int(stmt, 2, i);
        sqlite3_bind_int(stmt, 3, genotype_count[i]);
        sqlite3_bind_int(stmt, 4, genotype_count[i]);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            spdlog::error("Failed to insert/update genotype carriers: {}", sqlite3_errmsg(db));
        }
    }

    sqlite3_finalize(stmt);
    spdlog::info("Genotype carriers data updated in SQLite3.");
}

void GenotypeCarriersReporter::after_run() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        spdlog::info("SQLite3 database closed.");
    }
}
