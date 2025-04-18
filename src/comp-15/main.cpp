#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	// pros::lcd::print(0, "Comp 15 Bot");
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {}

Controller master(E_CONTROLLER_MASTER);

pros::MotorGroup leftDrive({4, -18, 1, -6});
pros::MotorGroup rightDrive({-2, 15, -14, 7});

drive *maindrive = drive_builder(master)
	.with_drive_config(drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(drive::drive_mode_e::SPLIT_ARCADE_PL)
	// .add_exponetial_drive_scale(2.5)
	// .add_sin_drive_scale(2)
	.add_straight_drive_scale(0.83,1)
	// .add_ctrler_deadzone(10)
	.build();

void opcontrol() {
	// Unknown latency issue, neccessary to delay to prevent crash 
	delay(100);
	
	maindrive->loop(true);
	
	while (true) {
		
		delay(10);
	}
}

/*
track widht 8 1/16
track length 11 1/16


*/