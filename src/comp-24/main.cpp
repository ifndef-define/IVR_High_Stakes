#include "main.h"
#include "robots/comp-24/devices.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/auton.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	intakeColor.set_led_pwm(100);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	imuLeft.reset(true);

	armRot.reset();
	chassis.calibrate();
	delay(100);
	// pros::lcd::print(0, "Comp 15 Bot");
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

// pros::Task *odomTask;

/* Autonmous Method */
void autonomous() {
	// pros::Task odomTask(odom::start);
	// pros::Task ringThread(Intake::ringTask);

}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	// pros::Task ringThread(Intake::ringTask);

	// teleOp();
	auton1(1);
}