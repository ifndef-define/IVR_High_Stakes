#include "robots/comp-24/controls.h"

/*
List of everything that can be controlled:
- Drive
- Intake
- Arm

- Mogo Clamp
- Mogo Rush R
- Mogo Rush L
- Mogo Rush Teeth
- Intake Lock
- Intake Lift

- Left Wing
- Right Wing
- Inner Climb Arms
- Outer Climb Arms
- Climb PTO

Controller Screens
Drive Mode - Menu
MogoClamp: Up/Down
################ (16) Example
Comp - Drive
MogoClamp: Down
*/

struct driverProfile {
    drive::drive_mode_e driveMode;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;
    bool adiSpecial;

    pros::controller_digital_e_t mogoClampToggle;
    pros::controller_digital_e_t rightMogoRushCycle;
    pros::controller_digital_e_t leftMogoRushCycle;
    pros::controller_digital_e_t mogoRushTeethToggle;
    pros::controller_digital_e_t intakeLock;
    pros::controller_digital_e_t intakeLiftToggle;

    pros::controller_digital_e_t climbMode_1;
    pros::controller_digital_e_t climbMode_2;

    pros::controller_digital_e_t leftWingToggle;
    pros::controller_digital_e_t rightWingToggle;
    pros::controller_digital_e_t innerClimbArmsToggle;
    pros::controller_digital_e_t outerClimbArmsToggle;
    pros::controller_digital_e_t climbPTOToggle;

    pros::controller_digital_e_t toggleColorSort;
};

const driverProfile JesusPrimary = {
    .driveMode = drive::drive_mode_e::SPLIT_ARCADE_PL,
    
    .intakeIn = BUTTON_R1,
    .intakeOut = BUTTON_R2,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .adiSpecial = false,

    .mogoClampToggle = BUTTON_Y,
    .rightMogoRushCycle = BUTTON_A,
    .leftMogoRushCycle = BUTTON_A,
    .mogoRushTeethToggle = BUTTON_UP,
    // .intakeLock = null,  // retracted
    // .intakeLiftToggle = null, // retracted

    .climbMode_1 = BUTTON_B,
    .climbMode_2 = BUTTON_DOWN,

    // .leftWingToggle = null, // folded
    // .rightWingToggle = null, // folded
    // .innerClimbArmsToggle = null, // stowed
    // .outerClimbArmsToggle = null, // stowed
    // .climbPTOToggle = null, // disengaged

    .toggleColorSort = BUTTON_X
};


const driverProfile CompClimbMode = {
    .driveMode = drive::drive_mode_e::CUSTOM_m, // add custom drive mode no turn
    
    // .intakeIn = null, // no control
    // .intakeOut = null, // no control

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .adiSpecial = false,

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null, // stowed
    // .leftMogoRushCycle = null, // stowed
    // .mogoRushTeethToggle = null, // disabled
    // .intakeLock = null, // automatic
    // .intakeLiftToggle = null, // automatic

    .climbMode_1 = BUTTON_B,
    .climbMode_2 = BUTTON_DOWN,

    // .leftWingToggle = null, // automatic
    // .rightWingToggle = null, // automatic
    .innerClimbArmsToggle = BUTTON_RIGHT,
    .outerClimbArmsToggle = BUTTON_LEFT,
    .climbPTOToggle = BUTTON_B,

    // .toggleColorSort = null // off
};

const driverProfile SoloDriveMode = {
    .driveMode = drive::drive_mode_e::SPLIT_ARCADE_PL,

    .intakeIn = BUTTON_R1,
    .intakeOut = BUTTON_R2,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .adiSpecial = false,

    .mogoClampToggle = BUTTON_DOWN,
    .rightMogoRushCycle = BUTTON_RIGHT,
    .leftMogoRushCycle = BUTTON_LEFT,
    .mogoRushTeethToggle = BUTTON_Y,
    .intakeLock = BUTTON_A,
    .intakeLiftToggle = BUTTON_B,

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    // .leftWingToggle = null, // folded
    // .rightWingToggle = null, // folded
    // .innerClimbArmsToggle = null, // stowed
    // .outerClimbArmsToggle = null, // stowed
    // .climbPTOToggle = null, // disengaged

    // .toggleColorSort = null // on by default
};

const driverProfile SoloClimbMode = {
    .driveMode = drive::drive_mode_e::CUSTOM_m, // add custom drive mode no turn
    
    // .intakeIn = null, // no control
    // .intakeOut = null, // no control

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .adiSpecial = false,

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null, // stowed
    // .leftMogoRushCycle = null, // stowed
    // .mogoRushTeethToggle = null, // disabled
    .intakeLock = BUTTON_A, // automatic
    .intakeLiftToggle = BUTTON_B, // automatic

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    .leftWingToggle = BUTTON_R1, // automatic
    .rightWingToggle = BUTTON_R2, // automatic
    .innerClimbArmsToggle = BUTTON_RIGHT,
    .outerClimbArmsToggle = BUTTON_LEFT,
    .climbPTOToggle = BUTTON_Y,

    // .toggleColorSort = null // off
};

void controllerMsg(string msg) {
    ctrler.rumble("...");
    delay(100);
    ctrler.clear_line(3);
    delay(100);
    ctrler.print(0, 0, msg.c_str());
    delay(650);
}

/*
Before Climb
- Get ring in arm
- Line up with ladder (no pressure on intake to allow intake to lock)

Switch climb menu
( this toggles wing and engages pto, locks intake? )
Wait for rumble to climb

inner arms out
driver starts climbing
*/

const driverProfile controls[4] = {
    SoloDriveMode,
    SoloClimbMode,
    JesusPrimary,
    CompClimbMode
};

enum DriveMode {
    MODE_SOLO,
    MODE_SOLO_CLIMB,
    MODE_COMP,
    MODE_COMP_CLIMB
};

void teleOp(Ring::Color ringToKeep) {
    chassis->loop(true);
    DriveMode activeProfile = MODE_SOLO;

    if (!pros::competition::is_connected()) {
        activeProfile = MODE_SOLO;
        controllerMsg("Solo - Drive");
    } else {
        activeProfile = MODE_COMP;
        controllerMsg("Comp - Drive");
    }

    while(1) {

        switch (activeProfile) {
            case MODE_SOLO:
                //////////////////
                /*    INTAKE    */
                //////////////////
                if(ctrler.get_digital(controls[activeProfile].intakeIn)) {
                    actions.setIntakeSpeed(1);
                } else if(ctrler.get_digital(controls[activeProfile].intakeOut)) {
                    actions.setIntakeSpeed(-1);
                } else {
                    actions.setIntakeSpeed(0);
                }

                ///////////////
                /*    ARM    */
                ///////////////
                if(controls[activeProfile].backpackCycleStageUp && ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)){
                    actions.setArmState(Arm::State::DESCORE);
                } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                    actions.setArmState(Arm::State::READY);
                } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                    actions.setArmState(Arm::State::SCORE);
                } else {
                    actions.setArmState(Arm::State::DOWN);
                }

                //////////////////
                /*  Pneumatics  */
                //////////////////
                if(ctrler.get_digital_new_press(controls[activeProfile].mogoClampToggle)) {
                    pneumatics.mogoClamp.toggle();
                    if(pneumatics.mogoClamp.is_extended()) {
                        ctrler.rumble("..");
                    } else {
                        ctrler.rumble("--");
                    }
                }
                
                if(ctrler.get_digital_new_press(controls[activeProfile].rightMogoRushCycle)) {
                    pneumatics.rightMogoRushArm.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile].leftMogoRushCycle)) {
                    pneumatics.leftMogoRushArm.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile].mogoRushTeethToggle)) {
                    pneumatics.mogoRushTeeth.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile].intakeLiftToggle)) {
                    pneumatics.intakeLift.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile].intakeLock)) {
                    pneumatics.intakeLock.toggle();
                }

                actions.runSubsystemFSM();

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_SOLO_CLIMB;
                    controllerMsg("Solo - Climb");
                }
                break;
            case MODE_SOLO_CLIMB:

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_SOLO;
                    controllerMsg("Solo - Drive");
                }
                break;
            case MODE_COMP:

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_COMP_CLIMB;
                    controllerMsg("Comp - Climb");
                }
                break;
            case MODE_COMP_CLIMB:
            
                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_COMP;
                    controllerMsg("Comp - Drive");
                }
                break;
        }

        delay(10);
    }
}


// void teleOp(Ring::Color ringToKeep) {
//     int pow, turn, rushState = 0;
//     actions.setRingColor(ringToKeep);
//     actions.setArmState(Arm::State::DOWN);
//     actions.setRunColorSort(true);

//     while(1) {
//         // Drive Control
//         pow = ctrler.get_analog(currentProfile.powerAxis);
//         turn = ctrler.get_analog(currentProfile.turnAxis)/1.3;
//         // chassis.arcade(pow, turn, 0);

//         leftDrive.move(pow + turn);
//         rightDrive.move(pow - turn);

//         // Intake/Arm Control
//         actions.setOverride(ctrler.get_digital(currentProfile.shift));
//         if(actions.getActionState() != ActionState::SORTING || actions.getOverride()){
//             //////////////////
//             /*    INTAKE    */
//             //////////////////
//             if(ctrler.get_digital(currentProfile.intakeIn)) {
//                 actions.setIntakeSpeed(1);
//             } else if(ctrler.get_digital(currentProfile.intakeOut)) {
//                 actions.setIntakeSpeed(-1);
//             } else {
//                 actions.setIntakeSpeed(0);
//             }
//             ///////////////
//             /*    ARM    */
//             ///////////////
//             if(actions.getOverride()){
//                 if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
//                     actions.setArmSpeed(1);
//                 } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
//                     actions.setArmSpeed(-1);
//                 } else {
//                     actions.setArmSpeed(0);
//                 }
//             } else if(currentProfile.incrementBackpack) {
//                     if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageUp)) {
//                         actions.nextArmState();
//                     } else if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageDown)) {
//                         actions.prevArmState();
//                     }
//             } else {
//                 if(ctrler.get_digital(currentProfile.backpackCycleStageUp) && ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
//                     actions.setArmState(Arm::State::DESCORE);
//                 } else if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
//                     actions.setArmState(Arm::State::SCORE);
//                 } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
//                     actions.setArmState(Arm::State::READY);
//                 } else {
//                     actions.setArmState(Arm::State::DOWN);
//                 }
//             }
//         }
//         //////////////////////
//         /*    Pneumatics    */
//         //////////////////////
//         if(ctrler.get_digital_new_press(currentProfile.mogoClampToggle)) {
//             pneumatics.mogoClamp.toggle();
//             if(pneumatics.mogoClamp.is_extended()) {
//                 ctrler.rumble("..");
//             } else {
//                 ctrler.rumble("--");
//             }
//         }

//         if(ctrler.get_digital_new_press(currentProfile.mogoRushClamp)) {
//             pneumatics.mogoRushClamp.toggle();
//         }

//         if(ctrler.get_digital_new_press(currentProfile.mogoRushCycle)) {
//             if(rushState == 0) {
//                 pneumatics.mogoRushLeftArm.extend();
//                 pneumatics.mogoRushRightArm.retract();
//             } else if (rushState == 1) {
//                 pneumatics.mogoRushLeftArm.retract();
//                 pneumatics.mogoRushRightArm.extend();
//             } else {
//                 pneumatics.mogoRushLeftArm.retract();
//                 pneumatics.mogoRushRightArm.retract();
//             }

//             if(ctrler.get_digital(currentProfile.toggleColorSort)){
//                 actions.setRunColorSort(!actions.getRunColorSort());
//                 if(actions.getRunColorSort()){
//                     ctrler.rumble("..");
//                 } else {
//                     ctrler.rumble("--");
//                 }
//             }

//             rushState = rushState == 2 ? 0 : rushState + 1;
//         }

//         if(ctrler.get_digital_new_press(currentProfile.toggleClimb)){
//             if(!actions.getRunClimb()){
//                 actions.setRunClimb(!actions.getRunClimb());
//             } else {
//                 actions.setRunClimb(false);
//                 //MANUAL CONTROL HERE
//                 if(ctrler.get_digital_new_press(currentProfile.intakeIn)){
//                     actions.isPtoExtended() ? actions.extendPto() : actions.retractPto();
//                 } else if(ctrler.get_digital_new_press(currentProfile.intakeOut)){
//                     actions.isPusherExtended() ? actions.extendPusher() : actions.retractPusher();
//                 }
//             }
//         }
//         //Print out data for 
//         pros::lcd::print(7, "Arm Angle: %f", actions.getArmAngle());
//         pros::lcd::print(8, "Pullback: %d", actions.getPullbackFlag());
//         pros::lcd::print(9, "State: %d", int(actions.getActionState()));
//         pros::delay(10);
//     }
// }