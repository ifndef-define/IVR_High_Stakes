#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "math.h"

#define i_waitUntil(condition) \
	do                         \
	{                          \
		pros::delay(5);        \
	} while (!(condition))

const double pwr = 50;
const double encoder_radius = 1.96 / 2;
// motor ranges from -127 to 127
const double degTorad = M_PI / 180;
const double encVal = (M_PI * 1.36) / 8192; // 8192 ticks per revolution

// 1.96 inches diameter
IMU imu(11);
Controller master_ctrler(E_CONTROLLER_MASTER);

#define SMART_PORT 16
pros::adi::Encoder L_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'A', 'B'});
pros::adi::Encoder X_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'C', 'D'});
pros::adi::Encoder R_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'E', 'F'});

MotorGroup right_drive{3, 4, -1, 5};
MotorGroup left_drive{-10, -9, 7, -8};
MotorGroup intake{-13, 17};
MotorGroup arm{-14};

void initialize()
{
	pros::lcd::initialize();
	L_ENC.reset();
	R_ENC.reset();
	X_ENC.reset();
	imu.reset(true);
	pros::lcd::print(0, "imu resetted");

}

int drive_stop()
{
	right_drive.brake();
	left_drive.brake();
	return 0;
}

int drive_fwd(int pwr)
{
	right_drive.move_velocity(pwr);
	left_drive.move_velocity(pwr);
	return 0;
}

void disabled() {}

void competition_initialize() {}

void autonomous()
{
	right_drive.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	left_drive.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
}

void opcontrol()
{
	// right_drive.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	// left_drive.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	double left, right = 0;
	double forward, turn = 0;
	double deadzone = 5;
	bool toggle = 0;
	const double L_offset = 1.25, R_offset = 1.25, x_offset = 1, halfRoboX = 2.25 / 2, halfRoboY = 0.5;
	double L_encoder, R_encoder, deltL, deltR, X_encoder, turnRad, deltX, deltY, strafRad;
	double timeNow, timePrev, dt;
	double target_x = 10, target_y = 4, theta = 0, now_x = 0, now_y = 0, diff_theta = 0; 
	double orientation_theta = 0;

	L_encoder = L_ENC.get_value() * encVal; // Gets the encoder Value for Left value
	R_encoder = R_ENC.get_value() * encVal; // Gets the encoder Value for Right value
	X_encoder = X_ENC.get_value() * encVal; // Gets the encoder Value for X value
	theta = convert::degToRad(imu.get_rotation()); //Gets the IMU Value of the global angle

	while (1)
	{
		deltL = L_ENC.get_value() * encVal - L_encoder; // Subtract old with new values;
		deltR = R_ENC.get_value() * encVal - R_encoder;
		deltX = X_ENC.get_value() * encVal - X_encoder;

		L_encoder = L_ENC.get_value() * encVal; // Update these Values
		R_encoder = R_ENC.get_value() * encVal;
		X_encoder = X_ENC.get_value() * encVal;

		diff_theta = convert::degToRad(imu.get_rotation()) - theta;
		if(diff_theta != 0){ 
			//Update deltX and deltY as sthey traversed at an angle*
			deltX = 2*sin(diff_theta/2)*(deltX/diff_theta + x_offset);
			deltY = 2*sin(diff_theta/2)*(deltR/diff_theta + R_offset);
		}
		else{
			//deltY takes the average of deltL and deltR incase both of them aren't equal
			deltY = (deltL+deltR)/2;
		}
		orientation_theta = theta + diff_theta/2; //We are going to apply matrix multiplication to the current deltX and deltY
		theta = convert::degToRad(imu.get_rotation());

		now_x+= deltX*cos(orientation_theta)+deltY*sin(orientation_theta);
		now_y+= -deltX*sin(orientation_theta)+deltY*cos(orientation_theta);


		// //theta = (deltR - deltL) / (2.25); // Find angle made in one turn
		// //diff_theta = convert::degToRad(imu.get_rotation()) - theta;
		// theta += diff_theta;

		// if (diff_theta != 0)
		// {
		// 	turnRad = (halfRoboX * (deltL + deltR)) / (deltR - deltL+0.0000001);
		// 	strafRad = (deltX / diff_theta) - halfRoboY;
		// 	deltX = turnRad * (cos(theta) - 1) + strafRad * sin(theta);
		// 	deltY = turnRad * sin(theta) + strafRad * (1 - cos(theta));
		// }

		// else
		// {
		// 	deltX = turnRad * (cos(theta) - 1) + strafRad * sin(theta);
		// 	deltY = (deltL + deltR) / 2;
		// }

		// now_x += deltX;
		// now_y += deltY;

		//pros::lcd::print(1, "now_x: %f inches", now_x*(48/41.67));
		//pros::lcd::print(2, "now_y: %f inches", now_y*(72 / 73.188));
		pros::lcd::print(1, "now_x: %f inches", now_x);
		pros::lcd::print(2, "now_y: %f inches", now_y);
		pros::lcd::print(3, "L_encoder: %f inches", L_encoder);
		pros::lcd::print(4, "R_encoder: %f inches", R_encoder);
		pros::lcd::print(5, "X_ENC: %f inches", X_ENC.get_value() * encVal);
		pros::lcd::print(6, "now_theta: %f", convert::radToDeg(theta));
		pros::lcd::print(7, "deltX: %f inches", convert::radToDeg(deltX));

		delay(15);
		timePrev = timeNow;
	}

	// while (1)
	// {
	// 	if (master_ctrler.get_digital_new_press(E_CONTROLLER_DIGITAL_A))
	// 		toggle = !toggle;

	// 	if (toggle)
	// 	{
	// 		lcd::clear_line(0);
	// 		lcd::print(0, "Tank Drive Mode");
	// 		// Tank Drive
	// 		left = abs(master_ctrler.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) > deadzone ? master_ctrler.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) : 0;
	// 		right = abs(master_ctrler.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)) > deadzone ? master_ctrler.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y) : 0;

	// 		left_drive.move(left);
	// 		right_drive.move(right);
	// 	}
	// 	else
	// 	{
	// 		lcd::clear_line(0);
	// 		lcd::print(0, "Arcade Drive Mode");

	// 		// Arcade Drive
	// 		forward = abs(master_ctrler.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) > deadzone ? master_ctrler.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) : 0;
	// 		turn = abs(master_ctrler.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)) > deadzone ? master_ctrler.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) : 0;

	// 		left_drive.move(forward + turn);
	// 		right_drive.move(forward - turn);
	// 	}

	// 	delay(15);
	// }
}