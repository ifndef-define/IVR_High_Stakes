#include "main.h"
#include "robots/comp-15/auton.h"
#include "robots/comp-15/controls.h"
#include "robots/comp-15/action.h"

Ring::Color ringToKeep = Ring::Color::BLUE;

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

pros::Imu imu(12);
odom *robotOdom = new odom(&xEnc, &yEnc, &imu, odom::r_coord(0, 0, 90), 
	-1.25, 1.0001, -0.625, 1.0001);

void initialize() {
	pros::lcd::initialize();
	imu.reset(false);
	


	actions.setRingColor(ringToKeep);


	// chassis.calibrate(true);
	// chassis.setPose(0, 0, 0);
	// pros::Task screen_task([&]() {
    //     while (true) {
    //         // print robot location to the brain screen
    //         // pros::lcd::print(0, "X: %f Y: %f", chassis.getPose().x, chassis.getPose().y); // x,y
    //         // pros::lcd::print(1, "Theta: %f", chassis.getPose().theta); // global theta
	// 		// pros::lcd::print(2, "Heading: %f", imu.get_rotation()); // rotations
    //         // delay to save resources
    //         pros::delay(20);
    //     }
    // });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	auton(ringToKeep);
}
void opcontrol() {
	teleOp(ringToKeep, false);
}