#pragma once
#include "robots/comp-15/devices.h"

class Arm{
private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    int armState;
    int targetPosition[4] = {0, 1600, 11000, 14000};
    bool intakePullBackFlag;
    PID armPID;

public:
    Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD);
    void setPosition(int pos);
    int getState();
    void setState(int state);
    void updatePosition();
    void incrementArmState();
    void decrementArmState();
    void setIntakePullBackFlag(bool state);
    bool getIntakePullBackFlag();
    void manualControl();
};