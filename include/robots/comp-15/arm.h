#pragma once
#include "robots/comp-15/devices.h"

class Arm{
private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    int armState;
    int lastArmState;
    // int targetPosition[3] = {0, 1600, 14000};
    double targetPosition[4] = {-.5, 18, 26, 140};
    bool intakePullBackFlag;
    bool armFlag;
    PID armPID;

public:
    Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD, double exitRange);
    void setPosition(int pos);
    int getState();
    double normalizeAngle(double angle);
    double getNormalizedAngle();
    void setState(int state);
    void updatePosition();
    void incrementArmState();
    void decrementArmState();
    void setIntakePullBackFlag(bool state);
    bool getIntakePullBackFlag();
    void manualControl();
};