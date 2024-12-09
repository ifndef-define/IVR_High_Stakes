#pragma once
#include "robots/comp-24/devices.h"

class Intake
{
private:
    static pros::MotorGroup *intake;
    static bool isEjecting;
    static int pauseCounter1;
    static int pauseCounter2;
    static bool runColorSort;
    static bool colorToKeep; //0 for red, 1 for blue
    static bool autonControlFlag; //run intake in auton

public:
    Intake(pros::MotorGroup *intake);
    static void setVoltage(int volt);
    static void setRpm(int rpm);
    static void setRelative(double position, int rpm);
    static void brake();
    static void pullBack();
    static void manualControl();
    static void autonControl(int speed);
    static bool getIsEjecting();
    static void setColorToKeep(bool color);
    static void setAutonControlFlag(bool flag);
    static bool getAutonControlFlag();
    static void toggleColorSort();
    static void ringTask();
};