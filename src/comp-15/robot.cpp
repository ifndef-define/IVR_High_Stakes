#include "robots/comp-15/robot.hpp"

pros::Controller ctrler = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({8, -1, 5, -7}, pros::MotorGears::blue);
pros::MotorGroup rightDrive({-4, 11, -13, 16}, pros::MotorGears::blue);

pros::MotorGroup leftClimbDrive({-8, 1, -5, 7, -17}, pros::MotorGears::blue);
pros::MotorGroup rightClimbDrive({4, -11, 13, -16, 15}, pros::MotorGears::blue);

pros::adi::Encoder yEnc(1, 2, true);
pros::adi::Encoder xEnc(3, 4, false);
	
Drive *chassis = drive_builder(ctrler)
	.with_drive_config(Drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(Drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_max_rpm(600)
	// .add_straight_drive_scale(0.93,1)
	.add_ctrler_deadzone(3)
	.build();

DualIMU imu(2, 14, 2.5);
odom *robotOdom = new odom(&xEnc, &yEnc, &imu, odom::r_coord(0, 0, 0), 1.25, 1.0001, 0.625, 1.0001);