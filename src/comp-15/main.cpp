#include "main.h"
#include "robots/comp-15/devices.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/auton.h"
#include "common/pid.h"

const static bool isBlue = 0; // 0 for red, 1 for blue

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	intakeColor.set_led_pwm(100);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	intake.setColorToKeep(isBlue);
	// mogoRushClamp.extend();
	// intakeLift.extend();

	pros::lcd::print(0, "Comp 15 Bot");
	chassis.calibrate(true);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	leftDrive.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	rightDrive.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	// chassis.setPose(-50, 30, 270);
	chassis.setPose(-54, 13, 90); //53.5, 61, 90

	// armMotor.move(10);
	// delay(200);
	armRot.reset();
	// armMotor.brake();
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
	// runAuton(isBlue);
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	pros::Task ringThread(Intake::ringTask);
	// pros::Task telemetry(debug);
	// runAuton(isBlue);
	// intake.setAutonControlFlag(true);
	teleOp();
	// while(1)
	// 	delay(1000);
	// mogoRushReach.extend();
}