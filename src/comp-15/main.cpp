#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/robot.hpp"

Ring::Color ringToKeep = Ring::Color::BLUE;

// drive *climbChassis = drive_builder(ctrler)
// 	.with_drive_config(drive::drive_config_e::TANK_c)
// 	.with_drive_motors(leftClimbDrive, rightClimbDrive)
// 	.with_drive_mode(drive::drive_mode_e::SPLIT_ARCADE_PL)
// 	.add_max_rpm(600)
// 	.add_ctrler_deadzone(3)
// 	.build();

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