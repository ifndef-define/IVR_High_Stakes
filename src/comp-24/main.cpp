#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/robot.hpp"

Ring::Color ringToKeep = Ring::Color::RED;

void initialize() {
	pros::lcd::initialize();
	imu.reset(true);
	
	actions.setRingColor(ringToKeep);
	odom::start(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	if (ringToKeep == Ring::Color::RED) {
		redAuton1();
	} else if (ringToKeep == Ring::Color::BLUE) {
		blueAuton1();
	}
}
void opcontrol() {
    // odom::setPos(odom::r_coord(34.875, 35.125, 297.95));
	teleOp(ringToKeep);
}