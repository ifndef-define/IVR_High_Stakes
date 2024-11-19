#pragma once
#include "robots/comp-15/devices.h"
#include "common/pid.h"

class Chassis : public PID {
    private:
    pros::MotorGroup *leftDrv;
    pros::MotorGroup *rightDrv;
    PID lPID;
    PID tPID;

    public:
    Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive);
    Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive, double lkP, double lkI, double lkD, double tkP, double tkI, double tkD);
    void move(int pwr);
    void turn(int pwr);
    void tank(int fwd, int turn);
    void movePID(int lat);
    void turnPID(int angle);
    void moveToPID(int lat, int turn);
};