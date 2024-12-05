#pragma once
#include "robots/comp-24/devices.h"
#include "common/pid.h"

class Chassis {
    private:
    pros::MotorGroup *leftDrv;
    pros::MotorGroup *rightDrv;
    pros::IMU *imuI;
    pros::adi::Encoder *rEnc;
    pros::adi::Encoder *lEnc;

    PID lPID;
    PID tPID;

    public:
    Chassis(pros::MotorGroup *leftDrive, pros::MotorGroup *rightDrive, pros::IMU *imuLeft,
            pros::adi::Encoder *rEnc, pros::adi::Encoder *lEnc);
    Chassis(pros::MotorGroup *leftDrive, pros::MotorGroup *rightDrive, pros::IMU *imuLeft,
            pros::adi::Encoder *rEnc, pros::adi::Encoder *lEnc,
            double kpLat, double kiLat, double kdLat, double lExitRange, double kpTurn, double kiTurn, double kdTurn, double tExitRange);
    void setConstants(double lkP, double lkI, double lkD, double outMax, double lExitRange, double tkP, double tkI, double tkD, double outMax2, double tExitRange);
    double getPosition();
    void move(int fwdPwr);
    void turn(int turnPwr);
    void tank(int fwdPwr, int turnPwr);
    void movePID(int lat);
    void turnPID(int angle);
    void moveToPID(int lat, int angle);
};