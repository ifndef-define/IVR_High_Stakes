#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/robot.hpp"

Ring::Color ringToKeep = Ring::Color::BLUE;

// #define USE_LLEMU

void initialize() {
// #ifdef USE_LLEMU
	pros::lcd::initialize();
	imu.reset(true);
// #else
// 	imu.reset(false);
// 	ui::init("Comp-24");
// #endif
	
	actions.setRingColor(ringToKeep);
	robotOdom.start(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// if (ui::getCurrentAuto() == 0 || ui::getCurrentAuto() == 1) {
	// 	ringToKeep = Ring::Color::RED;
	// } else if (ui::getCurrentAuto() == 2 || ui::getCurrentAuto() == 3) {
	// 	ringToKeep = Ring::Color::BLUE;
	// } else {
	// 	ringToKeep = ui::getRingColor() ? Ring::Color::BLUE : Ring::Color::RED;
	// }
	// auton(ringToKeep);
	redAuton1();
}
void opcontrol() {
	odom::setPos(odom::r_coord(51.25, 35.25, 27.95));
	teleOp(ringToKeep);
}