//
// Created by Kien Tran on 3/17/25.
//

#ifndef SEASONALPATTERN_H
#define SEASONALPATTERN_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <sstream>
#include <spdlog/spdlog.h>
#include <limits>
#include "SeasonalInfo.h"
#include "Utils/Helpers/TimeHelpers.h"
#include "Spatial/GIS/SpatialData.h"

class SeasonalPattern : public ISeasonalInfo {
public:
  SeasonalPattern();
  void build();
  void read(const std::string &filename);
  double get_seasonal_factor(const date::sys_days &today, const int &location) override;

  int get_period() const {
    return period;
  }

  void set_period(int period) {
    this->period = period;
  }

  bool get_is_monthly() const {
    return is_monthly;
  }

  void set_is_monthly(bool is_monthly) {
    this->is_monthly = is_monthly;
  }

  int get_min_district_id() const {
    return min_district_id;
  }

  void set_min_district_id(int min_district_id) {
    this->min_district_id = min_district_id;
  }

  int get_max_district_id() const {
    return max_district_id;
  }

  void set_max_district_id(int max_district_id) {
    this->max_district_id = max_district_id;
  }

  std::string get_filename() const {
    return filename;
  }

  void set_filename(const std::string &filename) {
    this->filename = filename;
  }

private:
  std::vector<std::vector<double>> district_adjustments;
  int period;
  bool is_monthly;
  int min_district_id;
  int max_district_id;
  std::string filename;
  virtual int get_district_for_location(int location) const;
};

#endif //SEASONALPATTERN_H
