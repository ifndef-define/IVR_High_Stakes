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

Controller master(E_CONTROLLER_MASTER);
MotorGroup rightDrive({3, 4, -1, 5}, MotorGears::blue);
MotorGroup leftDrive({-10, -9, 7, -8}, MotorGears::blue);
void opcontrol() {
	drive *maindrive = drive_builder(master)
		.with_drive_config(drive::drive_config_e::TANK_c)
		.with_drive_motors(leftDrive, rightDrive)
		.with_drive_mode(drive::drive_mode_e::TANK_m)
		.build();

	while (true) {
		maindrive->loop();
		delay(20);
	}
}