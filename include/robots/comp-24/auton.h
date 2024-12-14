#pragma once
#include "robots/comp-24/devices.h"
#include "robots/comp-24/includeList.h"

void runAuton(bool isBlue);
void redAuton();
void blueAuton();
void moveFwd(float dist, int timeout, float maxSpeed, float minSpeed);
float convertAngle(float angle);

void skillsCaller();