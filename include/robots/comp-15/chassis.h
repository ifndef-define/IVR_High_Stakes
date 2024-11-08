#pragma once
#include "robots/comp-15/devices.h"

class Chassis{
    private:
    pros::MotorGroup *leftDrive;
    pros::MotorGroup *rightDrive;

    public:
    Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive);
    void move(int pwr);
    void turn(int pwr);
};