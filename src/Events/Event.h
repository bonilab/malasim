#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
public:
    // Disable copy and assignment
    Event(const Event&) = delete;
    void operator=(const Event&) = delete;
    Event(Event&&) = delete;
    Event& operator=(Event&&) = delete;

    Event() = default;
    virtual ~Event() = default;

    // Public interface
    void execute();  // Non-virtual public interface (Template Method)
    virtual const std::string name() const = 0;

    // Public state management
    bool is_executable() const { return executable_; }
    void set_executable(bool value) { executable_ = value; }
    
    int get_time() const { return time_; }
    void set_time(int value) { time_ = value; }

protected:
    // Protected interface for derived classes
    virtual void do_execute() = 0;  // Hook method for derived classes

private:
    bool executable_{false};
    int time_{-1};
};

class Person;

class PersonEvent : public Event {
public:
    PersonEvent(Person* person) : person_(person) {}

    Person* get_person() const { return person_; }
    void set_person(Person* person) { person_ = person; }
private:
    Person* person_;
};

class WorldEvent : public Event {};

#endif // EVENT_H