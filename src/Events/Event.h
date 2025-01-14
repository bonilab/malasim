#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <uuid.h>

class Dispatcher;
class Scheduler;

namespace utils {
    class Indexer;
}

class Event {
public:
    // Disable copy and assignment
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;
    Event(Event&&) = delete;
    Event& operator=(Event&&) = delete;

    bool executable{false};

    Event();
    virtual ~Event();

    Scheduler* scheduler;
    Dispatcher* dispatcher{nullptr};
    void perform_execute();
    virtual std::string name() = 0;
    int time{-1};
    void set_id(uuids::uuid id) { id_ = id; }
    [[nodiscard]] uuids::uuid get_id() const { return id_; }
    [[nodiscard]] std::string get_id_str() const { return to_string(id_).substr(to_string(id_).length()-8, 8); }


private:
    virtual void execute() = 0;
    uuids::uuid id_;
};

#endif // EVENT_H