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
    pros::controller_digital_e_t mogoRushCycle;
    pros::controller_digital_e_t mogoRushClamp;

    pros::controller_digital_e_t toggleColorSort;
    pros::controller_digital_e_t toggleClimb;
};

driverProfile AdiPrimary = {
    .powerAxis = pros::E_CONTROLLER_ANALOG_LEFT_Y,
    .turnAxis = pros::E_CONTROLLER_ANALOG_RIGHT_X,

    .intakeIn = pros::E_CONTROLLER_DIGITAL_R1,
    .intakeOut = pros::E_CONTROLLER_DIGITAL_R2,
    .shift = pros::E_CONTROLLER_DIGITAL_RIGHT,

    .backpackCycleStageUp = pros::E_CONTROLLER_DIGITAL_L1,
    .backpackCycleStageDown = pros::E_CONTROLLER_DIGITAL_L2,
    .incrementBackpack = false,

    .mogoClampToggle = pros::E_CONTROLLER_DIGITAL_Y,
    .mogoRushCycle = pros::E_CONTROLLER_DIGITAL_A, // Only w/o field connection
    .mogoRushClamp = pros::E_CONTROLLER_DIGITAL_UP,

    .toggleColorSort = pros::E_CONTROLLER_DIGITAL_X, 
    .toggleClimb = pros::E_CONTROLLER_DIGITAL_B
};

const driverProfile &currentProfile = AdiPrimary;

void teleOp(Ring::Color ringToKeep) {
    int pow, turn, rushState = 0;
    actions.setRingColor(ringToKeep);
    actions.setArmState(Arm::State::DOWN);
    actions.setRunColorSort(true);

    while(1) {
        // Drive Control
        pow = ctrler.get_analog(currentProfile.powerAxis);
        turn = ctrler.get_analog(currentProfile.turnAxis)/1.3;
        // chassis.arcade(pow, turn, 0);

        leftDrive.move(pow + turn);
        rightDrive.move(pow - turn);

        // Intake/Arm Control
        actions.setOverride(ctrler.get_digital(currentProfile.shift));
        if(actions.getActionState() != ActionState::SORTING || actions.getOverride()){
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
                if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                    actions.setArmSpeed(1);
                } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                    actions.setArmSpeed(-1);
                } else {
                    actions.setArmSpeed(0);
                }
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
            if(pneumatics.mogoClamp.is_extended()) {
                ctrler.rumble("..");
            } else {
                ctrler.rumble("--");
            }
        }

        if(ctrler.get_digital_new_press(currentProfile.mogoRushClamp)) {
            pneumatics.mogoRushClamp.toggle();
        }

        if(ctrler.get_digital_new_press(currentProfile.mogoRushCycle)) {
            if(rushState == 0) {
                pneumatics.mogoRushLeftArm.extend();
                pneumatics.mogoRushRightArm.retract();
            } else if (rushState == 1) {
                pneumatics.mogoRushLeftArm.retract();
                pneumatics.mogoRushRightArm.extend();
            } else {
                pneumatics.mogoRushLeftArm.retract();
                pneumatics.mogoRushRightArm.retract();
            }

            if(ctrler.get_digital(currentProfile.toggleColorSort)){
                actions.setRunColorSort(!actions.getRunColorSort());
                if(actions.getRunColorSort()){
                    ctrler.rumble("..");
                } else {
                    ctrler.rumble("--");
                }
            }

            rushState = rushState == 2 ? 0 : rushState + 1;
        }

        if(ctrler.get_digital_new_press(currentProfile.toggleClimb)){
            if(!actions.getRunClimb()){
                actions.setRunClimb(!actions.getRunClimb());
            } else {
                actions.setRunClimb(false);
                //MANUAL CONTROL HERE
                if(ctrler.get_digital_new_press(currentProfile.intakeIn)){
                    actions.isPtoExtended() ? actions.extendPto() : actions.retractPto();
                } else if(ctrler.get_digital_new_press(currentProfile.intakeOut)){
                    actions.isPusherExtended() ? actions.extendPusher() : actions.retractPusher();
                }
            }
        }
        //Print out data for 
        pros::lcd::print(7, "Arm Angle: %f", actions.getArmAngle());
        pros::lcd::print(8, "Pullback: %d", actions.getPullbackFlag());
        pros::lcd::print(9, "State: %d", int(actions.getActionState()));
        pros::delay(10);
    }
}