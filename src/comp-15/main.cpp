#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"

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
	chassis.calibrate(false);
	imu.reset(true);

	chassis.setPose(0, 0, 0);
	pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f Y: %f", chassis.getPose().x, chassis.getPose().y); // x,y
            pros::lcd::print(1, "Theta: %f", chassis.getPose().theta); // global theta
			pros::lcd::print(2, "Heading: %f", imu.get_rotation()); // rotations
            // delay to save resources
            pros::delay(20);
        }
    });

	pros::Task subsystem_task{[&]{
        while(true) {
			#ifdef ENABLE_DUAL_IMU
			imu.update();
			#endif
            actions.runSubsystemFSM();
            delay(10);
        }
    }};
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
	chassis.moveToPose(0,48,0,5000,{},0);
    chassis.turnToHeading(90, 5000, {}, 0);
	// auton(ringToKeep);
}

void opcontrol() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	actions.setAutonControlFlag(false);
	// teleOp(ringToKeep);

		while (true) {
			if(ctrler.get_digital_new_press(BUTTON_DOWN)) {
				// auton(ringToKeep);
				autonomous();
				chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
				teleOp	(ringToKeep);
				break;
			}
			else if (ctrler.get_digital_new_press(BUTTON_LEFT)) {
				teleOp(ringToKeep);
				break;
			}
	
			delay(20);
		}
}