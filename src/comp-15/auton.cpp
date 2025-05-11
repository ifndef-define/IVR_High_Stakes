#include "robots/comp-15/auton.h"

void auton(Ring::Color ringToKeep) {
    pros::Task fsm([&](){
        while (pros::competition::is_autonomous()) {
            actions.runSubsystemFSM();
            pros::delay(10);
        }
        // fsm.suspend();
    });

    blueAuton2();

}

void postRedMogoRush() {
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
    chassis->translateBy(6, 600, false);
    chassis->translateBy(-4, 400, false);

    // delay(450);
    actions.setArmState(Arm::State::DESCORE); // score preload on alliance stake
    delay(1000);
    chassis->translateBy(-10, 2000, false, 30, 35);
    // i_waitUntil(!chassis->isInMotion());
    actions.setArmState(Arm::State::DOWN);  
    chassis->turnToAngle(23, 1200, false, 90, 1);
    chassis->translateBy(38, 1500, true);
    actions.setIntakeSpeed(-1);
    delay(500);
    actions.setIntakeSpeed(1);
    i_waitUntil(!chassis->isInMotion());
    delay(500);
    chassis->translateBy(25, 2000, false, 30, 35);
    delay(500);
    chassis->translateBy(-28, 2000, true);
    actions.setIntakeSpeed(-1);
    delay(500);
    actions.setIntakeSpeed(1);
    i_waitUntil(!chassis->isInMotion());
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
}

void postBlueMogoRush() {
    actions.setIntakeSpeed(-1);
    delay(250);
    actions.setIntakeSpeed(0);
    i_waitUntil(robotOdom->getPos().y < 38);
    pneumatics.mogoRushTeeth.retract(); // release mogo
    // preload ring goes into Ladybrown
    actions.setArmState(Arm::State::READY);
    i_waitUntil(!chassis->isInMotion());
    // Get alliance goal
    chassis->turnToAngle(182, 1200, true, 127, 1);
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
    chassis->swingToAngle(270, Drive::DriveSide::LEFT, 1700, false, 90);
    // delay(450);
    chassis->translateBy(5, 500, false, 30, 35);
    chassis->translateBy(-7, 1000, false, 30, 35);
    actions.setArmState(Arm::State::DESCORE); // score preload on alliance stake
    delay(1000);
    chassis->translateBy(-10, 2000, false, 30, 35);
    // i_waitUntil(!chassis->isInMotion());
    actions.setArmState(Arm::State::DOWN);  
    chassis->turnToAngle(160, 1200, false, 90, 1);
    chassis->translateBy(38, 1500, true);
    actions.setIntakeSpeed(-1);
    delay(500);
    actions.setIntakeSpeed(1);
    i_waitUntil(!chassis->isInMotion());
    delay(1000);
    chassis->translateBy(28, 2000, false, 30, 35);
    delay(500);
    chassis->translateBy(-32, 2000, false);
    chassis->turnToAngle(225, 800, false, 90, 1);
    chassis->translateBy(45, 2500, false, 30, 35);
    chassis->translateBy(-15, 1000, false);
    chassis->turnToAngle(225, 300, false, 90, 1);
    chassis->translateBy(25, 1000, false);
    chassis->translateBy(-15, 1000, false);
    chassis->turnToAngle(225, 300, false, 90, 1);
    chassis->translateBy(25, 1000, false);
    chassis->translateBy(-20, 1000, false);
    chassis->turnToAngle(45, 1000, false, 90, 1);
    pneumatics.mogoClamp.retract();
    chassis->translateBy(-25, 1000, false);
    chassis->translateBy(25, 1000, false);
}

void redAuton1() {
    chassis->setBrakeMode(BRAKE_BRAKE);
    actions.setRunArm(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    
    // Reset Pos
    odom::setPos({95.625,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(87, 57, 999, false, 1200, true);
    pneumatics.leftMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(150);
    pneumatics.leftMogoRushArm.retract();
    // Move back to ladder ring
    chassis->moveToPose(95.625, 24, 90, true, 2250, true);

    postRedMogoRush();
}

void redAuton2() {
    chassis->setBrakeMode(BRAKE_BRAKE);
    actions.setRunArm(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    
    // Reset Pos
    odom::setPos({95.625,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(104.25, 57, 999, false, 1200, true);
    pneumatics.leftMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(150);
    pneumatics.leftMogoRushArm.retract();
    // Move back to ladder ring
    chassis->moveToPose(95.625, 24, 90, true, 2250, true);

    postRedMogoRush();
}

void blueAuton1() {
    chassis->setBrakeMode(BRAKE_BRAKE);
    actions.setRunArm(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    
    // Reset Pos
    odom::setPos({46.125,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(56, 57, 999, false, 1200, true);
    pneumatics.rightMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(150);
    pneumatics.rightMogoRushArm.retract();
    // Move back to ladder ring
    chassis->moveToPose(46.125, 20, 90, true, 2250, true);

    postBlueMogoRush();
}

void blueAuton2() {
    
    chassis->setBrakeMode(BRAKE_BRAKE);
    actions.setRunArm(true);
    actions.setRunColorSort(true);
    actions.setAutonControlFlag(true);
    
    // Reset Pos
    odom::setPos({46.125,20.25,90});
    // Release zip
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    // Move to middle mogo
    chassis->moveToPose(37.5, 57, 999, false, 1200, true);
    pneumatics.leftMogoRushArm.extend();
    i_waitUntil(!chassis->isInMotion());
    delay(150);
    pneumatics.leftMogoRushArm.retract();
    // Move back to ladder ring
    chassis->moveToPose(46.125, 20, 90, true, 2250, true);

    postBlueMogoRush();
}