#pragma once
#include "robots/comp-15/devices.h"

class Intake
{
private:
    pros::MotorGroup *intake;
    bool isEjecting;
    int pauseCounter1;
    int pauseCounter2;

public:
    Intake(pros::MotorGroup *intake);
    void setVoltage(int volt);
    void setRpm(int rpm);
    void setRelative(double position, int rpm);
    void brake();
    void pullBack();
    void manualControl();
    bool getIsEjecting();
};