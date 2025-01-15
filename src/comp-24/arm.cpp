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

/**
 * Normalize angle to be between -180 and 180 degrees
 *
 * This function takes a angle in degrees and normalizes it to be between -180
 * and 180 degrees. This is useful for calculating the difference between two
 * angles, as it will give the shortest distance between the two angles.
 *
 * @param angle angle to be normalized (centidegrees)
 * @return normalized angle (-180, 180]
 */
double Arm::normalizeAngle(double angle){
    return (angle/100 - (std::floor((angle/100 + 180.0)/360.0))*360.0);
}

double Arm::getNormalizedAngle(){
    return normalizeAngle(armRot->get_position());
}

void Arm::manualControl(){
    // Move arm to 23000 when L2 is held, return to 0 when released
    if(!ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            armState = 3;
        } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            armState = 1;
        } else {
            armState = 0;
        }
        armMotor->move(armPID.update(targetPosition[armState], getNormalizedAngle()));
    } else {
        if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && getNormalizedAngle() < targetPosition[2]-5) {
            armMotor->move(127);
        } else if (getNormalizedAngle() > targetPosition[2]+5) {
            armMotor->move(armPID.update(targetPosition[2], getNormalizedAngle()));
        } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            armMotor->move(-127);
        } else {
            armMotor->move(0);
        }
    }

    // Monitor arm position to set intakeArmFlag
    if (!armFlag && armRot->get_position() > 2000) {
        armFlag = true;
        intakePullBackFlag = true;
    }
    if (armRot->get_position() < 1700) {
        armFlag = false;
    }
}

bool Arm::setPosition(int pos, bool async){
    armPID.reset();
    if (async) {
        pros::Task task([&]() { setPosition(pos, false); });
        pros::delay(10); // delay to give the task time to start
        return armPID.getState().reachedTarget;
    }
    while(!armPID.getState().reachedTarget){
        armMotor->move(armPID.update(pos, getNormalizedAngle()));
    }
    return armPID.getState().reachedTarget;
}