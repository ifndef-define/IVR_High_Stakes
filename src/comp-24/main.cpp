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
	
	// pros::lcd::register_btn0_cb(on_center_button);
	// pros::lcd::register_btn1_cb(on_left_button);
	// pros::lcd::register_btn2_cb(on_right_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	auton(ringToKeep);
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	teleOp(ringToKeep);
}