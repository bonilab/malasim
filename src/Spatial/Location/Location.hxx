#ifndef LOCATION_HXX
#define LOCATION_HXX
#include <spdlog/spdlog.h>

class Coordinate {
public:
    Coordinate(double latitude, double longitude) : latitude_{latitude}, longitude_{longitude} {
    }

    ~Coordinate() {
    }

    static double calculate_distance_in_km(const Coordinate &from, const Coordinate &to) {
        // using Haversine
        double p = M_PI/180;
        int R = 6371; // Radius of the Earth in km
        double dLat = p*(from.latitude_ - to.latitude_);
        double dLon = p*(from.longitude_ - to.longitude_);
        double a = sin(dLat/2)*sin(dLat/2) +
            cos(from.latitude_*p)*cos(to.latitude_*p)*sin(dLon/2)*sin(dLon/2);
        double c = 2*atan2(sqrt(a), sqrt(1 - a));
        double result = R*c;

        return result;
    }

    friend std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate) {
        spdlog::info("Coordinate: [latitude: {} - longitude: {}]", coordinate.latitude_, coordinate.longitude_);
        return os;
    }

public:
    void set_latitude(double latitude) {
        latitude_ = latitude;
    }
    double get_latitude() const {
        return latitude_;
    }
    void set_longitude(double longitude) {
        longitude_ = longitude;
    }
    double get_longitude() const {
        return longitude_;
    }

private:
    double latitude_;
    double longitude_;

};

class Location {
private:
    int id;
    int population_size;
    int district;
    double beta;
    double p_treatment_under_5;
    double p_treatment_over_5;
    Coordinate* coordinate;
    std::vector<double> age_distribution;

public:
    Location(const int id, double latitude, double longitude, const int population_size) :
        id{id}, population_size{population_size}, beta{0.0f}, p_treatment_under_5{0.0f},
        p_treatment_over_5{0.0f}, coordinate{new Coordinate(latitude, longitude)} {}

    Location(const int id, double latitude, double longitude) :
        id{id}, population_size{0}, beta{0.0f}, p_treatment_under_5{0.0f},
        p_treatment_over_5{0.0f}, coordinate{new Coordinate(latitude, longitude)} {}

    Location() {
        coordinate = new Coordinate(0, 0);
    };

    ~Location() {
        delete coordinate;
    }

    Location(const Location &org) : id{org.id}, population_size{org.population_size},
                                              beta{org.beta}, p_treatment_under_5(0), p_treatment_over_5(0),
                                              coordinate{new Coordinate(
                                                    org.coordinate->get_latitude(),
                                                    org.coordinate->get_longitude())
                                                },
    age_distribution(org.age_distribution) {}

    Location &operator=(const Location &other) {
        id = other.id;
        beta = other.beta;
        population_size = other.population_size;
        p_treatment_under_5 = other.p_treatment_under_5;
        p_treatment_over_5 = other.p_treatment_over_5;
        coordinate = new Coordinate(other.coordinate->get_latitude(),
            other.coordinate->get_longitude());
        age_distribution = other.age_distribution;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Location &location) {
        os << "id: " << location.id << ", populationSize: " << location.population_size << ", beta: " << location.beta
           << ", coordinate: " << *location.coordinate << ", age_distribution: [";

        for (auto i : location.age_distribution) {
            os << i << ",";
        }
        os << "]";
        os << ", p_treatment: [" << location.p_treatment_under_5 << "," << location.p_treatment_over_5 << "]"
           << std::endl;
        return os;
    }

    void set_id(int value) {
        id = value;
    }

    [[nodiscard]] int get_id() const {
        return id;
    }

    void set_beta(double value) {
        beta = value;
    }

    [[nodiscard]] double get_beta() const {
        return beta;
    }

    void set_population_size(int value) {
        population_size = value;
    }

    [[nodiscard]] int get_population_size() const {
        return population_size;
    }

    void set_district(int value) {
        district = value;
    }

    [[nodiscard]] int get_district() const {
        return district;
    }

    void set_age_distribution(const std::vector<double> &value) {
        age_distribution = value;
    }

    [[nodiscard]] std::vector<double> &get_age_distribution() {
        return age_distribution;
    }

    void set_p_treatment_under_5(double value) {
        p_treatment_under_5 = value;
    }

    [[nodiscard]] double get_p_treatment_under_5() const {
        return p_treatment_under_5;
    }

    void set_p_treatment_over_5(double value) {
        p_treatment_over_5 = value;
    }

    [[nodiscard]] double get_p_treatment_over_5() const {
        return p_treatment_over_5;
    }

    void set_coordinate(Coordinate* value) {
        coordinate = value;
    }

    [[nodiscard]] const Coordinate &get_coordinate() const {
        return *coordinate;
    }
};

#endif //LOCATION_HXX
