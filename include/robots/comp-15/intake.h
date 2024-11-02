#pragma once
#include "robots/comp-15/devices.h"

class Intake{
    private:
    pros::MotorGroup *intakeMotor;

    public:
    Intake(pros::MotorGroup &intake);
    void setVoltage(int volt);
    void setRpm(int rpm);
    void setRelative(double position, int rpm);
};