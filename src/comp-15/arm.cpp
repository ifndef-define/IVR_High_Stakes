#include "robots/comp-15/arm.h"

Arm::Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD) : PID(kP, kI, kD, 127)
{
    armMotor = armM;
    armRot = armR;
    armState = 0;
    intakePullBackFlag = false;
}

int Arm::getState()
{
    return armState;
}

void Arm::setState(int state)
{
    armState = state;
}

void Arm::incrementArmState()
{
    if (armState == 1)
    {
        armState = 2;
        intakePullBackFlag = true;
    }
    else
    {
        armState++;
        if (armState > 3)
        {
            armState = 0;
        }
    }
}

void Arm::decrementArmState()
{
    armState--;
    if (armState < 0)
    {
        armState = 3;
    }
}

void Arm::updatePosition(){ 
    if(armState != 4){
        armMotor->move(update(targetPosition[armState-1], armRot->get_position()));
    } else if(armState == 4){
        if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
            if(armRot->get_position() >= 20000){
                armMotor->move(-20);
            } else {
                armMotor->move(127);
            }
        } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
            if(armRot->get_position() <= 0){
                armMotor->move(0);
            } else {
                armMotor->move(-127);
            }
        } else {
            armMotor->move(0);
        }
    }
}

void Arm::setIntakePullBackFlag(bool state)
{
    intakePullBackFlag = state;
}

bool Arm::getIntakePullBackFlag()
{
    return intakePullBackFlag;
}

void Arm::manualControl()
{
    if (!intake.getIsEjecting())
    {
        if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
        {
            armState = 4;
        }
        else if (ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
        {
            incrementArmState();
        }
        else if (ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
        {
            decrementArmState();
        }
    }

    updatePosition();
}