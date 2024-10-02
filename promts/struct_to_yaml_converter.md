```
Here is the example of using yaml-cpp to convert yaml to Vec3 class

namespace YAML {
template<>
struct convert<Vec3> {
  static Node encode(const Vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  static bool decode(const Node& node, Vec3& rhs) {
    if(!node.IsSequence() || node.size() != 3) {
      return false;
    }

    rhs.x = node[0].as<double>();
    rhs.y = node[1].as<double>();
    rhs.z = node[2].as<double>();
    return true;
  }
};
}
Then you could use Vec3 wherever you could use any other type:

YAML::Node node = YAML::Load("start: [1, 3, 0]");
Vec3 v = node["start"].as<Vec3>();
node["end"] = Vec3(2, -1, 0);


As an expert in C++ developer, would you mind make the similar convert function for the following struct:

struct ModelSettings {
  int days_between_stdout_output;      // Frequency of stdout output, in days
  int initial_seed_number;             // Seed for random number generator
  bool record_genome_db;               // Flag to record genomic data
  date::year_month_day starting_date;  // Simulation start date (YYYY/MM/DD)
  date::year_month_day
      start_of_comparison_period;    // Start of comparison period (YYYY/MM/DD)
  date::year_month_day ending_date;  // Simulation end date (YYYY/MM/DD)
  int start_collect_data_day;        // Day to start collecting data
};
```
