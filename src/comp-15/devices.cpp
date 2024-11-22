#include "robots/comp-15/devices.h"

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
adi::Pneumatics clampPiston('G', false);
adi::Pneumatics doinker('H', false);

//Odometry
#define SMART_PORT 16
pros::adi::Encoder L_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'A', 'B'});
pros::adi::Encoder X_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'C', 'D'});
pros::adi::Encoder R_ENC(pros::adi::ext_adi_port_tuple_t{SMART_PORT, 'E', 'F'});

//Chassis
Chassis joner(&leftDrive, &rightDrive, &imuLeft, &R_ENC, &L_ENC,7,.0,2,
                                                                .0,.0,.0);
Arm arm(&armMotor, &armRot, 0.045, 0.0, 0.11);
Intake intake(&intakeMotor);