#pragma once
#ifndef COMP_24_DEVICES_H
#define COMP_24_DEVICES_H

#include "common/includeList.h"
// #include "lemlib/api.hpp"
#include "robots/comp-24/pneumatics.h"
#include "robots/comp-24/action.h"
#include "common/dual_imu.h"

class Action;
extern Action actions;

// Add all robot device externs here //
extern pros::Controller ctrler;
extern pros::MotorGroup leftDrive;
extern pros::MotorGroup rightDrive;

extern PneumaticsGroup pneumatics;

// extern IMU imu1;
#ifdef ENABLE_DUAL_IMU
extern DualIMU imu;
#else
extern pros::Imu imu;
#endif

extern pros::adi::Encoder yEnc;
extern pros::adi::Encoder rxEnc;

// // LEMLIB Config //
// extern lemlib::TrackingWheel horizontal;
// extern lemlib::TrackingWheel vertical;

// // sensors for odometry
// extern lemlib::OdomSensors sensors;

// // drivetrain
// extern lemlib::Drivetrain drivetrain;

// // lateral motion controller
// extern lemlib::ControllerSettings linearController;

// // angular motion controller
// extern lemlib::ControllerSettings angularController;

// // input curve for throttle input during driver control
// extern lemlib::ExpoDriveCurve throttleCurve;

// // input curve for steer input during driver control
// extern lemlib::ExpoDriveCurve steerCurve;

// // create the chassis
// extern lemlib::Chassis chassis;

#endif // COMP_15_DEVICES_H