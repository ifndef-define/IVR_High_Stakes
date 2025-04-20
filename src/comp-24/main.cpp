#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/devices.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

/**
 * A callback function for LLEMU's center button.
 *
 * Switches the autonomous routine color.
 */

void on_center_button() {
	switch(ringToKeep){
		case Ring::Color::BLUE:
			pros::lcd::set_text(0, "Red");
			ringToKeep = Ring::Color::RED;
			break;
		case Ring::Color::RED:
			pros::lcd::set_text(0, "Blue");
			ringToKeep = Ring::Color::BLUE;
			break;
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::register_btn1_cb(on_center_button);
	// chassis.calibrate(true);
	imu.reset(true);
	actions.setRingColor(ringToKeep);
/*
	// chassis.setPose(0, 0, 0);
	pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            // pros::lcd::print(1, "X: %f Y: %f", chassis.getPose().x, chassis.getPose().y); // x,y
            // pros::lcd::print(2, "Theta: %f", imu.getPose().theta); // global theta
			// pros::lcd::print(3, "Heading: %f", imu.get_rotation()); // rotations
            // delay to save resources
            pros::delay(20);
        }
    });
	*/

	pros::Task subsystem_task{[&]{
		while(true) {
			actions.runSubsystemFSM();
			// Make sure to update the IMU every loop
			imu.update();
			// Print the rotation value for debugging
			pros::lcd::print(2, "Theta: %f", imu.get_rotation());
			// You might want to add more debug info
			pros::lcd::print(3, "IMU1 Status: %d", imu.get_status());
			delay(10);
		}
	}};
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	auton(ringToKeep);
}

void opcontrol() {
	// while(true){
		
	// }
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	teleOp(ringToKeep);
}