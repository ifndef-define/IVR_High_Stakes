#include "main.h"
#include "comp-15/pnuematics.h"
#include "pros/adi.hpp"


// Constructor to initialize the solenoid
Pnuematic::Pnuematic(std::uint8_t port, bool enabled): solenoid(port, enabled){

}

void Pnuematic::open() { 
    State = false;
    solenoid.set_value(false);  // Set solenoid to open position
}

void Pnuematic::close() { 
    State = true;
    solenoid.set_value(true);  // Set solenoid to close position
}

void Pnuematic::toggle() {
    State = !State; 
    solenoid.set_value(State);  // Toggle the solenoid
}

bool Pnuematic::getState() { 
    return State;  // Get the state of the solenoid
}