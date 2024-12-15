#include "main.h"
#include "robots/comp-15/devices.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/auton.h"
#include "common/pid.h"

// const static bool isBlue = 0; // 0 for red, 1 for blue

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	// lv_init();
	// ui::init();
	intakeColor.set_led_pwm(100);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	// pros::lcd::print(0, "Comp 15 Bot");
	chassis.calibrate(true);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
}

pros::Task *ringTask;
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
		ringTask = new pros::Task(Intake::ringTask);
		// chassis.setPose(-52, 0, 90);
		skillsAuton();
	// }
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {

	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	if (ringTask != nullptr) {
		ringTask->suspend();
	}

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

	if(pros::competition::is_connected() || pros::competition::is_competition_switch()) {

		chassis.setPose(-52, 0, 90);
		arm.setPosition(0);
		// intake.setVoltage(-127);
		// delay(250);
		// intake.setVoltage(127);
		// delay(750);
		// chassis.setPose(-52, 0, 90);
		//arm.setPosition(0);
		intake.setVoltage(-127);
		delay(250);
		intake.setVoltage(127);
		delay(500);

		chassis.moveToPose(-58.5, 0, 90, 1200, {.forwards = false, .maxSpeed = 60});
		// while(chassis.isInMotion())
		// {
		//     if(intakeColor.get_proximity() >= 170)
		//     {
		//         intake.brake();
		//     }
		//     else
		//     {
		//         intake.setVoltage(127);
		//     }
		// }
		// i_waitUntil(!chassis.isInMotion());
		// intake.setAutonControlFlag(true);
		delay(1200); //tune
		// intake.setAutonControlFlag(false);
		intake.setVoltage(0);
	}

	teleOp();

	while(1)
		delay(100);
}