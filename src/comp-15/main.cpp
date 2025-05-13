#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/action.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

void initialize() {
	pros::lcd::initialize();
	imu.reset(true);
	
	odom::start(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	auton(ringToKeep);
}

void opcontrol() {
	teleOp(ringToKeep);
}