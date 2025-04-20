#include "robots/comp-15/devices.h"

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

MotorGroup rightDrive({-4, 11, -13, 16}, MotorGears::blue);
MotorGroup leftDrive({8, -1, 5, -7}, MotorGears::blue);

// PneumaticsGroup pneumatics;

Action actions(0, Ring::Color::NONE);

// DualIMU imu(2, 14, 2.5);
pros::IMU imu(14);
// adi::Encoder yEnc(adi::ext_adi_port_tuple_t(19, 1, 2));
// adi::Encoder rxEnc(adi::ext_adi_port_tuple_t(19, 3, 4), true);  // 3 4
adi::Encoder xEnc(1, 2, true);
adi::Encoder yEnc(3, 4, true);  // 3 4

// // LEMLIB Config
// const float VERT_RATIO = 0.9865; //1.33225
// const float HORI_RATIO = 0.987; //1.3485
// lemlib::TrackingWheel vertical(&yEnc, 1.36*VERT_RATIO, 1);
// lemlib::TrackingWheel horizontal(&xEnc, 1.36*HORI_RATIO, 0.375);

// // sensors for odometry
// lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
//                             nullptr, // vertical tracking wheel 2
//                             &horizontal, // horizontal tracking wheel
//                             nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
//                             &imu // inertial sensor
// );

// lemlib::Drivetrain drivetrain(&leftDrive, // left motor group
//                               &rightDrive, // right motor group
//                               11, // 11.825 inch track width
//                               lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
//                               600, // drivetrain rpm is 480
//                               24 // horizontal drift is 2. If we had traction wheels, it would have been 8
// );

// // lateral motion controller
// lemlib::ControllerSettings linearController(10.35, // proportional gain (kP)
//                                             0.0, // integral gain (kI)
//                                             7.9, // derivative gain (kD)
//                                             10, // anti windup
//                                             0.25, // small error range, in inches
//                                             100, // small error range timeout, in milliseconds
//                                             0.5, // large error range, in inches
//                                             250, // large error range timeout, in milliseconds
//                                             0 // maximum acceleration (slew)
// );

// // angular motion controller 2.45, 5.5 ///// 2.7,7
// lemlib::ControllerSettings angularController(1.49, // proportional gain (kP) //1.41
//                                              0.09, // integral gain (kI)
//                                              6.75, // derivative gain (kD) //5
//                                              10, // anti windup
//                                              1, // small error range, in degrees
//                                              150, // small error range timeout, in milliseconds
//                                              1.5, // large error range, in degrees
//                                              500, // large error range timeout, in milliseconds
//                                              0 // maximum acceleration (slew)
// );

// // input curve for throttle input during driver control
// lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
//                                      10, // minimum output where drivetrain will move out of 127
//                                      1.019 // expo curve gain
// );

// // input curve for steer input during driver control
// lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
//                                   10, // minimum output where drivetrain will move out of 127
//                                   1.019 // expo curve gain
// );

// // create the chassis
// lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);


// /**
//  * Odom tunning note,
//  * tune for small distance and large distance separately, then ccreate equation to
//  * represent the error in the system and use this to increment error based on
//  * absolute rotation of the tracking wheel
//  */