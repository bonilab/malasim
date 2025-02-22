/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StrategyBuilder.h
 * Author: Merlin
 *
 * Created on August 23, 2017, 11:03 AM
 */

#ifndef STRATEGYBUILDER_H
#define STRATEGYBUILDER_H

#include <yaml-cpp/yaml.h>

#include "Utils/TypeDef.hxx"

class IStrategy;

class Config;

class StrategyBuilder {
  //disallow copy and assign
  StrategyBuilder(const StrategyBuilder &) = delete;
  void operator=(const StrategyBuilder &) = delete;


 public:
  StrategyBuilder();

  virtual ~StrategyBuilder();

  static IStrategy *build(const YAML::Node &ns, const int &strategy_id);

  static void add_therapies(const YAML::Node &ns, IStrategy *result);

  static void add_distributions(const YAML::Node &ns, DoubleVector &v);

  static IStrategy *buildSFTStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildCyclingStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildAdaptiveCyclingStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildMFTStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildMFTRebalancingStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildNestedSwitchingStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildMFTMultiLocationStrategy(const YAML::Node &node, const int &id);

  static IStrategy *
  buildNestedMFTDifferentDistributionByLocationStrategy(const YAML::Node &ns, const int &strategy_id);

  static IStrategy *buildNovelDrugIntroductionStrategy(const YAML::Node &ns, const int strategy_id);
};

#endif /* STRATEGYBUILDER_H */
