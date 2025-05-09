#include "robots/comp-15/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
    // if (ui::getCurrentAuto() == 0) {
        redAuton1();
        // pneumatics.mogoRushTeeth.retract();
        // ctrler.rumble("..");
        // delay(2000);
        // pneumatics.mogoRushTeeth.extend();
        // ctrler.rumble("..");
        // delay(2000);

    // } else if (ui::getCurrentAuto() == 1) {
    //     redAuton2();
    // } else if (ui::getCurrentAuto() == 2) {
    //     blueAuton1();
    // } else if (ui::getCurrentAuto() == 3) {
    //     blueAuton2();
    // } else {
    //     // No auton
    //     ctrler.rumble("--");
    // }
}

/*
Middle Mogo Auton
==========================================
- MogoClamp down
- translate by to the middle mogo (async) (Move to pose?)
- mogoRush down
- wait til translate by is done
- translate back to ladder ring (async)
- release goal by ladder
- arm to ready
- mogoClamp up
- intake down
- intake run to wallstake ring
- wait for translate to finish
- swing to clamp alance goal (async)
- autoclamp alliance goal
- intake stop
- mogoRush up
- wait for swing to finish
- turn to face alliance stake
- arm to score
- intake red ring to goal
- translate to stake
- score aliance
- intake off
- back up to goal (async)
- arm down
- wait for backup to finish
- turn to face inner ring stack
- translate to inner ring stack (async)
- intake lift up and running
- wait for translate to finish
- drop intake down
- translate a bit back
- translate to netural stack
- intake off
- turn to face netural stack
- translate to netural stack (async)
- intake on
- intake bottom ring
- back up to inner ring stack
- swing to outer ring stack
- translate a little past the outer ring stack
- turn to positive corner
- translate to positive corner 
- cycler through the rings x4
- back out a little bit
- 180
- back into the corner
- drop mogo down
- translate halfway to the ladder
- 180
- back into mogorushed goal
- end auto
*/

void postRedMogoRush() {
    // Add post red mogo rush code here
    pneumatics.intakeLift.extend();
    chassis->moveToPose(110,49,999,false,3000,true);
    i_waitUntil(robotOdom->getPos().x > 95);
    actions.setIntakeSpeed(1);
    i_waitUntil(!chassis->isInMotion());
    pneumatics.intakeLift.retract();
    delay(400);
    chassis->translateBy(-5, 1000, false, 0, 127, 0.5, 1);
    delay(9000);
}

void redAuton1() {
    // Add red auton 1 code here
    // ctrler.rumble(".");

    pros::Task fsm([&](){
        while (true) {
            actions.runSubsystemFSM();
            pros::delay(10);
        }
    });
    chassis->setBrakeMode(BRAKE_COAST);
    actions.setRunArm(true);
    // actions.setRunAutoMogoClamp(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    // actions.setArmState(Arm::State::READY);
    
    // Reset Pos
    robotOdom->setPos({95.625,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(86, 56, 999, false, 1200, true); // mogo rush
    pneumatics.mogoRushTeeth.extend(); // engage teeth
    pneumatics.leftMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(250);
    // Move back to ladder ring
    chassis->translateBy(-24, 3000, true, 0, 127, 0.5, 1);
    i_waitUntil(robotOdom->getPos().y < 44);
    pneumatics.mogoRushTeeth.retract(); // release mogo
    // preload ring goes into Ladybrown
    actions.setArmState(Arm::State::READY);
    actions.setIntakeSpeed(1);
    i_waitUntil(!chassis->isInMotion());
    // Get alliance goal
    chassis->swingToAngle(3, Drive::DriveSide::LEFT, 1200, true);
    pneumatics.mogoClamp.retract();
    i_waitUntil(robotOdom->getPos().theta < 60);
    pneumatics.leftMogoRushArm.retract();
    i_waitUntil(!chassis->isInMotion());
    actions.setIntakeSpeed(0);
    // drive into the mogo on our side
    actions.setArmState(Arm::State::SCORE);
    chassis->translateBy(-26.75, 1200, false);
    pneumatics.mogoClamp.extend(); // autoclamp alliance goal
    actions.setIntakeSpeed(1);
    chassis->swingToAngle(-89.5, Drive::DriveSide::RIGHT, 1700, false, 80);
    chassis->translateBy(7.5, 800, false);
    chassis->translateBy(-7.5, 800, false);
    actions.setArmState(Arm::State::DESCORE); // score preload on alliance stake
    delay(300);
    chassis->translateBy(-7.5, 800, false);
    actions.setIntakeSpeed(0);   
    actions.setArmState(Arm::State::DOWN);

    // postRedMogoRush();
}

void redAuton2() {
    // Add red auton 2 code here
    pros::Task fsm([&](){
        while (true) {
            actions.runSubsystemFSM();
            pros::delay(10);
        }
    });
    chassis->setBrakeMode(BRAKE_COAST);
    actions.setRunArm(true);
    // actions.setRunAutoMogoClamp(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    // actions.setArmState(Arm::State::READY);
    
    // Reset Pos
    robotOdom->setPos({95.625,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(110, 58, 999, false, 1200, true); // mogo rush
    pneumatics.mogoRushTeeth.extend(); // engage teeth
    pneumatics.rightMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(250);
    // Move back to ladder ring
    // chassis->moveToPose(95, 30.5, 110, true, 2000, false, 0, 127, 80, 1, 1.5);
    chassis->translateBy(-33, 2000, true, 0, 127, 0.5, 1);
    i_waitUntil(robotOdom->getPos().y < 49);
    pneumatics.mogoRushTeeth.retract(); // release mogo
    i_waitUntil(!chassis->isInMotion());
    // preload ring goes into Ladybrown
    actions.setArmState(Arm::State::READY);
    actions.setIntakeSpeed(1);
    // Get alliance goal
    chassis->swingToAngle(-7, Drive::DriveSide::LEFT, 2000, true);
    i_waitUntil(robotOdom->getPos().theta < 60);
    pneumatics.mogoClamp.retract();
    pneumatics.rightMogoRushArm.retract();
    i_waitUntil(!chassis->isInMotion());
    actions.setIntakeSpeed(0);
    // drive into the mogo on our side
    chassis->translateBy(-27.5, 1100, false);
    actions.setArmState(Arm::State::SCORE);
    pneumatics.mogoClamp.extend(); // autoclamp alliance goal
    actions.setIntakeSpeed(1);
    chassis->swingToAngle(-89, Drive::DriveSide::RIGHT, 1700, false, 80);
    chassis->translateBy(9, 800, false);
    chassis->translateBy(-7.5, 800, false);
    actions.setArmState(Arm::State::DESCORE); // score preload on alliance stake
    chassis->translateBy(-7.5, 800, false);
    actions.setIntakeSpeed(0);   
    actions.setArmState(Arm::State::DOWN);

    postRedMogoRush();
}

void blueAuton1() {
    // Add blue auton 1 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("...");
}

void blueAuton2() {
    // Add blue auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("....");
}