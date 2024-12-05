#include "common/pid.h"

PID::PID (double kP, double kI, double kD, double outMax, double exitRange) {
    // setting constants seperatly with constructor
    consts.kP = kP; //proportion
    consts.kI = kI; //integral
    consts.kD = kD; //derivative
    consts.outMax = outMax; //used to determine saturation
}
PID::PID(PID_consts_s inpConsts, double outMax, double exitRange) {
    // setting constants the lazy way
    consts = inpConsts;
    consts.outMax = outMax; //used to determine saturation
    consts.exitRange = exitRange; //used to determine if the target has been reached
};
PID::PID (PID &other){
    consts = other.consts;
}; // Copy constructor

// Assignment operator overload for copying PID objects
PID &PID::operator=(PID & other) {
    // Deep copy of parameters
    this->consts = other.consts;
    this->state = other.state;
    return *this;
};

void PID::setConstants(double kP, double kI, double kD, double outMax, double exitRange) {
    //setting constants
    consts.kP = kP;
    consts.kI = kI;
    consts.kD = kD;
    consts.outMax = outMax;
    consts.exitRange = exitRange;
}

/**
 * @brief About the PID calculation method
 * 
 * What to do per PID loop:
 * - Calculate the error ✔️
 * - Calculate the integral ✔️
 *   - Check for integral windup               
 * - Calculate the derivative ✔️ 
 * - Calculate the output ✔️ 
 *   - Determine if saturated
 * - Update the last error ✔️ 
 */
float PID::update(double target, double current) {
    state.error = target - current;  //Error is the target value minus the current value
    state.derivative = state.error - state.lastError;  //Get derivative by subtacting current error and last error
    state.integral += state.error;  //Add current error to integral
    if(!ranFirstLoop){    
        PID::reset();
        ranFirstLoop = true;
    }
    if((state.error > 0 && state.lastError < 0) || (state.error < 0 && state.lastError > 0)) //integral clamp
        state.integral = 0;
    state.lastError = state.error;  //Set last error to what the error currently is
    state.rawOut = consts.kP * state.error + consts.kI * state.integral + consts.kD * state.derivative;
    if (std::fabs(state.rawOut) >  consts.outMax){ // checking for saturation
        state.saturated = true;
        state.rawOut = (state.rawOut > 0) ? consts.outMax : -consts.outMax;
    } else {
        state.saturated = false;
    }
    if(fabs(state.derivative) <= consts.exitRange){
        state.reachedTarget = true;
    }
    return state.rawOut;
}

void PID::reset() {
    state.error = 0;
    state.integral = 0;
    state.lastError = 0;
    state.saturated = false;
    state.derivative = 0;
    state.rawOut = 0;
    state.reachedTarget = false;
    ranFirstLoop = false;
}