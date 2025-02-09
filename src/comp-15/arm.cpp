#include "robots/comp-15/arm.h"

void Arm::update(){ 
    if (!override)
        armMotor.move(armPID.update(armStateAngles[int(curArmState)])-getAngle()); 
}


void Arm::nextState(){ curArmState = (State)((int(curArmState) + 1) % int(State::NUM_ARM_STATES)); }

void Arm::prevState(){ curArmState = (State)((int(curArmState) + int(State::NUM_ARM_STATES) - 1) % int(State::NUM_ARM_STATES)); }

void Arm::setState(State newState){ curArmState = newState; }

void Arm::setSpeed(int speed){ 
    armMotor.move(speed); 
    override = (speed)>0;
}

double Arm::getAngle(){ return armRot.get_position()*100; }

Arm::State Arm::getState(){ return curArmState; }
