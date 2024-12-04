#include "main.h"
#include "robots/comp-24/devices.h"
#include "robots/comp-24/controls.h"
#include "common/pid.h"

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	intakeColor.set_led_pwm(100);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	imuLeft.reset(true);
	// imuRight.reset(true);
	rxEnc.reset();
	lxEnc.reset();
	yEnc.reset();

	delay(100);

	armRot.reset();
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
	// odomTask = new pros::Task(odom::start);
	// if (odomTask != nullptr)
	// 	odomTask->suspend();

	odom::init(&rxEnc, &lxEnc, &yEnc, &imuLeft, {0, 0, 0});

	pros::Task odom(odom::start);


	teleOp();

	while(1){
		// joner.movePID(24);
		delay(15);
	}
}