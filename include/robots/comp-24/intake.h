#pragma once
#include "robots/comp-24/devices.h"

class Intake
{
private:
    static pros::MotorGroup *intake;
    static bool isEjecting;
    inline static bool colorToKeep = 0; //0 for red, 1 for blue
    static int pauseCounter1;
    static int pauseCounter2;

public:
    Intake(pros::MotorGroup *intake);
    static void setVoltage(int volt);
    static void setRpm(int rpm);
    static void setRelative(double position, int rpm);
    static void brake();
    static void pullBack();
    static void manualControl();
    static bool getIsEjecting();
    static void ringTask();
};