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

    Event();
    virtual ~Event();

    // Public interface
    void execute();  // Non-virtual public interface (Template Method)
    virtual const std::string name() const = 0;

    bool executable{false};
    Dispatcher* dispatcher{nullptr};
    int time{-1};

protected:
    // Protected interface for derived classes
    virtual void do_execute() = 0;  // Hook method for derived classes

private:

    // Friend declaration to allow Dispatcher to set executable state
    friend class Dispatcher;
};

#endif // EVENT_H