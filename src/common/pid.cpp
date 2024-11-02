#include "common/pid.h"

PID::PID (double kP, double kI, double kD, double outMax) {
    // setting constants seperatly with constructor
    pid_consts_.kP = kP; //proportion
    pid_consts_.kI = kI; //integral
    pid_consts_.kD = kD; //derivative
    pid_consts_.outMax = outMax; //used to determine saturation
}
PID::PID(PID_consts_s inpConsts, double outMax) {
    // setting constants the lazy way
    pid_consts_ = inpConsts;
    pid_consts_.outMax = outMax; //used to determine saturation
};
PID::PID (PID &other){
    pid_consts_ = other.pid_consts_;
}; // Copy constructor

// Assignment operator overload for copying PID objects
PID &PID::operator=(PID & other) {
    // Deep copy of parameters
    this->pid_consts_ = other.pid_consts_;
    this->pid_state_ = other.pid_state_;
    return *this;
};

void PID::setConstants(double kP, double kI, double kD, double outMax) {
    //setting constants
    pid_consts_.kP = kP;
    pid_consts_.kI = kI;
    pid_consts_.kD = kD;
    pid_consts_.outMax = outMax;
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
double PID::update(double target, double current) {
      
    pid_state_.error = target - current;  //Error is the target value minus the current value
    pid_state_.derivative = pid_state_.error - pid_state_.lastError;  //Get derivative by subtacting current error and last error
    pid_state_.integral += pid_state_.error;  //Add current error to integral

    if(!ranFirstLoop){    
        pid_state_.derivative = 0;
        ranFirstLoop = true;
    }
    
    if((pid_state_.error > 0 && pid_state_.lastError < 0) || (pid_state_.error < 0 && pid_state_.lastError > 0)) //integral clamp
        pid_state_.integral = 0;
    
    pid_state_.lastError = pid_state_.error;  //Set last error to what the error currently is

    pid_state_.rawOut = pid_consts_.kP * pid_state_.error + pid_consts_.kI * pid_state_.integral + pid_consts_.kD * pid_state_.derivative;
    if (std::fabs(pid_state_.rawOut) >  pid_consts_.outMax){ // checking for saturation
        pid_state_.saturated = true;
        pid_state_.rawOut = (pid_state_.rawOut > 0) ? pid_consts_.outMax : -pid_consts_.outMax;
    } else {
        pid_state_.saturated = false;
    }

    return pid_state_.rawOut;
}