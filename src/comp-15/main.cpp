#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/action.h"

Ring::Color ringToKeep = Ring::Color::NONE;
#define USE_LLEMU

void initialize() {
#ifdef USE_LLEMU
	pros::lcd::initialize();
	imu.reset(true);
#else
	imu.reset(false);
	ui::init("Comp-15");
#endif
	
	actions.setRingColor(ringToKeep);
	odom::start(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	chassis->setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	// if (ui::getCurrentAuto() == 0 || ui::getCurrentAuto() == 1) {
	// 	ringToKeep = Ring::Color::RED;
	// } else if (ui::getCurrentAuto() == 2 || ui::getCurrentAuto() == 3) {
	// 	ringToKeep = Ring::Color::BLUE;
	// } else {
	// 	ringToKeep = ui::getRingColor() ? Ring::Color::BLUE : Ring::Color::RED;
	// }
	auton(ringToKeep);
	// ctrler.rumble("----");
}

void opcontrol() {
	// chassis->turnToAngle(90, 10000, 127, 0.3, false);
	// chassis->swingToAngle(90, Drive::DriveSide::RIGHT, 10000, 127, 0.3, false);
	// chassis->turnToPoint(-25, -25, 10000, 127, 0.3, false);
	// chassis->swingToPoint(-25, -25, Drive::DriveSide::RIGHT, 10000, 127, 0.3, false);
	// chassis->translateBy(24, 10000, 0.5, 127, false);


	// chassis->moveToPose(25, 35, 90, true, 100000, 0, 127, 1120, 0.5, 0.5, 1.5, 8.2, false);
	// chassis->moveToPose(25, 25, 0, 1000000, 0, 80, 100, 0.3, 0.3, false);
	// chassis->moveByVector(25, -25, 100);
	// delay(3000);
	autonomous();
	teleOp(ringToKeep);
}