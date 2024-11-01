#pragma once
#include "robots/comp-15/devices.h"

class Intake{
    private:
    pros::MotorGroup intakeMotor();

    public:
    Intake(std::int8_t port);
    void setVoltage(int volt);
    void setRpm(int rpm);
    
};