#pragma once
#include "robots/comp-24/devices.h"
#include "robots/comp-24/includeList.h"

void runAuton(bool isBlue);
void redAuton();
void blueAuton();
void sift(float dist = 5, int n = 3, int timeout = 1500, float maxSpeed = 127, float minSpeed = 0);
void moveFwd(float dist, int timeout, float maxSpeed, float minSpeed);