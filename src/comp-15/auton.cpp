#include "robots/comp-15/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
    // if (ui::getCurrentAuto() == 0) {
        // blueAuton1();
        // pneumatics.mogoRushTeeth.extend();
        // ctrler.rumble("..");
        // delay(2000);
        // pneumatics.mogoRushTeeth.retract();
        // ctrler.rumble("--");
        // delay(2000);
        // pneumatics.mogoRushTeeth.extend();
        // ctrler.rumble("..");
        // delay(2000);
    // } else if (ui::getCurrentAuto() == 1) {
        redAuton1();
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
    chassis->setBrakeMode(BRAKE_BRAKE);
    actions.setRunArm(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);

    // actions.setArmState(Arm::State::READY);
    // delay(500);
    // actions.setIntakeSpeed(1);
    // delay(500);
    // actions.setIntakeSpeed(-1);
    // delay(150);
    // actions.setArmState(Arm::State::SCORE);
    // actions.setIntakeSpeed(0);
    // delay(1000000000);
    
    // Reset Pos
    odom::setPos({95.625,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(87, 57, 999, false, 1200, true); // mogo rush
    // pneumatics.mogoRushTeeth.extend(); // engage teeth
    pneumatics.leftMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(150);
    pneumatics.leftMogoRushArm.retract();
    // Move back to ladder ring
    // chassis->translateBy(-26, 2000, true, 10, 127, 0.5, 0.5);
    chassis->moveToPose(95.625, 24, 90, true, 2250, true);
    actions.setIntakeSpeed(-1);
    delay(250);
    actions.setIntakeSpeed(0);
    i_waitUntil(robotOdom->getPos().y < 38);
    pneumatics.mogoRushTeeth.retract(); // release mogo
    // preload ring goes into Ladybrown
    actions.setArmState(Arm::State::READY);
    i_waitUntil(!chassis->isInMotion());
    // Get alliance goal
    chassis->turnToAngle(0, 1200, true, 127, 1);
    actions.setArmState(Arm::State::READY);
    pneumatics.mogoClamp.retract();
    delay(750);
    actions.setIntakeSpeed(1);
    // drive into the mogo on our side
    chassis->translateBy(-28, 1500, true, 0, 60);
    delay(600);
    pneumatics.mogoClamp.extend();
    actions.setIntakeSpeed(-1);
    actions.setArmState(Arm::State::STANDBY);

    i_waitUntil(!chassis->isInMotion());
    actions.setIntakeSpeed(1);
    chassis->swingToAngle(-84, Drive::DriveSide::RIGHT, 1700, false, 90);
    chassis->translateBy(2, 200, false);
    // delay(450);
    actions.setArmState(Arm::State::DESCORE); // score preload on alliance stake
    delay(1000);
    chassis->translateBy(-10, 2000, false, 30, 35);
    // i_waitUntil(!chassis->isInMotion());
    actions.setArmState(Arm::State::DOWN);  
    chassis->turnToAngle(20, 1200, false, 90, 1);
    chassis->translateBy(38, 1500, true);
    actions.setIntakeSpeed(-1);
    delay(500);
    actions.setIntakeSpeed(1);
    i_waitUntil(!chassis->isInMotion());
    delay(500);
    chassis->translateBy(25, 2000, false, 30, 35);
    delay(500);
    chassis->translateBy(-32, 2000, false);
    chassis->turnToAngle(315, 800, false, 90, 1);
    chassis->translateBy(45, 2500, false, 30, 35);
    chassis->translateBy(-15, 1000, false);
    chassis->turnToAngle(315, 300, false, 90, 1);
    chassis->translateBy(25, 1000, false);
    chassis->translateBy(-15, 1000, false);
    chassis->turnToAngle(315, 300, false, 90, 1);
    chassis->translateBy(25, 1000, false);
    chassis->translateBy(-15, 1000, false);
    chassis->turnToAngle(315, 300, false, 90, 1);
    chassis->translateBy(25, 1000, false);
    chassis->translateBy(-20, 1000, false);
    chassis->turnToAngle(135, 1200, false, 90, 1);
    pneumatics.mogoClamp.retract();
    chassis->translateBy(-25, 1000, false);
    chassis->translateBy(25, 1000, false);









    // pneumatics.intakeLift.extend();
    // chassis->turnToAngle(80, 1200, false, 127, 1);
    // chassis->translateBy(7, 1500, false);
    // pneumatics.intakeLift.retract();
    // chassis->translateBy(-4, 1200, false);
    // chassis->translateBy(4, 1200, false);


    // pneumatics.intakeLift.retract();
    // actions.setIntakeSpeed(-1);
    // delay(500);
    // actions.setIntakeSpeed(1);
    // delay(1000);
    // chassis->translateBy(-12, 800, false);
    // delay(500);
    // chassis->translateBy(28, 800, false);
    // chassis->turnToAngle(80, 1200, false, 127, 1);
    // actions.setIntakeSpeed(0);  
    // pneumatics.intakeLift.extend();
    // delay(500);
    // chassis->translateBy(12, 6000, true);
    // pneumatics.intakeLift.retract();
    // actions.setIntakeSpeed(-1);
    // delay(500);
    // actions.setIntakeSpeed(1);
    // delay(1000);
    // i_waitUntil(!chassis->isInMotion());
    // chassis->translateBy(-12, 800, false);
    // delay(500);

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
    // Add red auton 1 code here
    // ctrler.rumble(".");

    pros::Task fsm([&](){
        while (true) {
            actions.runSubsystemFSM();
            pros::delay(10);
        }
    });
    chassis->setBrakeMode(BRAKE_BRAKE);
    actions.setRunArm(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    
    // Reset Pos
    odom::setPos({48.375,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(60, 57, 999, false, 1200, true); // mogo rush
    // pneumatics.mogoRushTeeth.extend(); // engage teeth
    pneumatics.rightMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(150);
    pneumatics.rightMogoRushArm.retract();
    // Move back to ladder ring
    // chassis->translateBy(-26, 2000, true, 10, 127, 0.5, 0.5);
    chassis->moveToPose(48.375, 20, 90, true, 2500, true);
    actions.setIntakeSpeed(-1);
    delay(250);
    actions.setIntakeSpeed(0);
    i_waitUntil(robotOdom->getPos().y < 38);
    pneumatics.mogoRushTeeth.retract(); // release mogo
    // preload ring goes into Ladybrown
    actions.setArmState(Arm::State::READY);
    i_waitUntil(!chassis->isInMotion());
    // Get alliance goal
    chassis->turnToAngle(180, 1200, false, 127, 1);
    actions.setArmState(Arm::State::READY);
    pneumatics.mogoClamp.retract();
    actions.setIntakeSpeed(1);
    // drive into the mogo on our side
    chassis->translateBy(-26.5, 1100, false);
    pneumatics.mogoClamp.extend();
    actions.setIntakeSpeed(0);
    actions.setArmState(Arm::State::SCORE);
    delay(300);
    chassis->swingToAngle(272, Drive::DriveSide::LEFT, 1700, false, 90);
    actions.setIntakeSpeed(1);
    chassis->translateBy(8, 1200, false);
    chassis->translateBy(-8, 1200, false);
    actions.setArmState(Arm::State::DESCORE); // score preload on alliance stake
    delay(300);
    chassis->translateBy(-3.5, 800, false);
    actions.setArmState(Arm::State::DOWN);  
    chassis->turnToAngle(150, 1200, false, 127, 1);
    chassis->translateBy(39, 1500, false);
    chassis->translateBy(20, 3000, false, 45, 65);
}


void blueAuton2() {
    // Add blue auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("....");
}