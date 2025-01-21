#pragma once
#include "robots/comp-15/devices.h"

class Intake
{
private:
    static pros::MotorGroup *intake;
    static bool isEjecting;
    static int pauseCounter1;
    static int pauseCounter2;
    inline static bool runColorSort;
    inline static bool colorToKeep; //0 for red, 1 for blue
    inline static bool autonControlFlag; //run intake in auton

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
    static bool getColorToKeep() {
        return colorToKeep;
    } //0 for red, 1 for blue
    static void setAutonControlFlag(bool flag);
    static bool getAutonControlFlag();
    static void toggleColorSort();
    static void ringTask();
    // Standard spin in rev to release intake
    static void releaseIntake(bool inv=false);
    // static void ringTaskNew();
};