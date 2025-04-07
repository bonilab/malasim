#ifndef THERAPYBUILDER_H
#define THERAPYBUILDER_H

#include <yaml-cpp/yaml.h>

class Therapy;

class TherapyBuilder {
 public:
  TherapyBuilder();

  TherapyBuilder(const TherapyBuilder &orig) = default;

  virtual ~TherapyBuilder();

  static std::unique_ptr<Therapy> build(const YAML::Node &ns, const int &t_id);
private:
  static std::unique_ptr<Therapy> create_simple(const YAML::Node &ns, const int &t_id);
  static std::unique_ptr<Therapy> create_complex(const YAML::Node &ns, const int &t_id);

 private:

};

#endif /* THERAPYBUILDER_H */

