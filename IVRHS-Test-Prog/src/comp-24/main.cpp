#include "main2.h"

// Devices //

pros::Controller ctrler(CONTROLLER_MASTER);

pros::Motor left_drive(-12, GEARSET_18);
pros::Motor right_drive(16, GEARSET_18);

pros::Rotation y_encoder(-11);
pros::Rotation x_encoder(-15);

pros::IMU imu_left(14);
pros::IMU imu_right(20);

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	imu_left.reset();
	imu_right.reset(true);

	waitUntil(imu_left.is_calibrating() == false && imu_right.is_calibrating() == false);
	
	// pros::lcd::print(0, "Comp 24 Bot");
	x_encoder.reset();
	x_encoder.set_position(0);
	y_encoder.reset();
	y_encoder.set_position(0);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {}

void display() {
	lcd::print(0, "IMU1: %f", imu_left.get_rotation());
	lcd::print(1, "IMU2: %f", imu_right.get_rotation());
	lcd::print(2, "X: %d", x_encoder.get_position());
	lcd::print(3, "Y: %d", y_encoder.get_position());
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	// Drive Vars
	int fwd, turn, left, right = 0;

	while(1) {
		fwd = ctrler.get_analog(RIGHT_Y_AXIS);
		turn = ctrler.get_analog(LEFT_X_AXIS);

		left = fwd - turn;
		right = fwd + turn;

		left_drive.move(left);
		right_drive.move(right);

		display();
		delay(15);
	}
}