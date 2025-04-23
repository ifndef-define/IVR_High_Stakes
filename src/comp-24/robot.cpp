#include "robots/comp-24/robot.hpp"

pros::Controller ctrler = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({4, -18, 1, -6}, pros::MotorGears::blue);
pros::MotorGroup rightDrive({-2, 15, -14, 7}, pros::MotorGears::blue);

drive *chassis = drive_builder(ctrler)
	.with_drive_config(drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_max_rpm(600)
	.add_straight_drive_scale(0.93,1)
	.add_ctrler_deadzone(3)
	.build();

DualIMU imu(11, 12, 2.5);