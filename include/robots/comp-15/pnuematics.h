#pragma once

class Pnuematic
{
private:
    bool State;
    pros::ADIDigitalOut solenoid;
    // true = pnuematic is closed
    // false = pnuematic is open
public:
    Pnuematic(std::uint8_t port, bool enabled);
    void open();
    void close();
    void toggle();
    bool getState();
};


