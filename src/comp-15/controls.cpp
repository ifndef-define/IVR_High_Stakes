#include "robots/comp-15/controls.h"

struct driverProfile {
    Drive::drive_mode_e driveMode;

    // Pair Value //
    // < Button, use in comp > //

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;
    pros::controller_digital_e_t shift;

    std::pair<pros::controller_digital_e_t, bool> mogoClampToggle;
    std::pair<pros::controller_digital_e_t, bool> rightMogoRushCycle;
    std::pair<pros::controller_digital_e_t, bool> leftMogoRushCycle;
    std::pair<pros::controller_digital_e_t, bool> mogoRushTeethToggle;
    std::pair<pros::controller_digital_e_t, bool> intakeLift;

    pros::controller_digital_e_t climbMode_1;
    pros::controller_digital_e_t climbMode_2;

    pros::controller_digital_e_t innerClimbArmsToggle;
    pros::controller_digital_e_t outerClimbArmsToggle;
    pros::controller_digital_e_t climbPTOToggle;

    std::pair<pros::controller_digital_e_t, bool> toggleColorSort;
    bool defaultColorSort = true;

};

const driverProfile Drive = {
    .driveMode = Drive::drive_mode_e::SPLIT_ARCADE_PL,

    .intakeIn = BUTTON_R1,
    .intakeOut = BUTTON_R2,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .shift = BUTTON_RIGHT,

    .mogoClampToggle = {BUTTON_Y,true},
    .rightMogoRushCycle = {BUTTON_B,true},
    .leftMogoRushCycle = {BUTTON_B,true},
    .mogoRushTeethToggle = {BUTTON_DOWN,true},
    .intakeLift = {BUTTON_A,false},

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    // .innerClimbArmsToggle = null, // stowed
    // .outerClimbArmsToggle = null, // stowed
    // .climbPTOToggle = null,       // disengaged

    .toggleColorSort = {BUTTON_LEFT,false}, // Ask if this is needed
    .defaultColorSort = true
};

const driverProfile Climb = {
    .driveMode = Drive::drive_mode_e::CUSTOM_m,
    
    // .intakeIn = null, // no control
    // .intakeOut = null, // no control

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    // .shift = null,   // no control

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null, // stowed
    // .leftMogoRushCycle = null, // stowed
    // .mogoRushTeethToggle = null, // disabled
    .intakeLift = {BUTTON_A,true}, // automatic

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    .innerClimbArmsToggle = BUTTON_RIGHT,
    .outerClimbArmsToggle = BUTTON_LEFT,
    .climbPTOToggle = BUTTON_Y,

    // .toggleColorSort = null // off
};

const driverProfile *controls[4] = {
    &Drive,
    &Climb,
    &Drive,
    &Climb
};

enum DriveMode {
    MODE_SOLO,
    MODE_SOLO_CLIMB,
    MODE_COMP,
    MODE_COMP_CLIMB
};
DriveMode activeProfile = MODE_SOLO;

extern pros::MotorGroup leftDrive, rightDrive, rightClimbDrive, leftClimbDrive;
void updateRobotSystems(DriveMode newMode) {
    ctrler.rumble("...");
    delay(100);
    ctrler.clear_line(3);
    delay(100);

    switch (newMode) {
        case MODE_SOLO:
        case MODE_COMP:
            if (newMode == MODE_COMP) {
                ctrler.print(0, 0, "Comp - Drive");
            } else {
                ctrler.print(0, 0, "Solo - Drive");
            }
            delay(100);
            pneumatics.innerClimbArms.retract();
            pneumatics.outerClimbArms.retract();
            pneumatics.mogoRushTeeth.extend();
            pneumatics.climbPTO.retract();
            chassis->changeDriveMode(controls[activeProfile]->driveMode);
            chassis->changeDriveMotors(leftDrive, rightDrive);
            chassis->setBrakeMode(BRAKE_COAST);
            actions.setRunColorSort(newMode == (MODE_COMP ? controls[activeProfile]->defaultColorSort : true));
            actions.setOverride(false);
            actions.setArmState(Arm::State::DOWN);
            actions.setRunArm(true);
            actions.setClimbing(false);
            break;
        case MODE_SOLO_CLIMB:
        case MODE_COMP_CLIMB:
            if (newMode == MODE_COMP_CLIMB) {
                ctrler.print(0, 0, "Comp - Climb");
            } else {
                ctrler.print(0, 0, "Solo - Climb");
            }
            delay(100);
            pneumatics.mogoClamp.extend();
            pneumatics.leftMogoRushArm.retract();
            pneumatics.rightMogoRushArm.retract();
            pneumatics.mogoRushTeeth.retract();
            pneumatics.innerClimbArms.extend();
            pneumatics.intakeLift.extend();
            actions.setRunColorSort(false);
            actions.setOverride(true);
            actions.setArmState(Arm::State::DOWN);
            actions.setRunArm(false);
            actions.setClimbing(true);
            break;
    }

    delay(500); // bounce time
}

void teleOp(Ring::Color ringToKeep) {

    chassis->loop(true);

    // if (!pros::competition::is_connected()) {
    //     activeProfile = MODE_SOLO;
    //     updateRobotSystems(activeProfile);
    // } else {
        activeProfile = MODE_COMP;
        updateRobotSystems(activeProfile);
    // }

    actions.setRingColor(ringToKeep);
    actions.setAutonControlFlag(false);
    actions.setRunAutoMogoClamp(false);
    actions.setArmState(Arm::State::DOWN);
    actions.setRunArm(true);    
    actions.setRunColorSort(controls[activeProfile]->defaultColorSort);
    
    while(1) {
        actions.runSubsystemFSM();

        switch (activeProfile) {
            case MODE_SOLO:
            case MODE_COMP:
                
                /// INTAKE ///
                actions.setOverride(ctrler.get_digital(controls[activeProfile]->shift));
                if(actions.getActionState() == ActionState::IDLE || actions.getOverride()){
                    if(ctrler.get_digital(controls[activeProfile]->intakeIn)) {
                        actions.setIntakeSpeed(1);
                    } else if(ctrler.get_digital(controls[activeProfile]->intakeOut)) {
                        actions.setIntakeSpeed(-1);
                    } else {
                        actions.setIntakeSpeed(0);
                    }

                    /// ARM ///
                    if(actions.getOverride()){
                        if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageUp)) {
                            actions.setArmSpeed(1);
                        } else if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageDown)) {
                            actions.setArmSpeed(-1);
                        } else {
                            actions.setArmSpeed(0);
                        }
                    } else {
                        if(ctrler.get_digital_new_press(controls[activeProfile]->backpackCycleStageUp)) {
                            if(actions.getArmState()==Arm::State::READY) {
                                actions.setArmState(Arm::State::DOWN);
                            } else {
                                actions.setArmState(Arm::State::READY);
                            }
                        } else if(ctrler.get_digital_new_press(controls[activeProfile]->backpackCycleStageDown)) {
                            if(actions.getArmState()==Arm::State::JESUS){
                                actions.setArmState(Arm::State::SCORE);
                            } else {
                                actions.setArmState(Arm::State::JESUS);
                            }
                        }
                    }
                }

                /// PNEUMATICS ///
                if(ctrler.get_digital_new_press(controls[activeProfile]->mogoClampToggle.first) 
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->mogoClampToggle.second : true)) {
                    pneumatics.mogoClamp.toggle();
                    if(pneumatics.mogoClamp.is_extended()) {
                        ctrler.rumble("..");
                    } else {
                        ctrler.rumble("--");
                    }
                }
                
                if(ctrler.get_digital_new_press(controls[activeProfile]->rightMogoRushCycle.first) 
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->rightMogoRushCycle.second : true)) {
                    pneumatics.rightMogoRushArm.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile]->leftMogoRushCycle.first) 
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->leftMogoRushCycle.second : true)) {
                    pneumatics.leftMogoRushArm.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile]->mogoRushTeethToggle.first)
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->mogoRushTeethToggle.second : true)) {
                    pneumatics.mogoRushTeeth.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile]->intakeLift.first)
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->intakeLift.second : true)) {
                    pneumatics.intakeLift.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile]->toggleColorSort.first)
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->toggleColorSort.second : true)) {
                    actions.setRunColorSort(!actions.getRunColorSort());
                }

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile]->climbMode_1) && ctrler.get_digital(controls[activeProfile]->climbMode_2)) {
                    activeProfile = MODE_SOLO_CLIMB;
                    updateRobotSystems(activeProfile);
                }
                break;
            case MODE_SOLO_CLIMB:
            case MODE_COMP_CLIMB:
                // No Arm Control in Climb//

                /// PNEUMATICS ///
                if (ctrler.get_digital_new_press(controls[activeProfile]->intakeLift.first)) {
                    pneumatics.intakeLift.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile]->innerClimbArmsToggle)) {
                    pneumatics.innerClimbArms.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile]->outerClimbArmsToggle)) {
                    pneumatics.outerClimbArms.toggle();
                }

                if (ctrler.get_digital_new_press(controls[activeProfile]->climbPTOToggle)) {
                    pneumatics.climbPTO.toggle();
                    if (pneumatics.climbPTO.is_extended()) {
                        chassis->changeDriveMotors(leftClimbDrive, rightClimbDrive);
                        chassis->changeDriveMode(controls[activeProfile]->driveMode);
                        chassis->setBrakeMode(BRAKE_BRAKE);
                    } else if (!(pneumatics.climbPTO.is_extended())) {
                        chassis->changeDriveMode(controls[MODE_SOLO]->driveMode);
                        chassis->changeDriveMotors(leftDrive, rightDrive);
                        chassis->setBrakeMode(BRAKE_COAST);
                    }
                }          

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile]->climbMode_1) && ctrler.get_digital(controls[activeProfile]->climbMode_2)) {
                    if (pros::competition::is_connected()) {
                        activeProfile = MODE_COMP;
                    } else {
                        activeProfile = MODE_SOLO;
                    }
                    updateRobotSystems(activeProfile);
                }
                break;
        }

        delay(10);
    }
}