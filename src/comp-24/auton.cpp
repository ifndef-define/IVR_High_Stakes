#include "robots/comp-24/auton.h"

void auton(Ring::Color ringToKeep) {
    pros::Task fsm([&]{
        while(pros::competition::is_autonomous()){
            actions.runSubsystemFSM(); 
            delay(10);
        }
        chassis->cancelAllMotions();
    });


    if (ringToKeep == Ring::Color::RED) {
        // pros::Task redAutonTask([&]{
            redAuton1();
        // });
        
    } else if (ringToKeep == Ring::Color::BLUE) {
        // pros::Task blueAutonTask([&]{
            blueAuton1();
        // });
    }
}

void redAuton1() {
    // Add red auton 1 code here
    ctrler.rumble(".");

    odom::setPos(odom::r_coord(34.875, 35.125, 297.95));
    actions.setRunColorSort(true);

    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    pneumatics.leftMogoRushArm.extend();
    
    // Auton Code: //
    chassis->translateBy(-18.5, 800, false, 127, 127, .25, 1); 
    chassis->translateBy(18.5, 5000, false, 127, 127, .675, 1);
    pneumatics.mogoRushTeeth.retract();
    pneumatics.mogoClamp.retract();
    
    // chassis->swingToAngle(280, Drive::DriveSide::LEFT, 2500, false, 127, .5);
    pneumatics.leftMogoRushArm.retract();
    chassis->turnToAngle(280, 900, false, 127, 1);

    chassis->moveToPose(33, 53, 999, false, 1800, true, 0, 70, 50, .675, 1);
    // chassis->translateBy(-18, 2500, false, 0, 60, .5, .5);
    
    // CLAMPING MOGO AUTO /// true
    
    // delay(1200);
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    
    // Clean knocked rings
    // chassis->translateBy(6, 1500, false, 0, 60);
    actions.setIntakeSpeed(1);
    chassis->turnToAngle(173, 1200, false, 127, 1);
    // chassis->swingToAngle(105, Drive::DriveSide::RIGHT, 1500, false);
    // chassis->moveToPose(17, 55, 999, false, 1500, false, 0, 45,45,.5,1);
    chassis->translateBy(9, 1500, false, 30, 50, .5, 1);//fiurst ring
    chassis->translateBy(-11, 1500, false, 20, 40, .5, 1);//fiurst ring
    // chassis->turnToAngle(160, 600, false, 127, 1);
    // chassis->translateBy(13, 1500, false, 20, 50, .5, 1);
    // delay(200);
    // chassis->translateBy(-5, 2000, false, 5, 30, .5, 1);//fiurst ring
    
    // MIDLINE RINGS
    // chassis->translateBy(5, 7000, false, 0, 45, .675, 1);
    // chassis->moveToPose(10, 63, 135, false, 1500, true, 0, 45,45,.5,1);
    // delay(350);
    // pneumatics.intakeLift.retract();
    // i_waitUntil(!chassis->isInMotion());
    // delay(250);
    // pneumatics.intakeLift.extend();
    // delay(500);
    // chassis->turnToAngle(270, 1500, false, 30, 1);
    // delay(400);
    chassis->moveToPose(24, 15, 270, false, 2000, false, 65, 127, 45, .675, 1);
    chassis->translateBy(-6, 1500, false, 20, 40, .5, 1);//fiurst ring
    chassis->turnToAngle(225, 1200, false, 127, 1);

    // To Corner
    // chassis->translateBy(6, 1000, false, 0, 45, .5, 1);//fiurst ring
    // odom::r_coord pos = odom::getPos();
    for(int i = 0; i < 3; i++) {
        // delay(250);
        if(i != 0) {chassis->turnToAngle(225, 600, false, 127, 1);}
        if(i == 0) {chassis->translateBy(31, 1200, false, 40, 70, .5, 1);}
        else {chassis->translateBy(20, 1200, false, 40, 60, .5, 1);}
        // chassis->moveToPose(pos.x, pos.y, 225, false, 1500, false, 0, 75, 75, .5, 1);
        chassis->translateBy(-11, 1200, false, 55, 75, .5, 1);//fiurst ring
    }
    
    delay(500);

    //To Ladder
    // chassis->translateBy(-45, 1500, false, 45, 80, .5, 1);//fiurst ring
    chassis->turnToAngle(40, 1200, false, 127, 1);
    chassis->moveToPose(75, 50, 40, false, 3000, false, 0, 95, 45, .675, 1);
}


void redAuton2() {
    // Add red auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("..");
}

void blueAuton1() {
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("...");


    // Mirror coordinates: (144-34.875, 35.125, (180-297.95)%360)
    odom::setPos(odom::r_coord(109.125, 35.125, 242.05));
    // actions.setRunAutoMogoClamp(true);
    actions.setRunColorSort(true);

    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    pneumatics.rightMogoRushArm.extend();
    // pneumatics.intakeLock.extend();
    
    // Auton Code: //
    chassis->translateBy(-18.5, 800, false, 127, 127, .25, 1); 
    chassis->translateBy(18.5, 7000, false, 127, 127, .675, 1);
    pneumatics.mogoRushTeeth.retract();
    pneumatics.mogoClamp.retract();
    
    // chassis->swingToAngle(280, Drive::DriveSide::LEFT, 2500, false, 127, .5);
    pneumatics.rightMogoRushArm.retract();
    chassis->turnToAngle(255, 900, false, 127, 1);
    // delay(700);

    chassis->moveToPose(111, 53, 999, true, 1800, false, 0, 70, 50, .675, 1);
    // chassis->translateBy(-18, 2500, false, 0, 60, .5, .5);
    
    // CLAMPING MOGO AUTO ///
    
    // delay(1200);
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    
    // Clean knocked rings
    // chassis->translateBy(6, 1500, false, 0, 60);
    actions.setIntakeSpeed(1);
    chassis->turnToAngle(14, 1200, false, 127, 1);
    // chassis->swingToAngle(105, Drive::DriveSide::RIGHT, 1500, false);
    // chassis->moveToPose(17, 55, 999, false, 1500, false, 0, 45,45,.5,1);
    chassis->translateBy(9, 1500, false, 30, 50, .5, 1);//fiurst ring
    chassis->translateBy(-11, 1500, false, 20, 40, .5, 1);//fiurst ring
    // chassis->turnToAngle(20, 600, false, 127, 1);
    // chassis->translateBy(13, 1500, false, 20, 50, .5, 1);
    // delay(200);
    // chassis->translateBy(-10, 2000, false, 5, 30, .5, 1);//fiurst ring
    
    // MIDLINE RINGS
    // chassis->translateBy(5, 7000, false, 0, 45, .675, 1);
    // chassis->moveToPose(134, 65, 45, false, 1500, true, 0, 45,45,.5,1);
    // chassis->turnToAngle(285, 1500, false, 30, 1);
    // delay(400);
    chassis->moveToPose(127, 15, 270, false, 2000, false, 65, 127, 45, .675, 1);
    chassis->translateBy(-6, 1500, false, 20, 40, .5, 1);//fiurst ring
    chassis->turnToAngle(310, 1200, false, 127, 1);

    // To Corner
    // chassis->translateBy(6, 1000, false, 0, 45, .5, 1);//fiurst ring
    // odom::r_coord pos = odom::getPos();
    for(int i = 0; i < 3; i++) {
        // delay(250);
        if(i != 0) {chassis->turnToAngle(315, 600, false, 127, 1);}
        if(i == 0) {chassis->translateBy(31, 1200, false, 40, 70, .5, 1);}
        else {chassis->translateBy(20, 1200, false, 30, 50, .5, 1);}
        // chassis->moveToPose(pos.x, pos.y, 225, false, 1500, false, 0, 75, 75, .5, 1);
        chassis->translateBy(-11, 1200, false, 35, 55, .5, 1);//fiurst ring
    }
    delay(500);

    //To Ladder
    // chassis->translateBy(-45, 1500, false, 45, 80, .5, 1);//fiurst ring
    chassis->turnToAngle(140, 1200, false, 127, 1);
    chassis->moveToPose(69, 50, 140, false, 3000, false, 0, 95, 45, .675, 1);
}

void blueAuton2() {
    // Add blue auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("....");
}