#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"

/* Motors:
============
Left Front: 4
Left Mid Front: -18
Left Mid Back: 1
Left Back: -6
Right Front: -2
Right Mid Front: 15
Right Mid Back: -14
Right Back: 7

Left Intake: -9
Right Intake: 10

Arm: 3

3-Wire 1: 8
MogoClamp: A
Left MogoRushArm: B
MogoRushTeeth: C
Intake Lift: D
Intake Lock: E
Right MogoRushArm: F

3-Wire 2:


*/

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({4, -18, 1, -6});
pros::MotorGroup rightDrive({-2, 15, -14, 7});
pros::MotorGroup intake({-9, 10});
pros::Motor arm(3);

pros::adi::Pneumatics mogoClamp(pros::adi::ext_adi_port_pair_t(8, 1), false);
pros::adi::Pneumatics leftMogoRushArm(pros::adi::ext_adi_port_pair_t(8, 2), false);
pros::adi::Pneumatics mogoRushTeeth(pros::adi::ext_adi_port_pair_t(8, 3), false);
pros::adi::Pneumatics intakeLift(pros::adi::ext_adi_port_pair_t(8, 4), false);
pros::adi::Pneumatics intakeLock(pros::adi::ext_adi_port_pair_t(8, 5), true);
pros::adi::Pneumatics rightMogoRushArm(pros::adi::ext_adi_port_pair_t(8, 6), false);


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
	int fwd, turn;
	arm.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	while(1) {
		fwd = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		leftDrive.move(fwd + turn);
		rightDrive.move(fwd - turn);

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			intake.move(127);
		} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			intake.move_velocity(-127);
		} else {
			intake.brake();
		}

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			arm.move(127/2.3);
		} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			arm.move(-127/2.3);
		} else {
			arm.brake();
		}

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			leftMogoRushArm.toggle();
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			rightMogoRushArm.toggle();
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
			mogoRushTeeth.toggle();
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			mogoClamp.toggle();
		}

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			intakeLift.toggle();
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			intakeLock.toggle();
		}
		delay(10);
	}
}