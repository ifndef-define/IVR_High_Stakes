#include "robots/comp-15/controls.h"

struct driverProfile {
    pros::controller_analog_e_t powerAxis;
    pros::controller_analog_e_t turnAxis;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;
    pros::controller_digital_e_t intakeSlow;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;

    pros::controller_digital_e_t mogoClampToggle;
    pros::controller_digital_e_t doinkerToggle;
};

driverProfile JesusPrimary = {
    .powerAxis = pros::E_CONTROLLER_ANALOG_LEFT_Y,
    .turnAxis = pros::E_CONTROLLER_ANALOG_RIGHT_X,

    .intakeIn = pros::E_CONTROLLER_DIGITAL_R1,
    .intakeOut = pros::E_CONTROLLER_DIGITAL_R2,
    .intakeSlow = pros::E_CONTROLLER_DIGITAL_L2,

    .backpackCycleStageUp = pros::E_CONTROLLER_DIGITAL_UP,
    .backpackCycleStageDown = pros::E_CONTROLLER_DIGITAL_DOWN,

    .mogoClampToggle = pros::E_CONTROLLER_DIGITAL_X,
    .doinkerToggle = pros::E_CONTROLLER_DIGITAL_Y
};

const driverProfile &currentProfile = JesusPrimary;

void driverctrl(void) {
    int pow, turn;

    while(1) {
        // Drive Control
        pow = ctrler.get_analog(currentProfile.powerAxis);
        turn = ctrler.get_analog(currentProfile.turnAxis)/1.5;

        leftDrive.move(pow + turn);
        rightDrive.move(pow - turn);
        
        // Intake Control

        // Backpack Control

        pros::delay(10);
    }
}