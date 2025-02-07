#include "robots/comp-15/Arm.h"

void Arm::update(Arm::armState_t newState){
    armState = newState;
    
}

void Arm::nextState(){

}

void Arm::prevState(){
    
}

double Arm::getPosition(){
    return armRot.get_position();
}

Arm::armState_t Arm::getState(){
    return armState;
}