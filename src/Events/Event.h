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
    void operator=(const Event&) = delete;
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
private:
    virtual void execute() = 0;
};

#endif // EVENT_H