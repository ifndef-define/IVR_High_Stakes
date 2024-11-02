#pragma once
#include "comp-15/devices.cpp"

class Pnuematic
{
private:
    bool State;
    pros::adi::Pnuematics *solenoid;
    // true = pnuematic is closed
    // false = pnuematic is open
public:
    Pnuematic(std::uint8_t port, bool enabled);
    void open();
    void close();
    void toggle();
    bool getState();
};


