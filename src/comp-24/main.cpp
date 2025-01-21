#include "main.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/auton.h"
#include "common/pid.h"

const static bool isBlue = 0; // 0 for red, 1 for blue

/* First method to run. Should last only a few seconds max. */
pros::Task *ringTask;
void initialize() {
	pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1,`1 "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
			pros::lcd::print(3, "IMU Heading: %f", imuRight.get_heading()); // heading
            // delay to save resources
            pros::delay(20);
        }
    });
	intakeColor.set_led_pwm(100);
	intakeColor.set_integration_time(50);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	intake.setColorToKeep(isBlue);
	// mogoRushClamp.extend();
	// intakeLift.extend();

	chassis.calibrate(true);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	leftDrive.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	rightDrive.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	// chassis.setPose(-50, 30, 270);
	// chassis.setPose(-54, 13, 90); //53.5, 61, 90

	pros::Task ringThread(Intake::ringTask);
	// armMotor.move(10);
	// delay(200);
	// armRot.reset();
	// armMotor.brake();
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