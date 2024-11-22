#pragma once
#include "robots/comp-15/devices.h"

class Pnuematic
{
private:
    bool State;
    pros::adi::Pneumatics *solenoid;
    // true = pnuematic is closed
    // false = pnuematic is open
public:
    Pnuematic(pros::adi::Pneumatics *piston);
    void open();
    void close();
    void toggle();
    bool getState();
};


