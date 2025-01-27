#include "main.h"
#include "robots/comp-24/controls.h"
#include "robots/comp-24/auton.h"
#include "common/pid.h"

const static bool isBlue = 1; // 0 for red, 1 for blue, 2 for skills

/* First method to run. Should last only a few seconds max. */
pros::Task *ringTask;
void initialize() {
	pros::lcd::initialize(); // initialize brain screen
	intakeColor.set_led_pwm(100);
	intakeColor.set_integration_time(20);
    chassis.calibrate(); // calibrate sensors
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
			pros::lcd::print(2, "Odom1: %f", rxEnc.get_value()); // x
            pros::lcd::print(3, "Odom2: %f", yEnc.get_value()); // y
            pros::lcd::print(4, "Theta: %f", chassis.getPose().theta); // heading
			pros::lcd::print(5, "IMU Heading: %f", imuLeft.get_heading()); // heading
            // delay to save resources
            pros::delay(20);
        }
    });
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	intake.setColorToKeep(isBlue);
	// mogoRushClamp.extend();
	// intakeLift.extend();

	chassis.calibrate(true);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	leftDrive.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	rightDrive.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);

	ringTask = new pros::Task(Intake::ringTask);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {
	runAuton(isBlue);
	// skillsAuton();
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	// if (ringTask == nullptr) {
	// 	ringTask->suspend();
	// }
	teleOp();
}