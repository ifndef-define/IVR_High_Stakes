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

MotorGroup rightDrive({3, -15, 1, 5}, pros::MotorGears::blue);
MotorGroup leftDrive({-10, -9, 7, -8}, pros::MotorGears::blue);

PneumaticsGroup pneumatics;

IMU imu(20);
adi::Encoder yEnc(adi::ext_adi_port_tuple_t(16, 1, 2));
adi::Encoder rxEnc(adi::ext_adi_port_tuple_t(16, 3, 4), true); // 3 4

// Lemlib Config
const float VERT_RATIO = 0.9795955882;
const float HORI_RATIO = 0.9915441176;
// lemlib::TrackingWheel vertical(&lxEnc, 1.36*VERT_RATIO, -1);
lemlib::TrackingWheel vertical2(&rxEnc, 1.36*VERT_RATIO, 1);
lemlib::TrackingWheel horizontal(&yEnc, 1.36*HORI_RATIO, 0.375);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical2, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

lemlib::Drivetrain drivetrain(&leftDrive, // left motor group
                              &rightDrive, // right motor group
                              11.825, // 11.825 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              480, // drivetrain rpm is 480
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
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
lemlib::ControllerSettings angularController(0.80, // proportional gain (kP)
                                             0.0003, // integral gain (kI)
                                             1.9, // derivative gain (kD)
                                             4, // anti windup
                                             0.25, // small error range, in degrees
                                             200, // small error range timeout, in milliseconds
                                             0.5, // large error range, in degrees
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