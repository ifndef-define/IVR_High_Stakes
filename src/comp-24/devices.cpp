#include "robots/comp-24/devices.h"

// Right Front: 3
// Right Top: 4
// Right Back Top: -1
// Right Back Bottom: 2

// Left Front: -9
// Left Top: -8
// Left Back Top: 10
// Left Back Bottom: -7

// Right Intake: -14
// Left Intake: 20

// Backpack -11

pros::Controller ctrler(pros::E_CONTROLLER_MASTER);

PneumaticsGroup pneumatics;
Action actions(0, Ring::Color::NONE);

MotorGroup rightDrive({-2, 15, -14, 7}, MotorGears::blue);
MotorGroup leftDrive({4, -18, 1, -6}, MotorGears::blue);

#ifdef ENABLE_DUAL_IMU
DualIMU imu(12, 16, 2.5);
#else
pros::IMU imu(12);
// pros::IMU imu(16);
#endif

adi::Encoder xEnc(3, 4, true);
adi::Encoder yEnc(1, 2, false);

// LEMLIB Config
lemlib::TrackingWheel horizontal(&xEnc, 1.3235, -0.25);
lemlib::TrackingWheel vertical(&yEnc, 1.339, -0.84375);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

lemlib::Drivetrain drivetrain(&leftDrive, // left motor group
                              &rightDrive, // right motor group
                              11, // 11.825 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 3.25" omnis
                              600, // drivetrain rpm is 600
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(35, // proportional gain (kP)
                                            0.0, // integral gain (kI)
                                            3.55, // derivative gain (kD)
                                            0, // anti windup
                                            0, // small error range, in inches
                                            0, // small error range timeout, in milliseconds
                                            0, // large error range, in inches
                                            0, // large error range timeout, in milliseconds
                                            0 // maximum acceleration (slew)
);

// angular motion controller 2.45, 5.5 ///// 2.7,7
lemlib::ControllerSettings angularController(1.555, // proportional gain (kP) //1.41
                                             0.08, // integral gain (kI)
                                             6.11, // derivative gain (kD) //5
                                             10, // anti windup
                                             1, // small error range, in degrees
                                             150, // small error range timeout, in milliseconds
                                             1.5, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);