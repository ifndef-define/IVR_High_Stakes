#include "main2.h"

/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::print(0, "Comp 15 Bot");
}

// pros::Motor right_front(-11,GEARSET_06);
// pros::Motor right_front_middle(13,GEARSET_06);
// pros::Motor right_back_middle(-14,GEARSET_06);
// pros::Motor right_back(15,GEARSET_06);

// pros::Motor left_front(1,GEARSET_06);
// pros::Motor left_front_middle(-3,GEARSET_06);
// pros::Motor left_back_middle(4,GEARSET_06);
// pros::Motor left_back(-5,GEARSET_06);

Controller master(E_CONTROLLER_MASTER);

MotorGroup left_drive({-11, 13, -14, 15}, pros::v5::MotorGears::blue);
MotorGroup right_drive({1, -3, 4, -5}, pros::v5::MotorGears::blue);

// pros::Motor_Group left_drive_motors = {left_front, left_front_middle, left_back_middle, left_back};
// pros::Motor_Group right_drive_motors = {right_front, right_front_middle, right_back_middle, right_back};

pros::Motor intake(10, GEARSET_06); // in = neg, out = pos

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	while (true) {
		right_drive.move(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
		left_drive.move(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));

		if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {
			intake.move(127/2);
		} else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) {
			intake.move(-127/2);
		} else {
			intake.brake();
		}

		pros::delay(20);
	}
}