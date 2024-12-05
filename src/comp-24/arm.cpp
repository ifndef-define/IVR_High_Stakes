#include "robots/comp-24/arm.h"

Arm::Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD, double exitRange) : armPID(kP, kI, kD, 127, exitRange)
{
    armMotor = armM;
    armRot = armR;
    armState = 0;
    intakePullBackFlag = false;
    armFlag = false;
}

int Arm::getState()
{
    return armState;
}

void Arm::setState(int state)
{
    armState = state;
}

void Arm::setIntakePullBackFlag(bool state)
{
    intakePullBackFlag = state;
}

bool Arm::getIntakePullBackFlag()
{
    return intakePullBackFlag;
}

void Arm::manualControl(){
    // if(!intake.getIsEjecting){
        // Move arm to 23000 when L2 is held, return to 0 when released
        if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            armState = 2;
        } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            armState = 1;
        } else {
            armState = 0;
        }
        armMotor->move(armPID.update(targetPosition[armState], armRot->get_position()));
    // } else {
    //     if(armState > 1){
    //         armMotor->move(armPID.update(6000, armRot->get_position()));
    //     } else {
    //         armState = 0;
    //     }
    // }

    // Monitor arm position to set intakeArmFlag
    if (!armFlag && armRot->get_position() > 1800) {
        armFlag = true;
        intakePullBackFlag = true;
    }
    if (armRot->get_position() < 1600) {
        armFlag = false;
    }
}