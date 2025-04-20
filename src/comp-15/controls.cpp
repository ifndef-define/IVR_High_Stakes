#include "robots/comp-15/controls.h"

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

// driverProfile old = {
//     .powerAxis = pros::E_CONTROLLER_ANALOG_LEFT_Y,
//     .turnAxis = pros::E_CONTROLLER_ANALOG_RIGHT_X,

//     .intakeIn = pros::E_CONTROLLER_DIGITAL_R1,
//     .intakeOut = pros::E_CONTROLLER_DIGITAL_R2,

//     .backpackCycleStageUp = pros::E_CONTROLLER_DIGITAL_L1,
//     .backpackCycleStageDown = pros::E_CONTROLLER_DIGITAL_L2,
//     .adiSpecial = true,

//     .mogoClampToggle = pros::E_CONTROLLER_DIGITAL_Y,
//     .mogoRushCycle = pros::E_CONTROLLER_DIGITAL_A, // Only w/o field connection
//     .mogoRushClamp = pros::E_CONTROLLER_DIGITAL_UP,

//     .toggleColorSort = pros::E_CONTROLLER_DIGITAL_X
// };

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
    // .intakeLock = null,
    // .intakeLiftToggle = null,

    .climbMode_1 = BUTTON_B,
    .climbMode_2 = BUTTON_DOWN,

    // .leftWingToggle = null,
    // .rightWingToggle = null,
    // .innerClimbArmsToggle = null,
    // .outerClimbArmsToggle = null,
    // .climbPTOToggle = null,

    .toggleColorSort = BUTTON_X
};


const driverProfile ClimbMode = {
    // .driveMode = drive::drive_mode_e::SPLIT_ARCADE_PL, // add custom drive mode no turn
    
    // .intakeIn = null,
    // .intakeOut = null,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .adiSpecial = false,

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null,
    // .leftMogoRushCycle = null,
    // .mogoRushTeethToggle = null,
    // .intakeLock = null, // automatic
    // .intakeLiftToggle = null, // automatic

    .climbMode_1 = BUTTON_B,
    .climbMode_2 = BUTTON_DOWN,

    // .leftWingToggle = null, // automatic
    // .rightWingToggle = null, // automatic
    .innerClimbArmsToggle = BUTTON_RIGHT,
    .outerClimbArmsToggle = BUTTON_LEFT,
    .climbPTOToggle = BUTTON_B,

    // .toggleColorSort = null
};

/*
Before Climb
- Get ring in arm
- Line up with ladder

Switch climb menu
( this toggles wing and engages pto, locks intake? )
Wait for rumble to climb

inner arms out
driver starts climbing
*/

const driverProfile &currentProfile = JesusPrimary;

void teleOp(Ring::Color ringToKeep) {
    int pow, turn, rushState = 0;
    actions.setRingColor(ringToKeep);
    actions.setAutonControlFlag(false);
    actions.setRunColorSort(true);
    actions.setArmState(Arm::State::DOWN);

    while(1) {
        // Drive Control
        pow = ctrler.get_analog(currentProfile.powerAxis);
        turn = ctrler.get_analog(currentProfile.turnAxis)/1.3;

        leftDrive.move(pow + turn);
        rightDrive.move(pow - turn);
        
        // Sorting Control
        // actions.runSubsystemFSM();

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
                if(ctrler.get_digital(currentProfile.backpackCycleStageUp)) {
                    actions.setArmSpeed(1);
                } else if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
                    actions.setArmSpeed(-1);
                } else {
                    actions.setArmSpeed(0);
                }
            } else if(currentProfile.incrementBackpack) {
                    if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageUp)) {
                        if(actions.getArmState()==Arm::State::READY) {
                            actions.setArmState(Arm::State::DOWN);
                        } else {
                            actions.setArmState(Arm::State::READY);
                        }
                    } else if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageDown)) {
                        actions.nextArmState();
                    }
            } else {
                if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
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

        //Print out data for 
        pros::lcd::print(1, "%f", actions.getArmAngle());
        pros::lcd::print(2, "%d", actions.getPullbackFlag());
        pros::lcd::print(3, "%d", int(actions.getState()));
        pros::delay(10);
    }
}