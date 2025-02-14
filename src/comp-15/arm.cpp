#include "robots/comp-15/arm.h"
Arm::Arm(float kP, float kI, float kD, float kP2, float kI2, float kD2) : armMotor(-11, pros::MotorGears::red), armRot(-17), large(kP, kI, kD, 0, 0), small(kP2, kI2, kD2, 0, 0) {
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void Arm::update(){
    if(!override){ 
        error = armStateAngles[(int)curArmState] - getAngle();
        if(abs(error) < 20){
            armMotor.move(small.update(error));
        } else {
            armMotor.move(large.update(error));
        }
    }
}

void Arm::setState(State newState){ curArmState = newState; }

void Arm::nextState(){ curArmState = (State)((int(curArmState) + 1) % int(State::NUM_ARM_STATES)); }

void Arm::prevState(){ curArmState = (State)((int(curArmState) + (int(State::NUM_ARM_STATES) - 1)) % int(State::NUM_ARM_STATES)); }

void Arm::setSpeed(int speed){ 
    override = speed!=0; 
    armMotor.move(speed*127); 
}

double Arm::getAngle(){ return armRot.get_position()/100; }
Arm::State Arm::getState(){ return curArmState; }