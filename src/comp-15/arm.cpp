#include "robots/comp-15/arm.h"

Arm::Arm(pros::Motor *armM, pros::Rotation *armR, double kP, double kI, double kD, double exitRange) : armPID(kP, kI, kD, 127, exitRange)
{
    armMotor = armM;
    armRot = armR;
    lastArmState = 0;
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

void Arm::incrementArmState()
{
    // if (armState == 2)
    // {
    //     armState = 3;
    //     intakePullBackFlag = true;
    // }
    // else
    // {
        armState++;
        if (armState > 2)
        {
            armState = 0;
        }
    // }
}

void Arm::decrementArmState()
{
    armState--;
    if (armState < 0)
    {
        armState = 2;
    }
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

// void Arm::updatePosition(){ 
//     if(armState != 4){
//         armMotor->move(armPID.update(targetPosition[armState], armRot->get_position()));
//     } else if(armState == 4){
//         if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
//             if(armRot->get_position() >= 20000){
//                 armMotor->move(-20);
//             } else {
//                 armMotor->move(127);
//             }
//         } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
//             if(armRot->get_position() <= 0){
//                 armMotor->move(0);
//             } else {
//                 armMotor->move(-127);
//             }
//         } else {
//             armMotor->move(0);
//         }
//     }
// }

void Arm::manualControl(){
    // if(!intake.getIsEjecting){
        // Move arm to 23000 when L2 is held, return to 0 when released
        if(!ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            if(ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
                if (armState != 3) {
                    lastArmState = armState;
                    armState = 3;
                } 
                else {
                    // armState = lastArmState;
                    armState = 0;
                }
                armMotor->move(armPID.update(targetPosition[armState], getNormalizedAngle()));
            } else if(ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
                incrementArmState();
            }
            if(armState >= 0){
                armMotor->move(armPID.update(targetPosition[armState], getNormalizedAngle()));
            }
        } else {
            if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && getNormalizedAngle() < targetPosition[2]) {
                armMotor->move(127);
            } else if (getNormalizedAngle() > targetPosition[2]+10) {
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
    if (armRot->get_position() < 1600) {
        armFlag = false;
    }
}


void Arm::setIntakePullBackFlag(bool state){
    intakePullBackFlag = state;
}

bool Arm::getIntakePullBackFlag(){
    return intakePullBackFlag;
}

// void Arm::manualControl(){
//     if (!intake.getIsEjecting()){
//         if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
//             armState = 4;
//         }
//         else if (ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
//             incrementArmState();
//         }
//         else if (ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
//             decrementArmState();
//         }
//         // Monitor arm position to set intakeArmFlag
//         if (!armFlag && armRot->get_position() > 1800) {
//             armFlag = true;
//             intakePullBackFlag = true;
//         }
//         if (armRot->get_position() < 1600) {
//             armFlag = false;
//         }
//     } else {
//         if(armState > 1){
//             armState = 2;
//         } else {
//             armState = 0;
//         }
//     }
//     updatePosition();
// }
void Arm::manualControl(){
        // Move arm to 23000 when L2 is held, return to 0 when released
    if(!ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            armState = 2;
        } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            armState = 1;
        } else {
            armState = 0;
        }
        armMotor->move(armPID.update(targetPosition[armState], 
                        normalizeAngle(armRot->get_position())));
    } else {
        if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && normalizeAngle(armRot->get_position()) < targetPosition[2]) {
            armMotor->move(127);
        } else if (getNormalizedAngle() > targetPosition[2]+5) {
            armMotor->move(armPID.update(targetPosition[2], normalizeAngle(armRot->get_position())));
        } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            armMotor->move(-127);
        } else {
            armMotor->move(0);
        }   
    }
}

// void Arm::manualControl(){
//     if(!intake.getIsEjecting){
//         // Move arm to 23000 when L2 is held, return to 0 when released
//         if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
//             armMotor->move(armPID.update(23000, armRot->get_position()));
//         } else if(ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
//             armMotor->move(armPID.update(1600, armRot->get_position()));
//         } else {
//             armMotor->move(armPID.update(0, armRot->get_position()));
//         }
//     }

//     // Monitor arm position to set intakeArmFlag
//     if (!armFlag && armRot->get_position() > 1800) {
//         armFlag = true;
//         intakePullBackFlag = true;
//     }
//     if (armRot->get_position() < 1600) {
//         armFlag = false;
//     }
// }