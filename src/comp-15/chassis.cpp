#include "comp-15/chassis.h"

Chassis::Chassis(pros::MotorGroup &leftDrive, pros::MotorGroup &rightDrive){
    leftDrv = &leftDrive;
    rightDrv = &rightDrive;
}

void Chassis::move(int pwr){
    leftDrv->move_voltage(pwr * 1000);
    rightDrv->move_voltage(pwr * 1000);
}

void Chassis::turn(int pwr){
    leftDrv->move_voltage(pwr * 1000);
    rightDrv->move_voltage(-pwr * 1000);
}




