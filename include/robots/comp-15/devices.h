#pragma once
#ifndef COMP_15_DEVICES_H
#define COMP_15_DEVICES_H

#include "common/includeList.h"

extern Controller ctrl_master;

// Motors //
extern MotorGroup rightDrive;
extern MotorGroup leftDrive;
extern MotorGroup intakeMotor;
extern Motor armMotor;

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
extern adi::Pneumatics mogoRush;
extern adi::Pneumatics clampPiston;
extern adi::Pneumatics doinker;

// Subssystems //
extern Chassis joner;
extern Arm arm;
extern Intake intake;

#endif // COMP_15_DEVICES_H