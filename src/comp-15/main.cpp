#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::print(0, "Comp 15 Bot");
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {}

/* Driver Control. Runs default if not connected to field controler */

/* Drive Motors
front left: 1
front mid left: -3
rear mid left: 4
rear left: -5

front right: -11
front mid right: 13
rear mid right: -14
rear right: 15
*/

Controller master(E_CONTROLLER_MASTER);
// MotorGroup rightDrive({3, 4, -1, 5}, MotorGears::blue);
// MotorGroup leftDrive({-10, -9, 7, -8}, MotorGears::blue);
// MotorGroup rightDrive({3, 4, -1, 2}, MotorGears::blue);
// MotorGroup leftDrive({-9, -8, 10, -7}, MotorGears::blue);

MotorGroup rightDrive({3, 4, -2, 5}, MotorGears::blue);
MotorGroup leftDrive({-8, -6, 7, -9}, MotorGears::blue);


drive *maindrive = drive_builder(master)
	.with_drive_config(drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_exponetial_drive_scale(1.5)
	.add_sin_drive_scale(2)
	.add_straight_drive_scale(0.80,1)
	.add_ctrler_deadzone(5)
	.build();

void opcontrol() {
	// Unknown latency issue, neccessary to delay to prevent crash 
	delay(100);
	
	maindrive->loop(true);

	while (true) {
		
		delay(10);
	}
}