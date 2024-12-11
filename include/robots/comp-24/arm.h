#pragma once
#include "robots/comp-24/devices.h"

class Arm{
private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    int armState;
    // int targetPosition[3] = {0, 1600, 14000};
    int targetPosition[4] = {-1, 10, 20, 140};
    bool intakePullBackFlag;
    bool armFlag;
    PID armPID;

public:
    Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD, double exitRange);
    void setPosition(int pos);
    int getState();
    void setState(int state);
    void setIntakePullBackFlag(bool state);
    bool getIntakePullBackFlag();
    void manualControl();
};