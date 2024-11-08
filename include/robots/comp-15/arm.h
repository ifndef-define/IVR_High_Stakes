#pragma once
#include "devices.h"

class Arm : public PID {
    private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    PID armPID;
    
    public:
    Arm(pros::Motor &armM, pros::Rotation &armR, double kP, double kI, double kD);
    void setPosition(int pos);
};