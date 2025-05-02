#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/action.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

void initialize() {
	pros::lcd::initialize();
	
	imu.reset(true);
	actions.setRingColor(ringToKeep);
	robotOdom->start(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	// auton(ringToKeep);
}

void opcontrol() {
	// chassis->turnToAngle(90, 10000, 127, 0.3, false);
	// chassis->swingToAngle(90, Drive::DriveSide::RIGHT, 10000, 127, 0.3, false);
	// chassis->turnToPoint(-25, -25, 10000, 127, 0.3, false);
	// chassis->swingToPoint(-25, -25, Drive::DriveSide::RIGHT, 10000, 127, 0.3, false);
	// chassis->translateBy(24, 10000, 0.5, 127, false);


	chassis->moveToPose(25, -25, 0, 10000, 35, 127, 100, 0.3, 0.3, 0.5, 3, false);

	teleOp(ringToKeep, false);
}