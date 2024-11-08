#pragma once
#include "main.h"

class PID {
    private:
        // patch for hole
        bool ranFirstLoop = false;
        // All private variables like PID constants, sensor and/or motor pointers
        struct PID_consts_s{
            double kP = 0;
            double kI = 0;
            double kD = 0;
            double outMax = 0;
        };
        PID_consts_s pid_consts_;

        // PID state variables like error, lastError, derivative, integral, etc
        // This struct can be used to store the state of the PID loop so if 
        // edge cases like saturation, etc can be handled by the user
        struct PID_state_s{
            double error = 0;
            double lastError = 0;
            double derivative = 0;
            double integral = 0;
            double rawOut = 0;
            bool saturated = false;
        };
        PID_state_s pid_state_;

    public:
        PID () = default;
        PID (double kP, double kI, double kD, double outMax);
        PID (PID_consts_s inpConsts, double outMax);
		PID (PID &other); // Copy constructor

        // Assignment operator overload for copying PID objects
        PID &operator=(PID & other);

		// A simple runPID method that takes in the target and current value
        double update(double target, double current);
        void setConstants(double kP, double kI, double kD, double outMax);

        // Getters and Setters for PID constants
        PID_consts_s getConstants();
        PID_state_s getState();
};

// class PID {
// private:
//   double p, i, d;
//   double accumulator;
//   double lastError;
//   double lastTime;

// public:
//   PID(double p = 0, double i = 0, double d = 0) : p{p}, i{i}, d{d}, accumulator{0}, lastError{0}, lastTime{-1} {}
//   void setCoefficients(double p, double i, double d);
//   double update(double error);
// };