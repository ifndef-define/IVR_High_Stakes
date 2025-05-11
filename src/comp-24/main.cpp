#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/robot.hpp"

Ring::Color ringToKeep = Ring::Color::RED;

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