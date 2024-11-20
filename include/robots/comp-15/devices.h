#pragma once
#ifndef COMP_15_DEVICES_H
#define COMP_15_DEVICES_H

#include "common/includeList.h"

extern Controller ctrl_master;

// Motors //
extern MotorGroup rightDrive;
extern MotorGroup leftDrive;
extern MotorGroup intake;
extern Motor arm;

// Sensors //
extern Distance intakeDist;
extern Optical intakeColor;
extern IMU imuLeft;
extern IMU imuRight;
extern Rotation armRot;

// Odometry
extern pros::adi::Encoder L_ENC;
extern pros::adi::Encoder X_ENC;
extern pros::adi::Encoder R_ENC;

// Pneumatics //
extern adi::Pneumatics mogoRush('F', false);
extern adi::Pneumatics clamp('G', false);
extern adi::Pneumatics doinker('H', false);

// Chassis //
extern Chassis joner;

#endif // COMP_15_DEVICES_H