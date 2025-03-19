#include "SeasonalPattern.h"

SeasonalPattern::SeasonalPattern() : period(12), is_monthly(true), min_district_id(-1), max_district_id(-1) {}

void SeasonalPattern::build() {
  read(filename);
}

double SeasonalPattern::get_seasonal_factor(const date::sys_days &today, const int &location) {
    int district = get_district_for_location(location);
    int doy = TimeHelpers::day_of_year(today);
    auto ymd = date::year_month_day{today};
    int month = static_cast<unsigned>(ymd.month()) - 1;
    if (is_monthly) {
        return district_adjustments[district][month];
    } else {
        doy = (doy == 366) ? 364 : doy - 1;
        return district_adjustments[district][doy];
    }
}

void SeasonalPattern::read(const std::string &filename) {
    std::ifstream in(filename);
    if (!in.good()) {
        throw std::runtime_error("Error opening the seasonal pattern file: " + filename);
    }
    std::string line;
    std::getline(in, line);
    min_district_id = std::numeric_limits<int>::max();
    max_district_id = std::numeric_limits<int>::min();
    std::map<int, std::vector<double>> temp_adjustments;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string token;
        std::getline(ss, token, ',');
        int district_id = std::stoi(token);
        min_district_id = std::min(min_district_id, district_id);
        max_district_id = std::max(max_district_id, district_id);
        std::vector<double> factors;
        while (std::getline(ss, token, ',')) {
            double factor = std::stod(token);
            if (factor < 0.0) {
                throw std::runtime_error("Seasonal factor less than zero: " + std::to_string(factor));
            }
            factors.push_back(factor);
        }
        if (factors.size() != period) {
            throw std::runtime_error("Incorrect number of seasonal factors in file.");
        }
        temp_adjustments[district_id] = factors;
    }
    int actual_district_count = max_district_id - min_district_id + 1;
    district_adjustments.resize(min_district_id == 0 ? actual_district_count : actual_district_count + 1);
    for (const auto &[file_id, factors] : temp_adjustments) {
        district_adjustments[file_id] = factors;
    }
    spdlog::info("Loaded {} districts from {}.", actual_district_count, filename);
}

int SeasonalPattern::get_district_for_location(int location) const {
    if (SpatialData::get_instance().district_count == -1) {
        return min_district_id;
    }
    return SpatialData::get_instance().get_district(location);
}