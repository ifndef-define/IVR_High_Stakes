#include "common/pid.h"
#include "pros/rtos.hpp"

void PID::setCoefficients(double p, double i, double d) {
    this->p = p;
    this->i = i;
    this->d = d;
}

double PID::update(double error) {
    if (lastTime >= 0) {
        double dt = pros::millis()/1e3 - lastTime;
        double de_dt = (error - lastError) / dt;
        accumulator += error * dt;

        return p * error + i * accumulator + d * de_dt;
    }
    lastError = error;
    lastTime = pros::millis()/1e3;
};