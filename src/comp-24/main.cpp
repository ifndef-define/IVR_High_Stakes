#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"

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
	chassis.calibrate(true);

	pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
			pros::lcd::print(3, "IMU Heading: %f", imu.get_rotation()); // heading
            // delay to save resources
            pros::delay(20);
        }
    });

	pros::Task autoRingSort{[&]{
        // while(pros::competition::is_autonomous()){
        while(true) {
            actions.runSubsystemFSM();
			imu.update();
            delay(10);
        }
    }};
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
    chassis.turnToHeading(90, 100000, {}, 0);
	// auton(ringToKeep);
}

void opcontrol() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	actions.setAutonControlFlag(false);
	teleOp(ringToKeep);
}