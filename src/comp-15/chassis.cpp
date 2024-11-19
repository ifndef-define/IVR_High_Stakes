#include "comp-15/chassis.h"

Chassis::Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive){
    leftDrv = &leftDrive;
    rightDrv = &rightDrive;
}

Chassis::Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive, double lkP, double lkI, double lkD, double tkP, double tkI, double tkD) : lPID(lkP, lkI, lkD, 127), tPID(tkP, tkI, tkD, 127) {
    leftDrv = &leftDrive;
    rightDrv = &rightDrive;
}

void Chassis::move(int pwr){
    leftDrv->move(pwr);
    rightDrv->move_voltage(pwr);
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
    int pwr = lPID.update(lat, 0);
    move(pwr);
}

void Chassis::turnPID(int angle){
    int pwr = tPID.update(angle, 0);
    turn(pwr);
}

void Chassis::moveToPID(int lat, int turn){
    int lPwr = lPID.update(lat, 0);
    int tPwr = tPID.update(turn, 0);
    tank(lPwr, tPwr);
}