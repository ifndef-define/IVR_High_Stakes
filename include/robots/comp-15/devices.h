#pragma once
#ifndef COMP_15_DEVICES_H
#define COMP_15_DEVICES_H

#include "common/includeList.h"
#include "robots/comp-15/includeList.h"
#include "robots/comp-15/arm.h"
#include "lemlib/api.hpp"

class Arm;  // Forward declaration of Arm
class Chassis;
class Intake;

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
// extern pros::adi::Encoder L_ENC;
// extern pros::adi::Encoder X_ENC;
// extern pros::adi::Encoder R_ENC;

extern pros::adi::Encoder yEnc;
extern pros::adi::Encoder rxEnc;
extern pros::adi::Encoder lxEnc;

// Pneumatics //
extern adi::Pneumatics mogoRushClamp;
extern adi::Pneumatics mogoRushReach;
extern adi::Pneumatics mogoClamp;
extern adi::Pneumatics doinker;
extern adi::Pneumatics intakeLift;

// Subsystems //
extern Chassis joner;
extern Arm arm;
extern Intake intake;

// LEMLIB Config //
extern lemlib::TrackingWheel horizontal;
extern lemlib::TrackingWheel vertical;

// sensors for odometry
extern lemlib::OdomSensors sensors;

// drivetrain
extern lemlib::Drivetrain drivetrain;

// lateral motion controller
extern lemlib::ControllerSettings linearController;

// angular motion controller
extern lemlib::ControllerSettings angularController;

// input curve for throttle input during driver control
extern lemlib::ExpoDriveCurve throttleCurve;

// input curve for steer input during driver control
extern lemlib::ExpoDriveCurve steerCurve;

// create the chassis
extern lemlib::Chassis chassis;

#endif // COMP_15_DEVICES_H