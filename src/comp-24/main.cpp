#include "main.h"
#include "robots/comp-24/auton.h"
#include "robots/comp-24/controls.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

/**
 * A callback function for LLEMU's center button.
 *
 * Sets our autonomous routine to the blue side.
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
	pros::Task autoRingSort{[&]{
        actions.setAutonControlFlag(true);
        while(true){
            actions.runSubsystemFSM();
            delay(10);
        }
    }};
	chassis.calibrate();
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	auton(ringToKeep);
}

void opcontrol() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	actions.setAutonControlFlag(false);
	teleOp(ringToKeep);
}