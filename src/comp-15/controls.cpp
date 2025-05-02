#include "robots/comp-15/controls.h"

struct driverProfile {
    Drive::drive_mode_e driveMode;

    pros::controller_digital_e_t intakeIn;
    pros::controller_digital_e_t intakeOut;

    pros::controller_digital_e_t backpackCycleStageUp;
    pros::controller_digital_e_t backpackCycleStageDown;
    bool jesusSpecial;

    pros::controller_digital_e_t mogoClampToggle;
    pros::controller_digital_e_t rightMogoRushCycle;
    pros::controller_digital_e_t leftMogoRushCycle;
    pros::controller_digital_e_t mogoRushTeethToggle;
    pros::controller_digital_e_t intakeLift;

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
    .driveMode = Drive::drive_mode_e::SPLIT_ARCADE_PL,
    
    .intakeIn = BUTTON_R1,
    .intakeOut = BUTTON_R2,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .jesusSpecial = false,

    .mogoClampToggle = BUTTON_Y,
    .rightMogoRushCycle = BUTTON_A,
    .leftMogoRushCycle = BUTTON_A,
    .mogoRushTeethToggle = BUTTON_UP,
    // .intakeLock = BUTTON_A,  // retracted

    .climbMode_1 = BUTTON_B,
    .climbMode_2 = BUTTON_DOWN,

    // .intake_wingsToggle = null, // dropped and folded
    // .innerClimbArmsToggle = null, // stowed
    // .outerClimbArmsToggle = null, // stowed
    // .climbPTOToggle = null, // disengaged

    // .toggleColorSort = null // off by default
};


const driverProfile CompClimbMode = {
    .driveMode = Drive::drive_mode_e::CUSTOM_m,
    
    // .intakeIn = null, // no control
    // .intakeOut = null, // no control

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .jesusSpecial = false,

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null, // stowed
    // .leftMogoRushCycle = null, // stowed
    // .mogoRushTeethToggle = null, // retracted
    // .intakeLock = null, // automatic

    .climbMode_1 = BUTTON_B,
    .climbMode_2 = BUTTON_DOWN,

    // .intake_wingsToggle = BUTTON_A, // this macros intakeLock
    // .innerClimbArmsToggle = BUTTON_RIGHT,
    // .outerClimbArmsToggle = BUTTON_LEFT,
    // .leftPaperToggle = BUTTON_R1,
    // .rightPaperToggle = BUTTON_R2,
    // .climbPTOToggle = BUTTON_UP,

    // .toggleColorSort = null // off
};

const driverProfile SoloDriveMode = {
    .driveMode = Drive::drive_mode_e::SPLIT_ARCADE_PL,

    .intakeIn = BUTTON_R1,
    .intakeOut = BUTTON_R2,

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .jesusSpecial = false,

    .mogoClampToggle = BUTTON_Y,
    .rightMogoRushCycle = BUTTON_RIGHT,
    .leftMogoRushCycle = BUTTON_LEFT,
    .mogoRushTeethToggle = BUTTON_DOWN,
    .intakeLift = BUTTON_B,

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
    .driveMode = Drive::drive_mode_e::CUSTOM_m,
    
    // .intakeIn = null, // no control
    // .intakeOut = null, // no control

    .backpackCycleStageUp = BUTTON_L1,
    .backpackCycleStageDown = BUTTON_L2,
    .jesusSpecial = false,

    // .mogoClampToggle = null, // Lock clamped
    // .rightMogoRushCycle = null, // stowed
    // .leftMogoRushCycle = null, // stowed
    // .mogoRushTeethToggle = null, // disabled
    .intakeLift = BUTTON_A, // automatic

    .climbMode_1 = BUTTON_UP,
    .climbMode_2 = BUTTON_X,

    .leftWingToggle = BUTTON_R1,
    .rightWingToggle = BUTTON_R2,
    .innerClimbArmsToggle = BUTTON_RIGHT,
    .outerClimbArmsToggle = BUTTON_LEFT,
    .climbPTOToggle = BUTTON_Y,

    // .toggleColorSort = null // off
};


/* Climb Operations:
Before Climb
- Get ring in arm
- Hold clamp mogo if climbing with

Switch climb menu (this locks intake, toggles wing, and deploys inner arms)
- Line up with ladder
- Engage pto (this enables arm control and disables turning in drive) 
- Driver starts climbing
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
DriveMode activeProfile = MODE_SOLO;

extern pros::MotorGroup leftDrive, rightDrive, rightClimbDrive, leftClimbDrive;
void updateRobotSystems(DriveMode newMode, Ring::Color botSide) {
    ctrler.rumble("...");
    delay(100);
    ctrler.clear_line(3);
    delay(100);

    switch (newMode) {
        case MODE_SOLO:
            ctrler.print(0, 0, "Solo - Drive");
            delay(100);
            pneumatics.leftClimbWing.retract();
            pneumatics.rightClimbWing.retract();
            pneumatics.innerClimbArms.retract();
            pneumatics.outerClimbArms.retract();
            pneumatics.mogoRushTeeth.extend();
            pneumatics.climbPTO.retract();
            chassis->changeDriveMode(controls[activeProfile].driveMode);
            chassis->changeDriveMotors(leftDrive, rightDrive);
            actions.setRunColorSort(false);
            break;
        case MODE_SOLO_CLIMB:
            ctrler.print(0, 0, "Solo - Climb");
            delay(100);
            pneumatics.mogoClamp.extend();
            pneumatics.leftMogoRushArm.retract();
            pneumatics.rightMogoRushArm.retract();
            pneumatics.mogoRushTeeth.retract();
            pneumatics.innerClimbArms.extend();
            actions.setRunColorSort(false);
            break;
        case MODE_COMP:
            ctrler.print(0, 0, "Comp - Drive");
            delay(100);
            pneumatics.leftClimbWing.retract();
            pneumatics.rightClimbWing.retract();
            pneumatics.innerClimbArms.retract();
            pneumatics.outerClimbArms.retract();
            pneumatics.mogoRushTeeth.extend();
            pneumatics.climbPTO.retract();
            chassis->changeDriveMode(controls[activeProfile].driveMode);
            chassis->changeDriveMotors(leftDrive, rightDrive);
            actions.setRunColorSort(true);
            break;
        case MODE_COMP_CLIMB:
            ctrler.print(0, 0, "Comp - Climb");
            delay(100);
            pneumatics.mogoClamp.extend();
            pneumatics.leftMogoRushArm.retract();
            pneumatics.rightMogoRushArm.retract();
            pneumatics.mogoRushTeeth.retract();
            pneumatics.innerClimbArms.extend();
            pneumatics.outerClimbArms.retract();
            actions.setRunColorSort(false);
            break;  
    }

    delay(500); // bounce time
}
pros::Motor armTemp(3, pros::MotorGear::red);
void teleOp(Ring::Color ringToKeep, bool forceCompMode) {

    chassis->loop(true);

    if (!pros::competition::is_connected() && !forceCompMode) {
        activeProfile = MODE_SOLO;
        updateRobotSystems(activeProfile, ringToKeep);
    } else {
        activeProfile = MODE_COMP;
        updateRobotSystems(activeProfile, ringToKeep);
    }

    actions.setRingColor(ringToKeep);
    actions.setAutonControlFlag(false);
    actions.setRunColorSort(false);
    actions.setRunAutoMogoClamp(false);
    actions.setRunArm(true);
    // actions.setArmState(Arm::State::DOWN);

    while(1) {
        actions.runSubsystemFSM();

        switch (activeProfile) {
            case MODE_SOLO:
                /// INTAKE ///
                if(ctrler.get_digital(controls[activeProfile].intakeIn)) {
                    actions.setIntakeSpeed(1);
                } else if(ctrler.get_digital(controls[activeProfile].intakeOut)) {
                    actions.setIntakeSpeed(-1);
                } else {
                    actions.setIntakeSpeed(0);
                }

                /// ARM ///
                if(actions.getOverride()){
                    if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                        actions.setArmSpeed(1);
                    } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                        actions.setArmSpeed(-1);
                    } else {
                        actions.setArmSpeed(0);
                    }
                } else {
                    if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp) && ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                        actions.setArmState(Arm::State::DESCORE);
                    } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                        actions.setArmState(Arm::State::READY);
                    } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                        actions.setArmState(Arm::State::SCORE);
                    } else {
                        actions.setArmState(Arm::State::DOWN);
                    }
                }
                // if(actions.getOverride()){
                //     if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                //         actions.setArmSpeed(1);
                //     } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                //         actions.setArmSpeed(-1);
                //     } else {
                //         actions.setArmSpeed(0);
                //     }
                // } else {
                //     if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageUp)) {
                //         if(actions.getArmState()==Arm::State::READY) {
                //             actions.setArmState(Arm::State::DOWN);
                //         } else {
                //             actions.setArmState(Arm::State::READY);
                //         }
                //     } else if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageDown)) {
                //         actions.nextArmState();
                //     }
                // }
                // if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                //     armTemp.move(127);
                // } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                //     armTemp.move(-127);
                // } else {
                //     armTemp.brake();
                // }

                /// PNEUMATICS ///
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
                if(ctrler.get_digital_new_press(controls[activeProfile].intakeLift)) {
                    pneumatics.intakeLift.toggle();
                }

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_SOLO_CLIMB;
                    updateRobotSystems(activeProfile, ringToKeep);
                }
                break;
            case MODE_SOLO_CLIMB:
                /// ARM ///
                if(actions.getOverride()){
                    if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                        actions.setArmSpeed(1);
                    } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                        actions.setArmSpeed(-1);
                    } else {
                        actions.setArmSpeed(0);
                    }
                } else {
                    if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageUp)) {
                        if(actions.getArmState()==Arm::State::READY) {
                            actions.setArmState(Arm::State::DOWN);
                        } else {
                            actions.setArmState(Arm::State::READY);
                        }
                    } else if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageDown)) {
                        actions.nextArmState();
                    }
                }

                /// PNEUMATICS ///
                if (ctrler.get_digital_new_press(controls[activeProfile].intakeLift)) {
                    pneumatics.intakeLift.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile].innerClimbArmsToggle)) {
                    pneumatics.innerClimbArms.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile].outerClimbArmsToggle)) {
                    pneumatics.outerClimbArms.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile].leftWingToggle)) {
                    pneumatics.leftClimbWing.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile].rightWingToggle)) {
                    pneumatics.rightClimbWing.toggle();
                }

                if (ctrler.get_digital_new_press(controls[activeProfile].climbPTOToggle)) {
                    pneumatics.climbPTO.toggle();
                    if (pneumatics.climbPTO.is_extended()) {
                        // chassis->stopLoop();
                        chassis->changeDriveMotors(leftClimbDrive, rightClimbDrive);
                        chassis->changeDriveMode(controls[activeProfile].driveMode);
                        chassis->setBrakeMode(BRAKE_BRAKE); // switch to hold?
                        // chassis->loop(true);
                    } else if (!(pneumatics.climbPTO.is_extended())) {
                        chassis->changeDriveMode(controls[MODE_SOLO].driveMode);
                        chassis->changeDriveMotors(leftDrive, rightDrive);
                        chassis->setBrakeMode(BRAKE_COAST);
                    }
                }          

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_SOLO;
                    updateRobotSystems(activeProfile, ringToKeep);
                }
                break;
            case MODE_COMP:
                /// INTAKE ///
                if(ctrler.get_digital(controls[activeProfile].intakeIn)) {
                    actions.setIntakeSpeed(1);
                } else if(ctrler.get_digital(controls[activeProfile].intakeOut)) {
                    actions.setIntakeSpeed(-1);
                } else {
                    actions.setIntakeSpeed(0);
                }

                /// ARM ///
                if(actions.getOverride()){
                    if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                        actions.setArmSpeed(1);
                    } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                        actions.setArmSpeed(-1);
                    } else {
                        actions.setArmSpeed(0);
                    }
                } else {
                    if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageUp)) {
                        if(actions.getArmState()==Arm::State::READY) {
                            actions.setArmState(Arm::State::DOWN);
                        } else {
                            actions.setArmState(Arm::State::READY);
                        }
                    } else if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageDown)) {
                        actions.nextArmState();
                    }
                }

                /// PNEUMATICS ///
                if(ctrler.get_digital_new_press(controls[activeProfile].mogoClampToggle)) {
                    pneumatics.mogoClamp.toggle();
                    if(pneumatics.mogoClamp.is_extended()) {
                        ctrler.rumble("..");
                    } else {
                        ctrler.rumble("--");
                    }
                }
                
                if (ringToKeep == Ring::Color::RED) {
                    if(ctrler.get_digital_new_press(controls[activeProfile].leftMogoRushCycle)) {
                        pneumatics.leftMogoRushArm.toggle();
                    }
                } else if (ringToKeep == Ring::Color::BLUE) {
                    if(ctrler.get_digital_new_press(controls[activeProfile].rightMogoRushCycle)) {
                        pneumatics.rightMogoRushArm.toggle();
                    }
                }
                if(ctrler.get_digital_new_press(controls[activeProfile].mogoRushTeethToggle)) {
                    pneumatics.mogoRushTeeth.toggle();
                }

                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_COMP_CLIMB;
                    updateRobotSystems(activeProfile, ringToKeep);
                }
                break;
            case MODE_COMP_CLIMB:
                /// ARM ///
                if(actions.getOverride()){
                    if(ctrler.get_digital(controls[activeProfile].backpackCycleStageUp)) {
                        actions.setArmSpeed(1);
                    } else if(ctrler.get_digital(controls[activeProfile].backpackCycleStageDown)) {
                        actions.setArmSpeed(-1);
                    } else {
                        actions.setArmSpeed(0);
                    }
                } else {
                    if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageUp)) {
                        if(actions.getArmState()==Arm::State::READY) {
                            actions.setArmState(Arm::State::DOWN);
                        } else {
                            actions.setArmState(Arm::State::READY);
                        }
                    } else if(ctrler.get_digital_new_press(controls[activeProfile].backpackCycleStageDown)) {
                        actions.nextArmState();
                    }
                }

                /// PNEUMATICS ///
                // if (ctrler.get_digital_new_press(controls[activeProfile].intake_wingsToggle)) {
                //     if(!pneumatics.intakeLift_Wings.is_extended()) {
                //         pneumatics.intakeLock.retract();
                //         delay(150);
                //         pneumatics.intakeLift_Wings.toggle();
                //     } else {
                //         pneumatics.intakeLift_Wings.toggle();
                //         delay(350);
                //         pneumatics.intakeLock.extend();
                //     }
                // }
                if (ctrler.get_digital_new_press(controls[activeProfile].innerClimbArmsToggle)) {
                    pneumatics.innerClimbArms.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile].outerClimbArmsToggle)) {
                    pneumatics.outerClimbArms.toggle();
                }
                if (ctrler.get_digital_new_press(controls[activeProfile].climbPTOToggle)) {
                    pneumatics.climbPTO.toggle();
                    if (pneumatics.climbPTO.is_extended()) {
                        // chassis->stopLoop();
                        chassis->changeDriveMotors(leftClimbDrive, rightClimbDrive);
                        chassis->changeDriveMode(controls[activeProfile].driveMode);
                        chassis->setBrakeMode(BRAKE_BRAKE); // switch to hold?
                        // chassis->loop(true);
                    } else if (!(pneumatics.climbPTO.is_extended())) {
                        chassis->changeDriveMode(controls[MODE_COMP].driveMode);
                        chassis->changeDriveMotors(leftDrive, rightDrive);
                        chassis->setBrakeMode(BRAKE_COAST);
                    }
                }
            
                // Mode Change //
                if(ctrler.get_digital(controls[activeProfile].climbMode_1) && ctrler.get_digital(controls[activeProfile].climbMode_2)) {
                    activeProfile = MODE_COMP;
                    updateRobotSystems(activeProfile, ringToKeep);
                }
                break;
        }

        delay(10);
    }
}

// void teleOp(Ring::Color ringToKeep) {
//     int pow, turn, rushState = 0;
//     actions.setRingColor(ringToKeep);
//     actions.setAutonControlFlag(false);
//     actions.setRunColorSort(true);
//     actions.setArmState(Arm::State::DOWN);

//     while(1) {
//         // Drive Control
//         pow = ctrler.get_analog(currentProfile.powerAxis);
//         turn = ctrler.get_analog(currentProfile.turnAxis)/1.3;

//         leftDrive.move(pow + turn);
//         rightDrive.move(pow - turn);
        
//         // Sorting Control
//         // actions.runSubsystemFSM();

//         // Intake/Arm Control
//         actions.setOverride(ctrler.get_digital(currentProfile.shift));
//         if(actions.getState() != ActionState::SORTING || actions.getOverride()){
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
//                         if(actions.getArmState()==Arm::State::READY) {
//                             actions.setArmState(Arm::State::DOWN);
//                         } else {
//                             actions.setArmState(Arm::State::READY);
//                         }
//                     } else if(ctrler.get_digital_new_press(currentProfile.backpackCycleStageDown)) {
//                         actions.nextArmState();
//                     }
//             } else {
//                 if(ctrler.get_digital(currentProfile.backpackCycleStageDown)) {
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

//         //Print out data for 
//         // pros::lcd::print(1, "%f", actions.getArmAngle());
//         // pros::lcd::print(2, "%d", actions.getPullbackFlag());
//         // pros::lcd::print(3, "%d", int(actions.getState()));
//         // pros::delay(10);
//     }
// }