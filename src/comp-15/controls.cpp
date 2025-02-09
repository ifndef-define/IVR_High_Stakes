#include "robots/comp-15/controls.h"

struct driverProfile {
    pros::controller_analog_e_t powerAxis;
    pros::controller_analog_e_t turnAxis;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;
    pros::controller_digital_e_t shift;

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
    .shift = pros::E_CONTROLLER_DIGITAL_DOWN,

    .backpackCycleStageUp = pros::E_CONTROLLER_DIGITAL_L1,
    .backpackCycleStageDown = pros::E_CONTROLLER_DIGITAL_L2,
    .incrementBackpack = true,

    .mogoClampToggle = pros::E_CONTROLLER_DIGITAL_Y,
    .doinkerToggle = pros::E_CONTROLLER_DIGITAL_X
};

const driverProfile &currentProfile = JesusPrimary;
Action actions(0, Ring::Color::RED, ctrler);

void teleOp(Ring::Color ringToKeep) {
    int pow, turn;

    while(1) {
        // Drive Control
        pow = ctrler.get_analog(currentProfile.powerAxis);
        turn = ctrler.get_analog(currentProfile.turnAxis)/1.5;

        leftDrive.move(pow + turn);
        rightDrive.move(pow - turn);
        
        // Sorting Control
        actions.runSubsystemFSM();

        // Intake/Arm Control
        actions.setOverride(ctrler.get_digital(currentProfile.shift));
        if(actions.getState() != ActionState::SORTING || actions.isOverride()){
            if(ctrler.get_digital(currentProfile.intakeIn)) {
                actions.setIntakeSpeed(1);
            } else if(ctrler.get_digital(currentProfile.intakeOut)) {
                actions.setIntakeSpeed(-1);
            } else {
                actions.setIntakeSpeed(0);
            }

            if(currentProfile.incrementBackpack) {
                if(ctrler.get_digital(currentProfile.shift)){
                    if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                        actions.setArmSpeed(1);
                    } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                        actions.setArmSpeed(-1);
                    } else {
                        actions.setArmSpeed(0);
                    }
                }
                if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                    actions.nextArmState();
                } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                    actions.prevArmState();
                }
            } else {
                if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                    actions.setArmState(Arm::State::SCORE);
                } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                    actions.setArmState(Arm::State::READY);
                } else {
                    actions.setArmState(Arm::State::DOWN);
                }
            }

        }

        pros::delay(10);
    }
}