#include "main2.h"
#include "lemlib/api.hpp"
// Devices //

pros::Controller ctrler(CONTROLLER_MASTER);

pros::Motor left_drive(-12, GEARSET_18);
pros::Motor right_drive(16, GEARSET_18);

pros::MotorGroup left_motor({-12});
pros::MotorGroup right_motor({16});

/*
Odom Details
//////////////
WB: 4"
TW: 11 9/16"

TC: 2" x 5.78125" aka 2" x 5 25/32"

XOFS: +9/16"
YOFS: -1 1/4"
*/

pros::Rotation y_encoder(-11);
pros::Rotation x_encoder(-15);

pros::IMU imu_left(14);
pros::IMU imu_right(20);

lemlib::TrackingWheel x_track_wheel(&x_encoder, 1.96, 5.78125, -1);
lemlib::TrackingWheel y_track_wheel(&y_encoder, 1.96, 5.78125, 1);

lemlib::Drivetrain drivetrain(&left_motor, // left motor group
                              &right_motor, // right motor group
                              11.5625, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              200, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

lemlib::OdomSensors sensors(&y_track_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &x_track_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu_left // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	imu_left.reset();
	imu_right.reset(true);

	i_waitUntil(imu_left.is_calibrating() == false && imu_right.is_calibrating() == false);
	
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
	lcd::print(2, "XE: %d", x_encoder.get_position());
	lcd::print(3, "YE: %d", y_encoder.get_position());
	lcd::print(4, "X: %f", chassis.getPose().x);
	lcd::print(5, "Y: %f", chassis.getPose().y);
}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	// Drive Vars
	int fwd, turn, left, right = 0;
	chassis.calibrate(false);

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