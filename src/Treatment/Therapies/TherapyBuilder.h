#ifndef THERAPYBUILDER_H
#define THERAPYBUILDER_H

#include <yaml-cpp/yaml.h>

class Therapy;

class TherapyBuilder {
 public:
  TherapyBuilder();

  TherapyBuilder(const TherapyBuilder &orig) = default;

  virtual ~TherapyBuilder();

  static Therapy *build(const YAML::Node &ns, const int &t_id);

 private:

};

#endif /* THERAPYBUILDER_H */

