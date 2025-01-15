#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <iostream>
#include "date/date.h"
#include "Utils/YamlFile.hxx"
#include <stdexcept>

class PopulationEvents {
public:
    // Inner class: EventInfo
    class EventInfo {
    public:
        // Getters and Setters
        [[nodiscard]] const date::year_month_day& get_date() const { return date_; }
        void set_date(const date::year_month_day& value) { date_ = value; }

        [[nodiscard]] int get_location_id() const { return location_id_; }
        void set_location_id(int value) { location_id_ = value; }

    private:
        date::year_month_day date_;
        int location_id_ = -1;
    };

    // Inner class: PopulationEvent
    class PopulationEvent {
    public:
        // Getters and Setters
        [[nodiscard]] const std::string& get_name() const { return name_; }
        void set_name(const std::string& value) { name_ = value; }

        [[nodiscard]] const std::vector<EventInfo>& get_info() const { return info_; }
        void set_info(const std::vector<EventInfo>& value) { info_ = value; }

    private:
        std::string name_;
        std::vector<EventInfo> info_;
    };

    // Getters and Setters for PopulationEvents
    [[nodiscard]] const std::vector<PopulationEvent>& get_events() const { return events_; }
    void set_events(const std::vector<PopulationEvent>& value) { events_ = value; }

private:
    std::vector<PopulationEvent> events_;  // Changed from std::map to std::vector
};

// Conversion functions inside namespace YAML
namespace YAML {

template <>
struct convert<PopulationEvents::EventInfo> {
    static Node encode(const PopulationEvents::EventInfo& rhs) {
        Node node;
        node["date"] = rhs.get_date();
        node["location_id"] = rhs.get_location_id();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::EventInfo& rhs) {
        if (!node["date"]) {
            throw std::runtime_error("Missing 'date' field in EventInfo.");
        }
        if (!node["location_id"]) {
            throw std::runtime_error("Missing 'location_id' field in EventInfo.");
        }

        rhs.set_date(node["date"].as<date::year_month_day>());
        rhs.set_location_id(node["location_id"].as<int>());
        return true;
    }
};

template <>
struct convert<PopulationEvents::PopulationEvent> {
    static Node encode(const PopulationEvents::PopulationEvent& rhs) {
        Node node;
        node["name"] = rhs.get_name();
        node["info"] = rhs.get_info();
        return node;
    }

    static bool decode(const Node& node, PopulationEvents::PopulationEvent& rhs) {
        if (!node["name"]) {
            throw std::runtime_error("Missing 'event_name' field in PopulationEvent.");
        }
        if (!node["info"]) {
            throw std::runtime_error("Missing 'info' field in PopulationEvent.");
        }

        rhs.set_name(node["name"].as<std::string>());
        rhs.set_info(node["info"].as<std::vector<PopulationEvents::EventInfo>>());
        return true;
    }
};

template <>
struct convert<PopulationEvents> {
    static Node encode(const PopulationEvents& rhs) {
        Node node;
        for (const auto& event : rhs.get_events()) {
            node["population_events"].push_back(event);  // Push each event into the list
        }
        return node;
    }
    static bool decode(const YAML::Node& node, PopulationEvents& rhs) {
        std::vector<PopulationEvents::PopulationEvent> events;
        for (const auto &eventNode : node["population_events"]) {
            PopulationEvents::PopulationEvent event;
            convert<PopulationEvents::PopulationEvent>::decode(eventNode, event);
            events.push_back(event);
        }
        rhs.set_events(events);
        return true;
    }
};

}
