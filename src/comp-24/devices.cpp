#include "robots/comp-24/devices.h"


pros::Controller ctrler(pros::E_CONTROLLER_MASTER);

// Right Front: 3
// Right Top: 4
// Right Back Top: -1
// Right Back Bottom: 2

// Left Front: -9
// Left Top: -8
// Left Back Top: 10
// Left Back Bottom: -7

// Right Intake: -14
// Left Intake: 20

// Backpack -11

MotorGroup rightDrive({3, -15, 1, 5}, pros::MotorGears::blue);
MotorGroup leftDrive({-10, -9, 7, -8}, pros::MotorGears::blue);

PneumaticsGroup pneumatics;