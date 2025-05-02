#include "robots/comp-15/robot.hpp"

pros::Controller ctrler = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({8, -1, 5, -7}, pros::MotorGears::blue);
pros::MotorGroup rightDrive({-4, 11, -13, 16}, pros::MotorGears::blue);

pros::MotorGroup leftClimbDrive({-8, 1, -5, 7, 17}, pros::MotorGears::blue);
pros::MotorGroup rightClimbDrive({4, -11, 13, -16, -15}, pros::MotorGears::blue);

pros::adi::Encoder yEnc(1, 2, false);
pros::adi::Encoder xEnc(3, 4, true);

DualIMU imu(2, 14, 2.5);
odom *robotOdom = new odom(&xEnc, &yEnc, &imu, odom::r_coord(0, 0, 0), 1.25, 0.9785, 0.625, 0.9765);

PID lat_pid(12,0,10,60,0);
PID turn_pid(3.2,0,13,60,0);

Drive *chassis = drive_builder(ctrler)
	.with_drive_config(Drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(Drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_max_rpm(600)
	.add_ctrler_deadzone(3)
	.add_odom_config(*robotOdom)
	.add_lat_pid(lat_pid)
	.add_turn_pid(turn_pid)
	// .add_drive_move_constants({
	// 	.kV = 0.15,
	// 	.kA = 0.002,
	// 	.a_des = 20,
	// 	.mVps_decrease = 15,
	// 	.mVps_increase = 35,
	// 	.ignore_slew = 1.5,
	// 	.maintain_angle_voltage = 30
	// })
	.build();
