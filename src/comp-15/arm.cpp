#include "robots/comp-15/arm.h"
Arm::Arm(float kP, float kI, float kD, float kP2, float kI2, float kD2) : armMotor(3, pros::MotorGears::red), armRot(-6), large(kP, kI, kD, 0, 0), small(kP2, kI2, kD2, 0, 0) {
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    armRot.set_position(-30);
}

void Arm::update(){
    // Get current limit switch state
    // currentLimitState = armLimit.get_value();
    
    // // Check if limit switch is NEWLY pressed (rising edge detection)
    // if(currentLimitState == 1 && prevLimitState == 0) {
        // Reset position to match DOWN state angle only when first pressed
        // armRot.set_position(10*100); // Multiply by 100 because getAngle divides by 100
    // }
    
    if(!override){ 
        error =  armStateAngles[(int)curArmState] - getAngle();
        if(abs(error) < 20){
            armMotor.move(small.update(error));
        } else {
            armMotor.move(large.update(error));
        } 
    } 
    // pros::lcd::print(0, "Arm State: %d", (int)curArmState);
    // pros::lcd::print(1, "Arm Angle: %f", getAngle());
    // Update previous limit switch state for next cycle
    // prevLimitState = currentLimitState;
}

void Arm::setState(State newState){ curArmState = newState; }

void Arm::nextState(){ curArmState = (State)((int(curArmState) + 1) % int(State::NUM_ARM_STATES)); }

void Arm::prevState(){ curArmState = (State)((int(curArmState) + (int(State::NUM_ARM_STATES) - 1)) % int(State::NUM_ARM_STATES)); }

void Arm::setSpeed(double speed){ 
    override = speed!=0; 
    armMotor.move(speed*127); 
}

double Arm::getAngle(){ return armRot.get_position()/100; }

Arm::State Arm::getState(){ return curArmState; }

void Arm::setBrakeMode(pros::motor_brake_mode_e_t mode){
    armMotor.set_brake_mode(mode);
}