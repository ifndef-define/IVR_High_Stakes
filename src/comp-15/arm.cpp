#include "robots/comp-15/Arm.h"

Arm::Arm() {
    armMotor = new pros::Motor(3);
    armRot = new pros::Rotation(-17);
}

void Arm::moveToState(Arm::armState_t newState){
    
}

void Arm::nextState(){
    
}

void Arm::prevState(){

}

double Arm::getPosition(){

}

Arm::armState_t Arm::getState(){

}