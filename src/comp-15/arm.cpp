#include "robots/comp-15/arm.h"

void Arm::update(){

}

void Arm::setState(armState_t newState) { armState = newState; }

double Arm::getAngle(){ return armRot.get_position()*100; }

Arm::armState_t Arm::getState(){ return armState; }
