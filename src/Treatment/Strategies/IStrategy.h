#ifndef ISTRATEGY_H
#define ISTRATEGY_H

#include <string>
#include <utility>
#include <vector>
#include <map>

class Therapy;

class Person;

class IStrategy {
 public:

  enum StrategyType {
    SFT = 0,
    Cycling = 1,
    MFT = 2,
    AdaptiveCycling = 3,
    MFTRebalancing = 4,
    MFTMultiLocation = 5,
    NestedMFT = 6,
    NestedMFTMultiLocation = 7,
    NovelDrugIntroduction = 8
  };

  static std::map<std::string, StrategyType> StrategyTypeMap;

  //disallow copy and assign
  IStrategy(const IStrategy&) = delete;
  void operator=(const IStrategy&) = delete;

 public:
  int id{-1};
  std::string name;
  StrategyType type;
 public:

  IStrategy(std::string name, const StrategyType &type) : name{std::move(name)}, type{type} {}

  virtual ~IStrategy() = default;

  virtual bool is_strategy(const std::string &s_name) {
    return name==s_name;
  }

  virtual StrategyType get_type() const {
    return type;
  };

  virtual void add_therapy(Therapy *therapy) = 0;

  virtual Therapy *get_therapy(Person *person) = 0;

  virtual std::string to_string() const = 0;

  virtual void adjust_started_time_point(const int &current_time) = 0;

  /**
   * This function will be executed at end of time step, to check and switch therapy if needed
   */
  virtual void update_end_of_time_step() = 0;

  virtual void monthly_update() = 0;

};

#endif /* ISTRATEGY_H */
