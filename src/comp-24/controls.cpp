#include "robots/comp-24/controls.h"

struct driverProfile {
    pros::controller_analog_e_t powerAxis;
    pros::controller_analog_e_t turnAxis;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;
    pros::controller_digital_e_t shift;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;
    bool incrementBackpack;

    pros::controller_digital_e_t mogoClampToggle;
    pros::controller_digital_e_t doinkerToggle;
    pros::controller_digital_e_t mogoRushCycle;
};

driverProfile JesusPrimary = {
    .powerAxis = pros::E_CONTROLLER_ANALOG_LEFT_Y,
    .turnAxis = pros::E_CONTROLLER_ANALOG_RIGHT_X,

    .intakeIn = pros::E_CONTROLLER_DIGITAL_R1,
    .intakeOut = pros::E_CONTROLLER_DIGITAL_R2,
    .shift = pros::E_CONTROLLER_DIGITAL_RIGHT,

    .backpackCycleStageUp = pros::E_CONTROLLER_DIGITAL_L1,
    .backpackCycleStageDown = pros::E_CONTROLLER_DIGITAL_L2,
    .incrementBackpack = false,

    .mogoClampToggle = pros::E_CONTROLLER_DIGITAL_Y,
    .doinkerToggle = pros::E_CONTROLLER_DIGITAL_B,
    .mogoRushCycle = pros::E_CONTROLLER_DIGITAL_A
};

const driverProfile &currentProfile = JesusPrimary;

void teleOp(Ring::Color ringToKeep) {
    int pow, turn;
    actions.setRingColor(ringToKeep);
    actions.setArmState(Arm::State::DOWN);

    while(1) {
        // Drive Control
        pow = ctrler.get_analog(currentProfile.powerAxis);
        turn = ctrler.get_analog(currentProfile.turnAxis);
        chassis.arcade(pow, turn, 0);

        // leftDrive.move(pow + turn);
        // rightDrive.move(pow - turn);

        // Intake/Arm Control
        actions.setOverride(ctrler.get_digital(currentProfile.shift));
        if(actions.getState() != ActionState::SORTING || actions.getOverride()){
            //////////////////
            /*    INTAKE    */
            //////////////////
            if(ctrler.get_digital(currentProfile.intakeIn)) {
                actions.setIntakeSpeed(1);
            } else if(ctrler.get_digital(currentProfile.intakeOut)) {
                actions.setIntakeSpeed(-1);
            } else {
                actions.setIntakeSpeed(0);
            }
            ///////////////
            /*    ARM    */
            ///////////////
            if(actions.getOverride()){
                // if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                //     actions.setArmSpeed(1);
                // } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                //     actions.setArmSpeed(-1);
                // } else {
                //     actions.setArmSpeed(0);
                // }
            } else if(currentProfile.incrementBackpack) {
                    if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageUp)) {
                        actions.nextArmState();
                    } else if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageDown)) {
                        actions.prevArmState();
                    }
            } else {
                if(ctrler.get_digital(currentProfile.backpackCycleStageUp) && ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                    actions.setArmState(Arm::State::DESCORE);
                } else if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                    actions.setArmState(Arm::State::SCORE);
                } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                    actions.setArmState(Arm::State::READY);
                } else {
                    actions.setArmState(Arm::State::DOWN);
                }
            }
        }
        //////////////////////
        /*    Pneumatics    */
        //////////////////////
        if(ctrler.get_digital_new_press(currentProfile.mogoClampToggle)) {
            pneumatics.mogoClamp.toggle();
        }

        if(ctrler.get_digital_new_press(currentProfile.doinkerToggle)) {
            pneumatics.doinker.toggle();
        }

        if(ctrler.get_digital_new_press(currentProfile.mogoRushCycle)) {
            if(!pneumatics.mogoRushArm.is_extended() && !pneumatics.mogoRushClamp.is_extended()){
                pneumatics.mogoRushArm.extend();
                pneumatics.mogoRushClamp.extend();
            } else if(pneumatics.mogoRushArm.is_extended() && pneumatics.mogoRushClamp.is_extended()){
                pneumatics.mogoRushClamp.retract();
            } else if(pneumatics.mogoRushArm.is_extended() && !pneumatics.mogoRushClamp.is_extended()){
                pneumatics.mogoRushArm.retract();
            }
        }
        //Print out data for 
        pros::lcd::print(1, "%f", actions.getArmAngle());
        pros::lcd::print(2, "%d", actions.getPullbackFlag());
        pros::lcd::print(3, "%d", int(actions.getState()));
        pros::delay(10);
    }
}