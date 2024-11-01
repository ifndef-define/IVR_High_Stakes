#include "main.h"
#include "pnuematics.h"
#include "pros/adi.hpp"

class Pnuematic {
private:
    bool State;
    pros::ADIDigitalOut solenoid;

public:
    // Constructor to initialize the solenoid
    Pnuematic(std::uint8_t port): solenoid(port, false) {}

    void open() { 
        State = false;
        solenoid.set_value(false);  // Set solenoid to open position
    }

    void close() { 
        State = true;
        solenoid.set_value(true);  // Set solenoid to close position
    }

    void toggle() {
        State = !State; 
        solenoid.set_value(State);  // Toggle the solenoid
    }

    bool getState() { 
        return State;  // Get the state of the solenoid
    }
};