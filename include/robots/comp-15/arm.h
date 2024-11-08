#pragma once
#include "devices.h"
#include "common/pid.h"

class Arm{
    private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    PID armPID( 0.0, 0.0, 0.0, 127.0);

    public:
    Arm(pros::Motor &armM, pros::Rotation &armR);
    void setPosition(int pos)
};