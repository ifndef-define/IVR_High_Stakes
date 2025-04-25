#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/action.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

void initialize() {
	pros::lcd::initialize();
	
	imu.reset(true);
	actions.setRingColor(ringToKeep);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	// auton(ringToKeep);
}

void opcontrol() {
	teleOp(ringToKeep, false);
}