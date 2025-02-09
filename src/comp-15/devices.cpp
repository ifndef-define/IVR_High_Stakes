#include "robots/comp-15/devices.h"
#include "robots/comp-15/includeList.h"

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

pros::MotorGroup leftDrive({-9, -8, 10, -7}, pros::MotorGearset::blue);
pros::MotorGroup rightDrive({3, 4, -1, 2}, pros::MotorGearset::blue);