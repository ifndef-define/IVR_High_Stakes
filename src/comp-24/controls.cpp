#include "robots/comp-24/controls.h"

struct driverProfile {
    Drive::drive_mode_e driveMode;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;

    std::pair <pros::controller_digital_e_t, bool> mogoClampToggle;
    std::pair <pros::controller_digital_e_t, bool> rightMogoRushCycle;
    std::pair <pros::controller_digital_e_t, bool> leftMogoRushCycle;
    std::pair <pros::controller_digital_e_t, bool> mogoRushTeethToggle;
    std::pair <pros::controller_digital_e_t, bool> intakeLockToggle;
    std::pair <pros::controller_digital_e_t, bool> intakeLiftToggle;

    pros::controller_digital_e_t climbMode_1;
    pros::controller_digital_e_t climbMode_2;

    pros::controller_digital_e_t innerClimbArmsToggle;
    pros::controller_digital_e_t outerClimbArmsToggle;
    pros::controller_digital_e_t leftPaperToggle;
    pros::controller_digital_e_t rightPaperToggle;
    pros::controller_digital_e_t climbPTOToggle;

    pros::controller_digital_e_t toggleColorSort;
    pros::controller_digital_e_t shift;
    bool defaultColorSort = false;
};

const driverProfile Drive = {
    .driveMode = Drive::drive_mode_e::SPLIT_ARCADE_PL,
    
    .intakeIn = BUTTON_R1,
    .intakeOut = BUTTON_R2,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,

    .mogoClampToggle = {BUTTON_Y, true},
    .rightMogoRushCycle = {BUTTON_A, true},
    .leftMogoRushCycle = {BUTTON_A, true},
    .mogoRushTeethToggle = {BUTTON_B, true},
    .intakeLockToggle = {BUTTON_LEFT,false},  // retracted
    .intakeLiftToggle = {BUTTON_DOWN,false},  // dropped

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    // .innerClimbArmsToggle = null, // stowed
    // .outerClimbArmsToggle = null, // stowed
    // .leftPaperToggle = null, // stowed
    // .rightPaperToggle = null, // stowed
    // .climbPTOToggle = null, // disengaged

    // .toggleColorSort = null // off by default
    .shift = BUTTON_RIGHT,
};


const driverProfile Climb = {
    .driveMode = Drive::drive_mode_e::CUSTOM_m,
    
    // .intakeIn = null, // no control
    // .intakeOut = null, // no control

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null, // stowed
    // .leftMogoRushCycle = null, // stowed
    // .mogoRushTeethToggle = null, // disabled
    // .intakeLiftToggle = null // dropped automatically
    .intakeLockToggle = {BUTTON_A, false}, // automatic

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    .innerClimbArmsToggle = BUTTON_RIGHT,
    .outerClimbArmsToggle = BUTTON_LEFT,
    .leftPaperToggle = BUTTON_R1,
    .rightPaperToggle = BUTTON_R2,
    .climbPTOToggle = BUTTON_DOWN,
    .shift = BUTTON_Y,
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
            pneumatics.intakeLift.extend();
            pneumatics.innerClimbArms.retract();
            pneumatics.outerClimbArms.retract();
            pneumatics.climbPTO.retract();
            chassis->changeDriveMode(controls[activeProfile]->driveMode);
            chassis->changeDriveMotors(leftDrive, rightDrive);
            chassis->setBrakeMode(BRAKE_COAST);
            actions.setRunColorSort(newMode == ((MODE_COMP || MODE_SOLO) ? controls[activeProfile]->defaultColorSort : true));
            actions.setOverride(false);
            actions.setArmState(Arm::State::DOWN);
            actions.setRunArm(true);
            actions.setClimbing(false);
            actions.setRunAutoMogoClamp(true);
            break;
        case MODE_SOLO_CLIMB:
        case MODE_COMP_CLIMB:
            if (newMode == MODE_COMP) {
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
            pneumatics.intakeLift.extend(); // gone for climb
            // pneumatics.intakeLock.retract();
            actions.setRunColorSort(false);
            actions.setRunArm(true);
            actions.setArmState(Arm::State::CLIMB);
            actions.setClimbing(true);
            actions.setRunAutoMogoClamp(true);
            break;
    }

    delay(500); // bounce time
}

void teleOp(Ring::Color ringToKeep) {
    chassis->loop(true);

    if (!pros::competition::is_connected()) {
        activeProfile = MODE_SOLO;
        updateRobotSystems(activeProfile);
    } else {
        activeProfile = MODE_COMP;
        updateRobotSystems(activeProfile);
    }

    actions.setRingColor(ringToKeep);
    actions.setAutonControlFlag(false);
    actions.setRunAutoMogoClamp(false);
    actions.setArmState(Arm::State::DOWN);
    actions.setRunArm(true);
    bool lastProfile = false;

    int targetAngle = 999;
    while(1) {
        actions.runSubsystemFSM();

        switch (activeProfile) {
            case MODE_SOLO:
                // if (ui::getRunColorSort()) {
                //     actions.setRunColorSort(true);
                // } else {
                //     actions.setRunColorSort(false);
                // }

                // if (ui::getRingColor()) {
                //     ringToKeep = Ring::Color::BLUE;
                // } else {
                //     ringToKeep = Ring::Color::RED;
                // }
                // actions.setRingColor(ringToKeep);

            case MODE_COMP:
                if (pros::competition::is_connected()) {
                    // if (ui::getCurrentAuto() == 0 || ui::getCurrentAuto() == 1) {
                    //     ringToKeep = Ring::Color::RED;
                    // } else if (ui::getCurrentAuto() == 2 || ui::getCurrentAuto() == 3) {
                    //     ringToKeep = Ring::Color::BLUE;
                    // } else {
                    //     ringToKeep = ui::getRingColor() ? Ring::Color::BLUE : Ring::Color::RED;
                    // }
                    actions.setRingColor(ringToKeep);
                    actions.setRunColorSort(controls[activeProfile]->defaultColorSort);
                } else {
                    if (lastProfile != ui::getRunForceCompMode()) {
                        lastProfile = ui::getRunForceCompMode();
                        activeProfile = lastProfile ? MODE_COMP : MODE_SOLO;
                        updateRobotSystems(activeProfile);
                    }
                }
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
                            targetAngle = 999;
                        } else if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageDown)) {
                            actions.setArmSpeed(-1);
                            targetAngle = 999;
                        } else {
                            actions.setArmSpeed(0);
                            targetAngle = actions.getArmAngle(); //updates arm angle
                        }
                    } else {
                        if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageUp) && ctrler.get_digital(controls[activeProfile]->backpackCycleStageDown)) {
                            actions.setArmState(Arm::State::DESCORE);
                            targetAngle = 999;
                        } else if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageUp)) {
                            actions.setArmState(Arm::State::READY);
                            targetAngle = 999;
                        } else if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageDown)) {
                            actions.setArmState(Arm::State::SCORE);
                            targetAngle = 999;
                        } else {
                            actions.setArmState(Arm::State::DOWN);
                            targetAngle = 999;
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
                
                if (ringToKeep == Ring::Color::RED) {
                    if(ctrler.get_digital_new_press(controls[activeProfile]->leftMogoRushCycle.first) 
                        && (activeProfile == MODE_COMP ? controls[activeProfile]->leftMogoRushCycle.second : true)) {
                        pneumatics.leftMogoRushArm.toggle();
                    }
                } else if (ringToKeep == Ring::Color::BLUE) {
                    if(ctrler.get_digital_new_press(controls[activeProfile]->rightMogoRushCycle.first) 
                        && (activeProfile == MODE_COMP ? controls[activeProfile]->rightMogoRushCycle.second : true)) {
                        pneumatics.rightMogoRushArm.toggle();
                    }
                }
                if(ctrler.get_digital_new_press(controls[activeProfile]->mogoRushTeethToggle.first) 
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->mogoRushTeethToggle.second : true)) {
                    pneumatics.mogoRushTeeth.toggle();
                }
                // if(ctrler.get_digital_new_press(controls[activeProfile]->intakeLockToggle.first) 
                //     && (activeProfile == MODE_COMP ? controls[activeProfile]->intakeLockToggle.second : true)) {
                //     pneumatics.intakeLock.toggle();
                // }
                if(ctrler.get_digital_new_press(controls[activeProfile]->intakeLiftToggle.first) 
                    && (activeProfile == MODE_COMP ? controls[activeProfile]->intakeLiftToggle.second : true)) {
                    pneumatics.intakeLift.toggle();
                }

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile]->climbMode_1) && ctrler.get_digital(controls[activeProfile]->climbMode_2)) {
                    activeProfile = MODE_SOLO_CLIMB;
                    updateRobotSystems(activeProfile);
                }
                break;
            case MODE_SOLO_CLIMB:
            case MODE_COMP_CLIMB:
                actions.setOverride(ctrler.get_digital(controls[activeProfile]->shift));
                /// ARM ///
                if(actions.getOverride()){
                    if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageUp)) {
                        actions.setArmSpeed(.7);
                        targetAngle = 999;
                    } else if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageDown)) {
                        actions.setArmSpeed(-.7);
                        targetAngle = 999;
                    } else {
                        actions.setArmSpeed(0);
                        targetAngle = actions.getArmAngle(); //updates arm angle
                    }
                } else {
                    if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageUp)) {
                        actions.setArmState(Arm::State::CLIMB);
                        targetAngle = 999;
                    } else if(ctrler.get_digital(controls[activeProfile]->backpackCycleStageDown)) {
                        actions.setArmState(Arm::State::SCORE);
                        targetAngle = 999;
                    } else {
                        if(actions.getArmAngle() < 80) {
                            actions.setArmState(Arm::State::CLIMB);
                            targetAngle = 999;
                        } else {
                            targetAngle = actions.getArmAngle(); //updates arm angle
                        }
                    }
                }

                /// PNEUMATICS ///
                // if (ctrler.get_digital_new_press(controls[activeProfile]->intakeLiftToggle.first) 
                //     && (activeProfile == MODE_COMP ? controls[activeProfile]->intakeLiftToggle.second : true)) {
                //     pneumatics.intakeLift.toggle();
                // }
                // if (ctrler.get_digital_new_press(controls[activeProfile]->intakeLockToggle.first) 
                //     && (activeProfile == MODE_COMP ? controls[activeProfile]->intakeLockToggle.second : true)) {
                //     pneumatics.intakeLock.toggle();
                // }
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
                    } else {
                        chassis->changeDriveMode(controls[MODE_SOLO]->driveMode);
                        chassis->changeDriveMotors(leftDrive, rightDrive);
                        chassis->setBrakeMode(BRAKE_COAST);
                    }
                }        
                if(ctrler.get_digital_new_press(controls[activeProfile]->leftPaperToggle)) {
                    pneumatics.paperLeft.toggle();
                }
                if(ctrler.get_digital_new_press(controls[activeProfile]->rightPaperToggle)) {
                    pneumatics.paperRight.toggle();
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