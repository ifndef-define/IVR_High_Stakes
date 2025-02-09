#include "robots/comp-15/controls.h"

struct driverProfile {
    pros::controller_analog_e_t powerAxis;
    pros::controller_analog_e_t turnAxis;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;
    pros::controller_digital_e_t intakeSlow;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;
    bool incrementBackpack = false;

    pros::controller_digital_e_t mogoClampToggle;
    pros::controller_digital_e_t doinkerToggle;
};

driverProfile JesusPrimary = {
    .powerAxis = pros::E_CONTROLLER_ANALOG_LEFT_Y,
    .turnAxis = pros::E_CONTROLLER_ANALOG_RIGHT_X,

    .intakeIn = pros::E_CONTROLLER_DIGITAL_R1,
    .intakeOut = pros::E_CONTROLLER_DIGITAL_R2,
    .intakeSlow = pros::E_CONTROLLER_DIGITAL_DOWN,

    .backpackCycleStageUp = pros::E_CONTROLLER_DIGITAL_L1,
    .backpackCycleStageDown = pros::E_CONTROLLER_DIGITAL_L2,
    .incrementBackpack = false,

    .mogoClampToggle = pros::E_CONTROLLER_DIGITAL_Y,
    .doinkerToggle = pros::E_CONTROLLER_DIGITAL_X
};

const driverProfile &currentProfile = JesusPrimary;

Action actions(Ring::Color::RED, ctrler);

void teleOp(void) {
    int pow, turn;

    while(1) {
        // Drive Control
        pow = ctrler.get_analog(currentProfile.powerAxis);
        turn = ctrler.get_analog(currentProfile.turnAxis)/1.5;

        leftDrive.move(pow + turn);
        rightDrive.move(pow - turn);
        
        // Subsystems
        actions.runSubsystemFSM();

        pros::delay(10);
    }
}