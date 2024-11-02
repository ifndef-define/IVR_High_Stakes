#include "comp-15/intake.h"

Intake::Intake(pros::MotorGroup &intake){
    intakeMotor = &intake;
}

void Intake::setVoltage(int volt){
    intakeMotor->move_voltage(volt * 1000);
}

void Intake::setRpm(int rpm){
    intakeMotor->move_velocity(rpm);
}

void Intake::setRelative(double position, int rpm){
    intakeMotor->move_relative(position, rpm);
}