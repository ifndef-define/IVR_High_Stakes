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

pros::MotorGroup leftDrive({3, -1, 16, -2}, MotorGears::blue);
pros::MotorGroup rightDrive({-13, 11, -14, 12}, MotorGears::blue);

PneumaticsGroup pneumatics;

Action actions(0, Ring::Color::NONE);

pros::IMU imu(8);
pros::adi::Encoder xEnc(1, 2, false);
pros::adi::Encoder yEnc(3, 4, true);

// LEMLIB Config
const float VERT_RATIO = 1;
const float HORI_RATIO = 1;
lemlib::TrackingWheel vertical(&xEnc, 1.36, -0.25);
lemlib::TrackingWheel horizontal(&yEnc, 1.36, -0.84375);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

lemlib::Drivetrain drivetrain(&leftDrive, // left motor group
                              &rightDrive, // right motor group
                              11.875, // 11.825 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 3.25" omnis
                              600, // drivetrain rpm is 600
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(11, // proportional gain (kP)
                                            0.0003, // integral gain (kI)
                                            1.4, // derivative gain (kD)
                                            60, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // maximum acceleration (slew)
);

// angular motion controller 2.45, 5.5 ///// 2.7,7
lemlib::ControllerSettings angularController(2.5, // proportional gain (kP) //.8
                                             0.001, // integral gain (kI)
                                             6, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             30 // maximum acceleration (slew)
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