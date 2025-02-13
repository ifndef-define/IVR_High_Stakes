#pragma once
#ifndef COMP_24_DEVICES_H
#define COMP_24_DEVICES_H

#include "common/includeList.h"
#include "pnuematics.h"

// Add all robot device externs here //
extern pros::Controller ctrler;
extern pros::MotorGroup leftDrive;
extern pros::MotorGroup rightDrive;

extern PneumaticsGroup pneumatics;

#endif // COMP_15_DEVICES_H