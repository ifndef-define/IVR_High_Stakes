#pragma once

// Add all robot header files here //
#include "robots/comp-24/arm.h"
#include "robots/comp-24/intake.h"
#include "robots/comp-24/pneumatics.h"
#include "robots/comp-24/climb.h"
#include "common/dualIMU.h"

// Robot Definitions //
extern pros::Controller ctrler;

extern Drive *chassis;

extern odom robotOdom;
extern DualIMU imu;
extern pros::IMU imu2;
