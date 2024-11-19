#include "main.h"
#include "common/pid.h"
#include "pros/imu.hpp"
#include "pros/rotation.hpp"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "common/pid.h"

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
void opcontrol() {
	//Create Necessary Objects
	PID latPID(0, 0.0, 0.0, 127); //Forward/back PID
	PID turnPID(0, 0.0, 0.0, 127); //Turn PID

	while (true) {
		latPID.update(0, 0);
		turnPID.update(0, 0);
	}
}