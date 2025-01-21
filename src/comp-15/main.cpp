#include "main.h"
#include "robots/comp-15/devices.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/auton.h"
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
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
			pros::lcd::print(3, "IMU Heading: %f", imuLeft.get_heading()); // heading
            // delay to save resources
            pros::delay(20);
        }
    });
	intakeColor.set_led_pwm(100);
	intakeColor.set_integration_time(50);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	// pros::lcd::print(0, "Comp 15 Bot");
	chassis.calibrate(true);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	intake.setColorToKeep(isBlue);
	// ringTask = new pros::Task(Intake::ringTask);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {
	// if (ui::selection == ui::COLOR_BLUE) {
		ringTask = new pros::Task(Intake::ringTask);
	// #ifdef BLUE
	// 	// chassis.setPose(-54, 13, 90);
	// 	// blueAuton();
	// #endif
	// // } else if (ui::selection == ui::COLOR_RED) {
	// #ifdef RED
	// 	// ringTask = new pros::Task(Intake::ringTask);
	// 	intake.setColorToKeep(0);
		chassis.setPose(-54, 13, 90);
	// #endif
		redAuton();
	// } else {
		// chassis.setPose(-52, 0, 90);
		// skillsAuton();
	// }
}

/* Driver Control. Runs default if not connected to field controller */
void opcontrol() {

	intakeLift.retract();
	arm.setPosition(0);
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	doinker.retract();

	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	if (ringTask == nullptr) {
		ringTask = new pros::Task(Intake::ringTask);
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

	// if(pros::competition::is_connected() || pros::competition::is_competition_switch()) {

	// 	chassis.setPose(-52, 0, 90);
	// 	arm.setPosition(0);
	// 	// intake.setVoltage(-127);
	// 	// delay(250);
	// 	// intake.setVoltage(127);
	// 	// delay(750);
	// 	// chassis.setPose(-52, 0, 90);
	// 	//arm.setPosition(0);
	// 	intake.setVoltage(-127);
	// 	delay(250);
	// 	intake.setVoltage(127);
	// 	delay(500);

	// 	chassis.moveToPose(-58.5, 0, 90, 1200, {.forwards = false, .maxSpeed = 60});
	// 	// while(chassis.isInMotion())
	// 	// {
	// 	//     if(intakeColor.get_proximity() >= 170)
	// 	//     {
	// 	//         intake.brake();
	// 	//     }
	// 	//     else
	// 	//     {
	// 	//         intake.setVoltage(127);
	// 	//     }
	// 	// }
	// 	// i_waitUntil(!chassis.isInMotion());
	// 	// intake.setAutonControlFlag(true);
	// 	delay(1200); //tune
	// 	// intake.setAutonControlFlag(false);
	// 	intake.setVoltage(0);
	// }

	teleOp();

	while(1)
		delay(100);
}