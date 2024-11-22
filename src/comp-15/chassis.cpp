#include "comp-15/chassis.h"

Chassis::Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive, pros::IMU &imu, pros::ADIEncoder &R_ENC, pros::ADIEncoder &L_ENC){
    leftDrv = &leftDrive;
    rightDrv = &rightDrive;
    imuI = &imu;
    rEnc = &R_ENC;
    lEnc = &L_ENC;
}

Chassis::Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive, pros::IMU &imu, pros::ADIEncoder &R_ENC, pros::ADIEncoder &L_ENC, double lkP, double lkI, double lkD, double tkP, double tkI, double tkD) : lPID(lkP, lkI, lkD, 127), tPID(tkP, tkI, tkD, 127) {
    leftDrv = &leftDrive;
    rightDrv = &rightDrive;
    imuI = &imu;
    rEnc = &R_ENC;
    lEnc = &L_ENC;
}

void Chassis::setConstants(double kP, double kI, double kD, double outMax, double kP2, double kI2, double kD2, double outMax2){
    lPID.setConstants(kP, kI, kD, outMax);
    tPID.setConstants(kP2, kI2, kD2, outMax2);
}

double Chassis::getPosition(){
    return (rEnc->get_value() + lEnc->get_value()) / 2;
}

void Chassis::move(int pwr){
    leftDrv->move(pwr);
    rightDrv->move(pwr);
}

void Chassis::turn(int pwr){
    leftDrv->move(pwr);
    rightDrv->move(-pwr);
}

void Chassis::tank(int fwd, int turn){
    leftDrv->move(fwd + turn);
    rightDrv->move(fwd - turn);
}

void Chassis::movePID(int lat){
    lPID.reset();
    while(abs(lPID.getState().derivative) <= .01){
        move(lPID.update(lat, 0));
        pros::delay(15);
    }
    move(0);
}

void Chassis::turnPID(int angle){
    tPID.reset();
    while(abs(tPID.getState().derivative) <= .01){
        turn(tPID.update(angle, imuI->get_roll()));
        pros::delay(15);
    }
    move(0);
}

void Chassis::moveToPID(int lat, int turn){
    lPID.reset();
    tPID.reset();
    while(abs(tPID.getState().derivative) <= .01 && abs(lPID.getState().derivative) <= .01){
        tank(lPID.update(lat, getPosition()), tPID.update(turn, imuI->get_roll()));
        pros::delay(15);
    }
    move(0);
}