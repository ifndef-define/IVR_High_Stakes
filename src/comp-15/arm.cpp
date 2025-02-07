#include "robots/comp-15/arm.h"

void Arm::update(){ armMotor.move(armPID.update(armStateAngles[armState])); }

void Arm::setState(armState_t newState){ armState = newState; }

void Arm::nextState(){ armState = (armState_t)((armState + 1) % NUM_ARM_STATES); }

void Arm::prevState(){ armState = (armState_t)((armState + NUM_ARM_STATES - 1) % NUM_ARM_STATES); }

double Arm::getAngle(){ return armRot.get_position()*100; }

Arm::armState_t Arm::getState(){ return armState; }
