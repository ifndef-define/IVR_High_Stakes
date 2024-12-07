#include "PID.h"

//Default Constructor with motor min/max (-127, 127)
PID::PID(double kp, double ki, double kd){
    consts.kP = kp;
    consts.kI = ki;
    consts.kD = kd;
}

//Custom Constructor with custom min/max values
PID::PID(double kp, double ki, double kd, double min, double max){
    consts.kP = kp;
    consts.kI = ki;
    consts.kD = kd;
    consts.OUT_MIN = min;
    consts.OUT_MAX = max;
}

//Set kP
void PID::set_kP(double kp){
    consts.kP = kp;
}

//Set kI
void PID::set_kI(double ki){
    consts.kI = ki;
}

//Set kD
void PID::set_kD(double kd){
    consts.kD = kd;
}

//Set all constants
void PID::setConstants(double kp, double ki, double kd){
    consts.kP = kp;
    consts.kI = ki;
    consts.kD = kd;
}

//Get PID state values
PID::PID_state_s PID::getState(){
    return pid_state;
}

//Get PID constants
PID::consts_s PID::getConstants(){
    return consts;
}

//Update PID loop
double PID::updatePID(double target, double current, double tolerance, double error){
    if(error != 0)
        pid_state.error = error;
    else
        pid_state.error = target-current;
    
    // if(std::abs(pid_state.error) > tolerance && !pid_state.targetReached){
        pid_state.targetReached = false;
        pid_state.integral += pid_state.error;
        //Ensures integral doesn't get too large
        if((pid_state.error > 0 && pid_state.lastError < 0) || (pid_state.error < 0 && pid_state.lastError > 0))
            pid_state.integral = 0;
        //Update derivative and lastError
        pid_state.derivative = pid_state.error - pid_state.lastError;
        pid_state.lastError = pid_state.error; 
        pid_state.output = consts.kP * pid_state.error + consts.kI * pid_state.integral + consts.kD * pid_state.derivative;
        //Ensures output doesn't exceed max/min (saturation)
        if(pid_state.output > consts.OUT_MAX){
            pid_state.output = consts.OUT_MAX;
            pid_state.saturated = true;
        } else if(pid_state.output < consts.OUT_MIN){
            pid_state.output = consts.OUT_MIN;
            pid_state.saturated = true;
        } else {
            pid_state.saturated = false;
        }
        pid_state.lastOutput = pid_state.output;
        // delay(20);
        if(std::abs(pid_state.error) < tolerance){
            pid_state.targetReached = true;
        } else {
            pid_state.targetReached = false;
        }
        return pid_state.output;
    // } else {
    //     pid_state.targetReached = true;
    //     return 0;
    // }
}

//Reset PID loop
void PID::resetPID(){
    pid_state.error = 0;
    pid_state.lastError = 0;
    pid_state.derivative = 0;
    pid_state.integral = 0;
    pid_state.output = 0;
    pid_state.lastOutput = 0;
    pid_state.saturated = false;
    pid_state.targetReached = false;
}
