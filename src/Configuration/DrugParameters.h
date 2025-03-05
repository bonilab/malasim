#ifndef DRUGPARAMETERS_H
#define DRUGPARAMETERS_H
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include "IConfigData.h"
#include "Utils/Helpers/NumberHelpers.hxx"
#include "Treatment/Therapies/DrugDatabase.h"

class DrugParameters: public IConfigData {
public:
    DrugDatabase* drug_db = new DrugDatabase();
    // Inner class: DrugInfo
    class DrugInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_name() const { return name_; }
        void set_name(const std::string& value) { name_ = value; }

        [[nodiscard]] double get_half_life() const { return half_life_; }
        void set_half_life(double value) { half_life_ = value; }

        [[nodiscard]] double get_maximum_parasite_killing_rate() const { return maximum_parasite_killing_rate_; }
        void set_maximum_parasite_killing_rate(double value) { maximum_parasite_killing_rate_ = value; }

        [[nodiscard]] int get_n() const { return n_; }
        void set_n(int value) { n_ = value; }

        [[nodiscard]] const std::vector<double>& get_age_specific_drug_concentration_sd() const { return age_specific_drug_concentration_sd_; }
        void set_age_specific_drug_concentration_sd(const std::vector<double>& value) { age_specific_drug_concentration_sd_ = value; }

        [[nodiscard]] const std::vector<double>& get_age_specific_drug_absorption() const { return age_specific_drug_absorption_; }
        void set_age_specific_drug_absorption(const std::vector<double>& value) { age_specific_drug_absorption_ = value; }

        [[nodiscard]] int get_k() const { return k_; }
        void set_k(int value) { k_ = value; }

        [[nodiscard]] double get_base_EC50() const { return base_EC50_; }
        void set_base_EC50(double value) { base_EC50_ = value; }

    private:
        std::string name_;
        double half_life_ = -1;
        double maximum_parasite_killing_rate_ = -1;
        int n_ = -1;
        std::vector<double> age_specific_drug_concentration_sd_{};
        std::vector<double> age_specific_drug_absorption_{};
        int k_ = -1;
        double base_EC50_ = -1;
    };

    // Getters and Setters for DrugParameters
    [[nodiscard]] const std::map<int, DrugInfo>& get_drug_db_raw() const { return drug_infos_; }
    void set_drug_db_raw(const std::map<int, DrugInfo>& value) { drug_infos_ = value; }

    //process config data
    void process_config() override {
      spdlog::info("Processing DrugParameters");
      for (auto drug_id = 0; drug_id < drug_infos_.size(); drug_id++) {
        auto *dt = new DrugType();
        dt->set_id(drug_id);

        const auto i_s = NumberHelpers::number_to_string<int>(drug_id);
        const auto &dt_node = drug_infos_.at(drug_id);

        dt->set_name(dt_node.get_name());
        dt->set_drug_half_life(dt_node.get_half_life());
        dt->set_maximum_parasite_killing_rate(dt_node.get_maximum_parasite_killing_rate());
        dt->set_n(dt_node.get_n());
        //    dt->set_EC50(node["EC50"].as<double>());

        //    std::cout <<dt->drug_half_life() << "-" << dt->maximum_parasite_killing_rate() << "-" << dt->n() << "-" <<
        //    dt->EC50() << std::endl;
        for (std::size_t i = 0; i < dt_node.get_age_specific_drug_concentration_sd().size(); i++) {
          dt->age_group_specific_drug_concentration_sd().push_back(
              dt_node.get_age_specific_drug_concentration_sd()[i]);
          dt->age_specific_drug_absorption().push_back(1.0);
        }
        //    assert(dt->age_group_specific_drug_concentration_sd().size() == 15);

        if (!dt_node.get_age_specific_drug_absorption().empty()) {
          dt->set_age_specific_drug_absorption(std::vector<double>(dt_node.get_age_specific_drug_absorption()));
        }

        dt->set_k(dt_node.get_k());

        dt->base_EC50 = dt_node.get_base_EC50();

        dt->populate_resistant_aa_locations();

        drug_db->add(dt);
      }
    };

private:
    std::map<int, DrugInfo> drug_infos_;
};

namespace YAML {

// DrugParameters::DrugInfo YAML conversion
template<>
struct convert<DrugParameters::DrugInfo> {
    static Node encode(const DrugParameters::DrugInfo& rhs) {
        Node node;
        node["name"] = rhs.get_name();
        node["half_life"] = rhs.get_half_life();
        node["maximum_parasite_killing_rate"] = rhs.get_maximum_parasite_killing_rate();
        node["n"] = rhs.get_n();
        node["age_specific_drug_concentration_sd"] = rhs.get_age_specific_drug_concentration_sd();
        node["age_specific_drug_absorption"] = rhs.get_age_specific_drug_absorption();
        node["k"] = rhs.get_k();
        node["base_EC50"] = rhs.get_base_EC50();
        return node;
    }

    static bool decode(const Node& node, DrugParameters::DrugInfo& rhs) {
        if (!node["name"] || !node["half_life"] || !node["maximum_parasite_killing_rate"] || !node["n"]
            || !node["age_specific_drug_concentration_sd"] || !node["k"] || !node["base_EC50"]) {
            throw std::runtime_error("Missing fields in DrugParameters::DrugInfo");
        }
        rhs.set_name(node["name"].as<std::string>());
        rhs.set_half_life(node["half_life"].as<double>());
        rhs.set_maximum_parasite_killing_rate(node["maximum_parasite_killing_rate"].as<double>());
        rhs.set_n(node["n"].as<int>());
        rhs.set_age_specific_drug_concentration_sd(node["age_specific_drug_concentration_sd"].as<std::vector<double>>());
        if(node["age_specific_drug_absorption"])
            rhs.set_age_specific_drug_absorption(node["age_specific_drug_absorption"].as<std::vector<double>>());
        rhs.set_k(node["k"].as<int>());
        rhs.set_base_EC50(node["base_EC50"].as<double>());
        return true;
    }
};

// DrugParameters YAML conversion
template<>
struct convert<DrugParameters> {
    static Node encode(const DrugParameters& rhs) {
        Node node;
        for (const auto& [key, value] : rhs.get_drug_db_raw()) {
            node["drug_db"][key] = value;
        }
        return node;
    }

    static bool decode(const Node& node, DrugParameters& rhs) {
        if (!node["drug_db"]) {
            throw std::runtime_error("Missing 'drug_db' field in DrugParameters");
        }
        std::map<int, DrugParameters::DrugInfo> drug_db;
        for (const auto& element : node["drug_db"]) {
            int key = element.first.as<int>();
            drug_db[key] = element.second.as<DrugParameters::DrugInfo>();
        }
        rhs.set_drug_db_raw(drug_db);
        return true;
    }
};

}  // namespace YAML

#endif //DRUGPARAMETERS_H
