#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/action.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

void initialize() {
	pros::lcd::initialize();
	imu.reset(true);
	
	actions.setRingColor(ringToKeep);
	odom::start(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	auton(ringToKeep);
	// redAuton1();
}

void opcontrol() {
	// autonomous();
	teleOp(ringToKeep);
}