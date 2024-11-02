#include "robots/comp-15/devices.h"
#include "robots/comp-15/includeList.h"

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

MotorGroup rightDrive{3, 4, -1, 5};
MotorGroup leftDrive{-10, -9, 7, -8};
MotorGroup intake{-13, 17};
MotorGroup arm{-14};

// Distance mogoDist(6);
Distance intakeDist(18);
Optical intakeColor(15);
IMU imuLeft(20);
IMU imuRight(11);
Rotation armRot{21};

adi::Encoder xEnc('A', 'B');
adi::Encoder YEnc('C', 'D'); //YEncR
// adi::Pneumatics YEncL{E, F};
adi::Pneumatics clamp('G', false);