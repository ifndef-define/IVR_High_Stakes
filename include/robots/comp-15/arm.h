#pragma once
#include "robots/comp-15/devices.h"

class Arm : public PID
{
private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    PID armPID;
    int armState;
    bool intakePullBackFlag;

public:
    Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD);
    void setPosition(int pos);
    int getState();
    void setState(int state);
    void incrementArmState();
    void decrementArmState();
    void setIntakePullBackFlag(bool state);
    bool getIntakePullBackFlag();
    void manualControl();
};