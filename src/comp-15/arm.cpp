#include "comp-15/arm.h"


Arm::Arm(pros::Motor &armM, pros::Rotation &armR){
    armMotor = &armM;
    armRot = &armR;
}

void setPosition(int pos){

}