#include "main.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/auton.h"
#include "common/pid.h"

const static bool isBlue = 0; // 0 for red, 1 for blue

/* First method to run. Should last only a few seconds max. */
pros::Task *ringTask;
void initialize() {
	intakeColor.set_led_pwm(100);
	intakeColor.set_integration_time(50);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.calibrate(true);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	intake.setColorToKeep(isBlue);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {
	// if (ui::selection == ui::COLOR_BLUE) {
	// 	ringTask = new pros::Task(Intake::ringTask);
	// 	intake.setColorToKeep(1);
	// 	chassis.setPose(-54, 13, 90);
	// 	blueAuton();
	// } else if (ui::selection == ui::COLOR_RED) {
	// 	ringTask = new pros::Task(Intake::ringTask);
	// 	intake.setColorToKeep(0);
	// 	chassis.setPose(54, 13, 270);
	// 	redAuton();
	// } else {
		// chassis.setPose(-52, 0, 90);
		// chassis.setPose(-52, 27, 90);
		ringTask = new pros::Task(Intake::ringTask);
		runAuton(0);
	// }
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {

	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	// if (ringTask != nullptr) {
	// 	ringTask->suspend();
	// }

	// pros::Task ringTControler([&]{
	// 	do {
	// 		if (ui::COLOR_GREEN) {
	// 			if (ringTask != nullptr) {
	// 				ringTask->suspend();
	// 			}
	// 		} else {
	// 			if (ringTask == nullptr) {
	// 				ringTask = new pros::Task(Intake::ringTask);
	// 			}
	// 		}

	// 		delay(30);
	// 	} while (!pros::competition::is_connected());
	// });


	teleOp();
}