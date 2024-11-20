#pragma once
#include "robots/comp-15/devices.h"
#include "common/pid.h"

class Chassis : public PID {
    private:
    pros::MotorGroup *leftDrv;
    pros::MotorGroup *rightDrv;
    pros::IMU *imuI;
    pros::ADIEncoder *rEnc;
    pros::ADIEncoder *lEnc;

    PID lPID;
    PID tPID;

    public:
    Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive, pros::IMU &imu, pros::ADIEncoder &R_ENC, pros::ADIEncoder &L_ENC);
    Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive, pros::IMU &imu, pros::ADIEncoder &R_ENC, pros::ADIEncoder &L_ENC, double lkP, double lkI, double lkD, double tkP, double tkI, double tkD);
    void setConstants(double kP, double kI, double kD, double outMax, double kP2, double kI2, double kD2, double outMax2);
    double getPosition();
    void move(int fwdPwr);
    void turn(int turnPwr);
    void tank(int fwdPwr, int turnPwr);
    void movePID(int lat);
    void turnPID(int angle);
    void moveToPID(int lat, int angle);
};