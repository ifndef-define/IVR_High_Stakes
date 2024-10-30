#include "main.h"
#include "clamp.h"
#include "pros/adi.hpp"

class Clamp {
private:
    bool clampState;
    pros::ADIDigitalOut solenoid;

public:
    // Constructor to initialize the solenoid
    Clamp(std::uint8_t port): solenoid(port, false) {}

    void clampOpen() { 
        clampState = false;
        solenoid.set_value(false);  // Set solenoid to open position
    }

    void clampClose() { 
        clampState = true;
        solenoid.set_value(true);  // Set solenoid to close position
    }

    void clampToggle() {
        clampState = !clampState; 
        solenoid.set_value(clampState);  // Toggle the solenoid
    }

    bool getClampState() { 
        return clampState;  // Get the state of the solenoid
    }
};