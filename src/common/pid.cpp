#include "common/pid.h"
#include "pros/rtos.hpp"
#include <cmath>

void PID::setCoefficients(double p, double i, double d) {
    this->p = p;
    this->i = i;
    this->d = d;
}

double PID::update(double error) {
    double value = 0;
    if (lastTime >= 0) {
        double dt = pros::micros()/1e6 - lastTime;
        double de_dt = (error - lastError) / dt;
        accumulator += error * dt;
        if (std::abs(accumulator) == infinity()) {
            accumulator = 0;
        }

        value = p * error + i * accumulator + d * de_dt;
    }
    lastError = error;
    lastTime = pros::micros()/1e6;
    return value;
};