#ifndef YAMLFILE_H
#define YAMLFILE_H

#include <yaml-cpp/yaml.h>
#include <date/date.h>
#include <sstream>
#include <string>

namespace YAML {
    template <>
    struct convert<date::year_month_day> {
        static ::YAML::Node encode(const date::year_month_day &rhs) {
            std::stringstream ss;
            ss << date::format("%Y/%m/%d", rhs);
            return ::YAML::Node(ss.str());
        }

        static bool decode(const ::YAML::Node &node, date::year_month_day &rhs) {
            if (!node.IsScalar()) {
                throw std::runtime_error("Invalid date format: not a scalar.");
            }

            std::stringstream ss(node.as<std::string>());
            date::year_month_day ymd{};
            ss >> date::parse("%Y/%m/%d", ymd);

            if (ss.fail()) {
                throw std::runtime_error("Invalid date format: failed to parse (should be yyyy/mm/dd).");
            }

            rhs = ymd;
            return true;
        }
    };
} // namespace YAML

#endif // YAMLFILE_H
