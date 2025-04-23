#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/robot.hpp"

Ring::Color ringToKeep = Ring::Color::BLUE;

void initialize() {
	pros::lcd::initialize();
	
	imu.reset(true);
	actions.setRingColor(ringToKeep);

	// chassis.calibrate(true);
	// chassis.setPose(0, 0, 0);
	// pros::Task screen_task([&]() {
    //     while (true) {
    //         // print robot location to the brain screen
    //         // pros::lcd::print(0, "X: %f Y: %f", chassis.getPose().x, chassis.getPose().y); // x,y
    //         // pros::lcd::print(1, "Theta: %f", chassis.getPose().theta); // global theta
	// 		// pros::lcd::print(2, "Heading: %f", imu.get_rotation()); // rotations
    //         // delay to save resources
    //         pros::delay(20);
    //     }
    // });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	auton(ringToKeep);
}
void opcontrol() {
	teleOp(ringToKeep, false);
}