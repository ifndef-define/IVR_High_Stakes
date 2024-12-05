#include "robots/comp-24/devices.h"

/*
Motor Ports
=============
Right Front: 3
Right Mid: 4
Right Back Top: -1
Right Back Bottom: 5
Left Front: -10
Left Mid: -9
Left Back Top: 7
Left Back Bottom: -8
Left Intake: 17
Right Intake: -13
Wall Arm: -14
Distance: 18
Optical: 15
IMU Left: 20
IMU Right: 11
Wall Arm Rotation: 21
X Enc: A-B
rY Enc: C-D
lY Enc: E-F
Mogo Mech: G
*/

Controller ctrl_master(E_CONTROLLER_MASTER);

//Motors
MotorGroup rightDrive({3, 4, -1, 5}, pros::MotorGears::blue);
MotorGroup leftDrive({-10, -9, 7, -8}, pros::MotorGears::blue);
MotorGroup intakeMotor({-13, 17}, pros::MotorGears::green);
Motor armMotor(-14, pros::MotorGears::red);

//Sensors
Distance intakeDist(18);
Optical intakeColor(15);
IMU imuLeft(20);
IMU imuRight(11);
Rotation armRot(-21);

//Pneumatics
adi::Pneumatics mogoRush('F', false);
adi::Pneumatics clampPiston('H', false);
adi::Pneumatics doinker('G', false);

//Odometry
// #define SMART_PORT 16
// pros::adi::Encoder L_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'A', 'B'});
// pros::adi::Encoder X_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'C', 'D'});
// pros::adi::Encoder R_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'E', 'F'});

adi::Encoder yEnc(adi::ext_adi_port_tuple_t(16, 3, 4));
adi::Encoder rxEnc(adi::ext_adi_port_tuple_t(16, 5, 6)); // 3 4
adi::Encoder lxEnc(adi::ext_adi_port_tuple_t(16, 1, 2), true); // 5 6

//Chassis
// Chassis joner(&leftDrive, &rightDrive, &imuLeft, &rxEnc, &lxEnc,7,.0,2,
//                                                                 .0,.0,.0);
Arm arm(&armMotor, &armRot, 0.045, 0.0, 0.11);
Intake intake(&intakeMotor);

// LEMLIB Config
const float VERT_RATIO = 1.9816176471;
const float HORI_RATIO = 1.9867647059;
lemlib::TrackingWheel vertical(&lxEnc, 1.36*VERT_RATIO, -1.125);
lemlib::TrackingWheel horizontal(&yEnc, 1.36*HORI_RATIO, -.375);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imuRight // inertial sensor
);

lemlib::Drivetrain drivetrain(&leftDrive, // left motor group
                              &rightDrive, // right motor group
                              11.825, // 11.825 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              480, // drivetrain rpm is 480
                              0 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(1, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            0, // derivative gain (kD)
                                            3, // anti windup
                                            .5, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            1, // large error range, in inches
                                            600, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller 2.45, 5.5
lemlib::ControllerSettings angularController(2.7, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             7, // derivative gain (kD)
                                             3, // anti windup
                                             .5, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             1, // large error range, in degrees
                                             680, // large error range timeout, in milliseconds
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