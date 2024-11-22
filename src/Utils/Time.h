#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <sstream>
#include <date/date.h>

namespace utils {

  class Time {
    public:
        // Delete copy and move constructors to ensure single instance
        Time(const Time&) = delete;
        Time& operator=(const Time&) = delete;
        Time(Time&&) = delete;
        Time& operator=(Time&&) = delete;

        // Access the singleton instance
        static Time& instance() {
            static Time instance;
            return instance;
        }

        // Get the current time in milliseconds
        long long current_time_millis() const {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        // Get the current time in seconds
        long long current_time_seconds() const {
            return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        // Get the current time in minutes
        long long current_time_minutes() const {
            return std::chrono::duration_cast<std::chrono::minutes>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        // Get the current time in hours
        long long current_time_hours() const {
            return std::chrono::duration_cast<std::chrono::hours>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        // Get the current time in days
        long long current_time_days() const {
            return std::chrono::duration_cast<std::chrono::days>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        // Get the current time in weeks
        long long current_time_weeks() const {
            return current_time_days() / 7;
        }

        // Get the current time in months
        long long current_time_months() const {
            return current_time_days() / 30;
        }

        // Get the current time in years
        long long current_time_years() const {
            return current_time_days() / 365;
        }

        // Get the current date
        date::year_month_day current_date() const {
            return date::year_month_day(date::floor<date::days>(
                std::chrono::system_clock::now()));
        }

        int get_day_count(const date::year_month_day& start_date, const date::year_month_day& end_date) const {
            return (date::sys_days{end_date} - date::sys_days{start_date}).count();
        }

        // Helper functions (merged from TimeHelpers)

        int number_of_days(const int &y1, const unsigned int &m1, const unsigned int &d1,
                           const int &y2, const unsigned int &m2, const unsigned int &d2) const {
            using namespace date;
            return (sys_days{year{y2}/month{m2}/day{d2}} - sys_days{year{y1}/month{m1}/day{d1}}).count();
        }

        int number_of_days(const date::sys_days &first, const date::sys_days &last) const {
            return (last - first).count();
        }

        template<typename T>
        T convert_to(const std::string &input) const {
            T result{};
            std::stringstream ss(input);
            date::from_stream(ss, "%Y/%m/%d", result);
            return result;
        }

        int number_of_days_to_next_year(const date::sys_days &today) const {
            const date::sys_days next_year{date::year_month_day{today} + date::years{1}};
            return number_of_days(today, next_year);
        }

        int get_simulation_time_birthday(const int &days_to_next_birthday, const int &age,
                                         const date::sys_days &starting_day) const {
            const auto calendar_birthday = date::floor<date::days>(
                starting_day + date::days{days_to_next_birthday + 1} - date::years{age + 1});
            return number_of_days(starting_day, calendar_birthday);
        }

        int day_of_year(const int &y, const unsigned &m, const unsigned &d) const {
            using namespace date;
            if (m < 1 || m > 12 || d < 1 || d > 31) return 0;
            return (sys_days{year{y}/month{m}/day{d}} - sys_days{year{y}/jan/0}).count();
        }

        int day_of_year(const date::sys_days &day) const {
            date::year_month_day ymd{day};
            return number_of_days(date::sys_days{ymd.year()/1/0}, day);
        }

        int month_of_year(const date::sys_days &day) const {
            date::year_month_day ymd{day};
            return static_cast<unsigned>(ymd.month());
        }

    private:
        // Private constructor to prevent instantiation
        Time() = default;
    };

    // Overload for printing date::sys_days
    inline std::ostream &operator<<(std::ostream &stream, const date::sys_days &o_days) {
        stream << date::year_month_day{o_days};
        return stream;
    }
}

#endif //TIME_H
