#pragma once
#include "robots/comp-24/devices.h"

class Intake
{
private:
    static pros::MotorGroup *intake;
    static bool isEjecting;
    static int pauseCounter1;
    static int pauseCounter2;
    inline static bool runColorSort;
    inline static bool colorToKeep; //0 for red, 1 for blue

public:
    Intake(pros::MotorGroup *intake);
    static void setVoltage(int volt);
    static void setRpm(int rpm);
    static void setRelative(double position, int rpm);
    static void brake();
    static void pullBack();
    static void manualControl();
    static bool getIsEjecting();
    static void setColorToKeep(bool color);
    static void toggleColorSort();
    static void ringTask();
};