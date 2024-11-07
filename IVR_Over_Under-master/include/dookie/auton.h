#pragma once
#include "dookie/include_list.h"
#include "dookie/devices.h"

// void auton18();
std::vector<double> vect(double x, double y);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor=3.0, double lookAheand=0.5, double turnP=1.12, double turnTol=3.0);
void turn(double angle);

void main_auton(bool wp);
// void win_point_auton();
// void non_win_point_auton();