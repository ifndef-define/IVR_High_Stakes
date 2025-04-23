#include "robots/comp-24/robot.hpp"

pros::Controller ctrler = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({4, -18, 1, -6}, pros::MotorGears::blue);
pros::MotorGroup rightDrive({-2, 15, -14, 7}, pros::MotorGears::blue);

pros::MotorGroup leftClimbDrive({4, -18, 1, -6, -9}, pros::MotorGears::blue);
pros::MotorGroup rightClimbDrive({-2, 15, -14, 7, 10}, pros::MotorGears::blue);

pros::adi::Encoder yEnc(3, 4, true);
pros::adi::Encoder xEnc(1, 2, false);

drive *chassis = drive_builder(ctrler)
	.with_drive_config(drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_max_rpm(600)
	// .add_straight_drive_scale(0.93,1)
	.add_ctrler_deadzone(3)
	.build();

// pros::Imu imu(12);
DualIMU imu(11, 12, 2.5);
odom *robotOdom = new odom(&xEnc, &yEnc, &imu, odom::r_coord(0, 0, 90), 
	-1.25, 1.0001, -0.625, 1.0001);