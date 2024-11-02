#pragma once
#ifndef COMP_15_DEVICES_H
#define COMP_15_DEVICES_H

#include "common/includeList.h"

extern Controller ctrl_master;

// Motors //
extern MotorGroup rightDrive;
extern MotorGroup leftDrive;
extern MotorGroup intake;
extern MotorGroup arm;

extern Distance mogoDist;
extern Distance intakeDist;
extern Optical intakeColor;
extern IMU imuLeft;
extern IMU imuRight;
extern Rotation armRot;

extern adi::Encoder xEnc;
extern adi::Encoder YEnc; //YEncR
// ADIPneumatics YEncL;
extern adi::Pneumatics clamp;

#endif // COMP_15_DEVICES_H