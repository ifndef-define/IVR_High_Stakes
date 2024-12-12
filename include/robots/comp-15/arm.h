#pragma once
#include "robots/comp-15/devices.h"

class Arm{
private:
    pros::Motor *armMotor;
    pros::Rotation *armRot;
    int armState;
    int lastArmState;
    // int targetPosition[4] = {0, 2000, 11400, 14400};
    double targetPosition[4] = {-1.5, 10, 14, 140};
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