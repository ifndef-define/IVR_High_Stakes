#include "robots/comp-24/robot.hpp"

pros::Controller ctrler = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({4, -18, 1, -6}, pros::MotorGears::blue);
pros::MotorGroup rightDrive({-2, 15, -14, 7}, pros::MotorGears::blue);

pros::MotorGroup leftClimbDrive({4, -18, 1, -6, -9}, pros::MotorGears::blue);
pros::MotorGroup rightClimbDrive({-2, 15, -14, 7, 10}, pros::MotorGears::blue);

pros::adi::Encoder yEnc(3, 4, true);
pros::adi::Encoder xEnc(1, 2, true);
DualIMU imu(12, 16, 2.5);
// pros::IMU imu(12);
odom robotOdom(&xEnc, &yEnc, &imu, odom::r_coord(0, 0, 0), 1.25, 1, 0.625, 1);

PID lat_pid(18,0,10,5,0);
PID turn_pid(6.15,0.600,53.333,5,0);

Drive *chassis = drive_builder(ctrler)
	.with_drive_config(Drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(Drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_max_rpm(600)
	.add_ctrler_deadzone(3)
	.add_odom_config(robotOdom)
	.add_lat_pid(lat_pid)
	.add_turn_pid(turn_pid)
	.build();
