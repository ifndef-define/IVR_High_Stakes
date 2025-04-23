#include "robots/comp-15/robot.hpp"

pros::Controller ctrler = pros::Controller(pros::E_CONTROLLER_MASTER);

MotorGroup rightDrive({-4, 11, -13, 16}, MotorGears::blue);
MotorGroup leftDrive({8, -1, 5, -7}, MotorGears::blue);
MotorGroup rightClimbDrive({-4, 11, -13, 16, -9}, MotorGears::blue);
MotorGroup leftClimbDrive({8, -1, 5, -7, 10}, MotorGears::blue);

drive *chassis = drive_builder(ctrler)
	.with_drive_config(drive::drive_config_e::TANK_c)
	.with_drive_motors(leftDrive, rightDrive)
	.with_drive_mode(drive::drive_mode_e::SPLIT_ARCADE_PL)
	.add_max_rpm(600)
	.add_straight_drive_scale(0.93,1)
	.add_ctrler_deadzone(3)
	.build();

DualIMU imu(2, 14, 2.5);