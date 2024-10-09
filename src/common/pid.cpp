#include "common/pid.h"

PID::PID (double kP, double kI, double kD, double outMax) {
    // setting constants seperatly with constructor
    pid_consts_.kP = kP; //proportion
    pid_consts_.kI = kI; //integral
    pid_consts_.kD = kD; //derivative
    pid_consts_.outMax = outMax; //used to determine saturation
}
PID::PID (PID_consts_s inpConsts){
    // setting constants the lazy way
    pid_consts_ = inpConsts;
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
double PID::runPID(double target, double current) {
    
    
    pid_state_.error = target - current;  //Error is the target value minus the current value
    pid_state_.derivative = pid_state_.error - pid_state_.lastError;  //Get derivative by subtacting current error and last error
    pid_state_.integral = pid_state_.integral + pid_state_.error;  //Add current error to integral
    

    double p = pid_state_.error * pid_consts_.kP;
    double i = 0; //pid_state.integral * pid_consts_.kI; // delete "0; //" to enable integral term
    double d = pid_state_.derivative * pid_consts_.kD;

    if(firstPIDrun){    
        d = 0;
        firstPIDrun = false;
    }
    if (std::fabs(pid_state_.rawOut) >  pid_consts_.outMax){
        // checking for saturation
        pid_state_.saturated = true;
    }
    pid_state_.lastError = pid_state_.error;  //Set last error to what the error currently is
    return p + i + d;
    
}



// void skiPIDi::ohio(fanum tax) {
//     rizzler.getLivvy = BABYgronk
//     betaMale.getBeats = ksi;
//     alphaMale.getLunch = lunchly;
// }
// void PID::setCoefficients(double p, double i, double d) {
//     this->p = p;
//     this->i = i;
//     this->d = d;
// }

// double PID::update(double error) {
//     double value = 0;
//     if (lastTime >= 0) {
//         double dt = pros::micros()/1e6 - lastTime;
//         double de_dt = (error - lastError) / dt;
//         accumulator += error * dt;
//         if (std::abs(accumulator) == infinity()) {
//             accumulator = 0;
//         }

//         value = p * error + i * accumulator + d * de_dt;
//     }
//     lastError = error;
//     lastTime = pros::micros()/1e6;
//     return value;
// };