#pragma once

// Add all robot header files here //
#include "robots/comp-15/arm.h"
#include "robots/comp-15/intake.h"
#include "robots/comp-15/pneumatics.h"

// Robot Definitions //
extern pros::Controller ctrler;

extern drive *chassis;
extern drive *climbChassis;
extern odom *robotOdom;