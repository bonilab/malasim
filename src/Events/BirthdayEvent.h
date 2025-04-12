#ifndef BIRTHDAYEVENT_H
#define BIRTHDAYEVENT_H

#include <string>

// #include "Core/ObjectPool.h"
#include "Event.h"

class Person;

class BirthdayEvent : public PersonEvent {
public:
    // Disallow copy
    BirthdayEvent(const BirthdayEvent&) = delete;
    BirthdayEvent& operator=(const BirthdayEvent&) = delete;

    // Disallow move
    BirthdayEvent(BirthdayEvent&&) = delete;
    BirthdayEvent& operator=(BirthdayEvent&&) = delete;

    explicit BirthdayEvent(Person* person) : PersonEvent(person) {}
    ~BirthdayEvent() override = default;

    // OBJECTPOOL(BirthdayEvent)

    // DELETE_COPY_AND_MOVE(BirthdayEvent)

public:
    [[nodiscard]] const std::string name() const override { return "Birthday Event"; }

private:
    void do_execute() override;
};

#endif /* BIRTHDAYEVENT_H */
