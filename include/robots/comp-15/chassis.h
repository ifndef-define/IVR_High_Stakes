#pragma once
#include "robots/comp-15/devices.h"

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
            double kp, double ki, double kd, double kpTurn, double kiTurn, double kdTurn);
    void setConstants(double lkP, double lkI, double lkD, double outMax, double tkP, double tkI, double tkD, double outMax2);
    double getPosition();
    void move(int fwdPwr);
    void turn(int turnPwr);
    void tank(int fwdPwr, int turnPwr);
    void movePID(int lat);
    void turnPID(int angle);
    void moveToPID(int lat, int angle);
};