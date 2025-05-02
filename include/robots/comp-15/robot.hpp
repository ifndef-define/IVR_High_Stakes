#pragma once

// Add all robot header files here //
#include "robots/comp-15/arm.h"
#include "robots/comp-15/intake.h"
#include "robots/comp-15/pneumatics.h"
#include "robots/comp-15/climb.h"
#include "common/dualIMU.h"

// Robot Definitions //
extern pros::Controller ctrler;

extern Drive *chassis;

extern DualIMU imu;
extern odom *robotOdom;