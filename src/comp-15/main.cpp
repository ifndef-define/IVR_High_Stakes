#include "main.h"
#include "common/pid.h"
#include "pros/imu.hpp"
#include "pros/rotation.hpp"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"

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
	PID pid1(2, 0, 0.5);
	PID pid2(2, 0, 0.5);
	pros::Motor motor1(16, MotorGears::green, MotorEncoderUnits::degrees);
	pros::Motor motor2(12, MotorGears::green, MotorEncoderUnits::degrees);
	pros::IMU imu(14);

	double angle = 180;

	while (true) {
		double error = angle - imu.get_rotation();
		double error1 = pid1.update(error);
		double error2 = pid2.update(error);
		motor1.move(error1);
		motor2.move(error2);

		if (std::abs(error) < 5) {
			angle = - angle;
		}

		delay(100);
	}
}