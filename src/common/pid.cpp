#include "common/pid.h"
#include "pros/rtos.hpp"
#include "main.h"

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

        value = p * error + d * de_dt;
        pros::lcd::print(1, "p %f, i %f, d %f, a %f, %f de_dt", p, i, d, accumulator, de_dt);
        pros::lcd::print(2, "a %f, %f de_dt, %f dt", accumulator, de_dt, dt);
    }
    lastError = error;
    lastTime = pros::micros()/1e6;
    return value;
};