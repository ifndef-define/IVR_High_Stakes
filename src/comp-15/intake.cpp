#include "comp-15/intake.h"

Intake::Intake(pros::MotorGroup &intake)
{
    intakeMotor = &intake;
    isEjecting = false;
    pauseCounter1 = 0;
    pauseCounter2 = 0;
}

void Intake::setVoltage(int volt)
{
    intakeMotor->move_voltage(volt * 1000);
}

void Intake::setRpm(int rpm)
{
    intakeMotor->move_velocity(rpm);
}

void Intake::setRelative(double position, int rpm)
{
    intakeMotor->move_relative(position, rpm);
}

void Intake::pullBack()
{
    if (arm.getIntakePullBackFlag())
    {
        intake.move(-127);
        if (pauseCounter1 < 7)
        { // 7*15 = 105ms
            pauseCounter1++;
        }
        else
        {
            pauseCounter1 = 0;
            intake.brake();
            arm.setIntakePullBackFlag(false);
        }
    }
}

void Intake::manualControl()
{

    if (!isEjecting)
    {
        if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            // sift(1, intakeToggle);
            if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
            {
                intake.move(127 / 2);
            }
            else
            {
                intake.move(127);
            }
        }
        else if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            // intakeToggle = false;
            if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
            {
                intake.move(-127 / 3);
            }
            else
            {
                intake.move(-127);
            }
        }
        else
        {
            intake.brake();
        }
    }
    else
    {

        if (arm.getState() <= 1)
        {
            arm.setState(0);
        }
        intake.move(-127);
        if (pauseCounter2 < 10)
        { // 10*15 = 150ms
            pauseCounter2++;
        }
        else
        {
            pauseCounter2 = 0;
            intake.brake();
            isEjecting = false;
        }
    }

}

bool Intake::getIsEjecting(){
    return isEjecting;
}