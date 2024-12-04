#include "robots/comp-24/chassis.h"

// Constructor without PID constants
Chassis::Chassis(pros::MotorGroup *leftDrive, pros::MotorGroup *rightDrive, pros::IMU *imu, pros::adi::Encoder *R_ENC, pros::adi::Encoder *L_ENC)
    : lPID(0, 0, 0, 127), tPID(0, 0, 0, 127) {
    leftDrv = leftDrive;
    rightDrv = rightDrive;
    imuI = imu;
    rEnc = R_ENC;
    lEnc = L_ENC;
}

// Constructor with PID constants
Chassis::Chassis(pros::MotorGroup *leftDrive, pros::MotorGroup *rightDrive, pros::IMU *imu, pros::adi::Encoder *R_ENC, pros::adi::Encoder *L_ENC,
                 double lkP, double lkI, double lkD, double tkP, double tkI, double tkD)
    : lPID(lkP, lkI, lkD, 127), tPID(tkP, tkI, tkD, 127) {
    leftDrv = leftDrive;
    rightDrv = rightDrive;
    imuI = imu;
    rEnc = R_ENC;
    lEnc = L_ENC;
}

void Chassis::setConstants(double lkP, double lkI, double lkD, double outMax, double tkP, double tkI, double tkD, double outMax2){
    lPID.setConstants(lkP, lkI, lkD, outMax);
    tPID.setConstants(tkP, tkI, tkD, outMax2);
}

double Chassis::getPosition(){
    return ((rEnc->get_value() + lEnc->get_value()) / 2)*(M_PI * 1.36) / 8192;
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
    // lPID.reset();
    // while(fabs(lPID.getState().derivative) <= .1){
        move(lPID.update(lat, getPosition()));
        // pros::delay(15);
    // }
    // move(0);
}

void Chassis::turnPID(int angle){
    tPID.reset();
    while(abs(tPID.getState().derivative) <= .1){
        turn(tPID.update(angle, imuI->get_roll()));
        pros::delay(15);
    }
    move(0);
}

void Chassis::moveToPID(int lat, int turn){
    lPID.reset();
    tPID.reset();
    while(abs(tPID.getState().derivative) <= .1 && abs(lPID.getState().derivative) <= .1){
        tank(lPID.update(lat, getPosition()), tPID.update(turn, imuI->get_roll()));
        pros::delay(15);
    }
    move(0);
}