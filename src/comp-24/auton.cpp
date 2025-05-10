#include "robots/comp-24/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
    if (ui::getCurrentAuto() == 0) {
        redAuton1();
    } else if (ui::getCurrentAuto() == 1) {
        redAuton2();
    } else if (ui::getCurrentAuto() == 2) {
        blueAuton1();
    } else if (ui::getCurrentAuto() == 3) {
        blueAuton2();
    } else {
        // No auton
        ctrler.rumble("--");
    }
}

/*
Middle Mogo Auton
==========================================
MogoClamp down
MovetoPose(left mogo, less accuracy)
    mogoRush down
MovetoPose(back behind knocked rings)
mogoRush unlock
mogorush up
mogoClamp up
Angle 180
MovetoPose(to clamp)
mogoClamp down
intake down
MovetoPose (collect knocked rings)) 
    intake run
MovetoPose(back rings)
MovetoPose(corner rings)

sequence(4)
    MovetoPose(back corner)
    MovetoPose(in corner)
MovetoPose(wall stake almost)
mogoClamp up
arm to ready
MovetoPose(line rings)
MovetoPose(wall stake lineup)
SLAM
MovetoPose(game ready)
end
*/

void redAuton1() {
    // Add red auton 1 code here
    ctrler.rumble(".");

    pros::Task task([&](){ 
        while(1){
            actions.runSubsystemFSM(); 
            delay(10);
        }
    });

    odom::setPos(odom::r_coord(34.875, 35.125, 297.95));
    // actions.setRunAutoMogoClamp(true);
    actions.setRunColorSort(true);

    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    pneumatics.leftMogoRushArm.extend();
    // pneumatics.intakeLock.extend();
    
    // Auton Code: //
    chassis->translateBy(-18, 800, false, 127, 127, .25, 1); 
    pneumatics.leftMogoRushArm.retract();
    chassis->translateBy(18, 7000, false, 127, 127, .675, 1);
    pneumatics.mogoRushTeeth.retract();
    pneumatics.mogoClamp.retract();
    
    // chassis->swingToAngle(280, Drive::DriveSide::LEFT, 2500, false, 127, .5);
    chassis->turnToAngle(280, 1200, false, 127, 1);
    chassis->moveToPose(33, 55, 999, true, 3000, true, 0, 60, 50, .675, 1);
    // chassis->translateBy(-18, 2500, false, 0, 60, .5, .5);
    
    // CLAMPING MOGO AUTO ///

    delay(1000);
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    i_waitUntil(!chassis->isInMotion());

    // Clean knocked rings
    // chassis->translateBy(6, 1500, false, 0, 60);
    actions.setIntakeSpeed(1);
    chassis->swingToAngle(105, Drive::DriveSide::RIGHT, 1500, false);
    // chassis->moveToPose(17, 55, 999, false, 1500, false, 0, 45,45,.5,1);
    chassis->translateBy(7, 1500, false, 20, 40, .5, 1);//fiurst ring
    chassis->translateBy(-9, 1500, false, 20, 40, .5, 1);//fiurst ring
    chassis->translateBy(14, 1500, false, 20, 40, .5, 1);
    chassis->translateBy(-9, 1500, false, 20, 40, .5, 1);//fiurst ring
    delay(1200);
    pneumatics.intakeLift.retract();
    
    // MIDLINE RINGS
    // chassis->translateBy(5, 7000, false, 0, 45, .675, 1);
    chassis->moveToPose(14, 63, 120, false, 1500, false, 0, 45,45,.5,1);
    pneumatics.intakeLift.extend();
    delay(500);
    chassis->translateBy(-10, 1500, false, 0, 45, .5, 1);//fiurst ring
    chassis->translateBy(10, 1500, false, 0, 45, .5, 1);//fiurst ring
    delay(500);
    chassis->swingToAngle(270, Drive::DriveSide::RIGHT, 1500, false);
    chassis->moveToPose(21, 0, 270, false, 6500, false, 0, 55, 45, .675, 1);
    // odom::setPos(odom::r_coord(20, 8, 270));
    chassis->translateBy(-10, 1500, false, 0, 45, .5, 1);//fiurst ring
    chassis->turnToAngle(225, 1200, false, 127, 1);

    // To Corner
    chassis->translateBy(6, 1000, false, 0, 45, .5, 1);//fiurst ring
    odom::r_coord pos = odom::getPos();
    for(int i = 0; i < 3; i++) {
        delay(250);
        chassis->turnToAngle(225, 600, false, 127, 1);
        chassis->translateBy(-15, 1200, false, 45, 60, .5, 1);//fiurst ring
        // chassis->moveToPose(pos.x, pos.y, 225, false, 1500, false, 0, 75, 75, .5, 1);
        chassis->translateBy(-15, 1200, false, 45, 60, .5, 1);//fiurst ring
    }
    delay(500);

    //To Ladder
    chassis->translateBy(-45, 1500, false, 45, 80, .5, 1);//fiurst ring
    chassis->turnToAngle(0, 1200, false, 127, 1);
    chassis->translateBy(30, 1500, false, 45, 80, .5, 1);//fiurst ring  
}


void redAuton2() {
    // Add red auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("..");
}

void blueAuton1() {
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("...");
    pros::Task task([&](){ 
        while(1){
            actions.runSubsystemFSM(); 
            delay(10);
        }
    });

    // Mirror coordinates: (144-34.875, 35.125, (180-297.95)%360)
    odom::setPos(odom::r_coord(109.125, 35.125, 242.05));
    // actions.setRunAutoMogoClamp(true);
    actions.setRunColorSort(true);
    pneumatics.rightMogoRushArm.extend(); // Use right arm for blue side
    pneumatics.mogoRushTeeth.extend();
    // pneumatics.intakeLock.extend();
    
    // Auton Code: //
    chassis->translateBy(-18, 800, false, 127, 127, .25, 1); 
    pneumatics.rightMogoRushArm.retract();
    chassis->translateBy(18, 7000, false, 127, 127, .675, 1);
    pneumatics.mogoRushTeeth.retract();
    pneumatics.mogoClamp.retract();
    
    // Mirror angle: (180-280)%360 = 260
    chassis->turnToAngle(260, 1200, false, 127, 1);
    // Mirror X: (144-33, 55, opposite angle)
    chassis->moveToPose(111, 55, 999, true, 3000, true, 0, 70, 60, .675, 1);
    
    // CLAMPING MOGO AUTO ///
    delay(1000);
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    i_waitUntil(!chassis->isInMotion());

    // Clean knocked rings
    actions.setIntakeSpeed(1);
    // Mirror angle: (180-105)%360 = 75
    chassis->swingToAngle(75, Drive::DriveSide::LEFT, 1500, false);
    chassis->translateBy(7, 1500, false, 20, 40, .5, 1);
    chassis->translateBy(-9, 1500, false, 20, 40, .5, 1);
    chassis->translateBy(12, 1500, false, 20, 40, .5, 1);
    chassis->translateBy(-9, 1500, false, 20, 40, .5, 1);
    delay(1200);
    pneumatics.intakeLift.retract();
    
    // MIDLINE RINGS
    // Mirror X: (144-14, 63, (180-120)%360 = 60)
    chassis->moveToPose(130, 65, 60, false, 1500, false, 0, 45, 45, .5, 1);
    pneumatics.intakeLift.extend();
    delay(500);
    chassis->translateBy(-10, 1500, false, 0, 45, .5, 1);
    chassis->translateBy(10, 1500, false, 0, 45, .5, 1);
    delay(500);
    // Mirror angle: (180-270)%360 = -90%360 = 270
    chassis->swingToAngle(270, Drive::DriveSide::LEFT, 1500, false);
    chassis->translateBy(5, 1500, false, 0, 45, .5, 1);
    chassis->turnToAngle(180, 1200, false, 127, 1);
    chassis->translateBy(48, 1500, false, 0, 45, .5, 1);
    // Mirror X: (144-21, 0, (180-270)%360 = -90%360 = 270)

    /*
    chassis->moveToPose(123, 0, 270, false, 6500, false, 0, 0, 127, .675, 1);
    chassis->translateBy(-10, 1500, false, 0, 45, .5, 1);
    // Mirror angle: (180-225)%360 = -45%360 = 315
    chassis->turnToAngle(315, 1200, false, 127, 1);
    
    // To Corner
    chassis->translateBy(6, 1000, false, 0, 45, .5, 1);
    odom::r_coord pos = odom::getPos();
    for(int i = 0; i < 3; i++) {
        chassis->turnToAngle(315, 120, false, 127, 1);
        chassis->translateBy(12+2*i, 1200, false, 80, 80, .5, 1);
        delay(250);
        // chassis->moveToPose(pos.x, pos.y, 315, true, 1500, false, 0, 60, 60, .675, 1);
        chassis->translateBy(12+2*i, 1200, false, 80, 80, .5, 1);
    }
    delay(500);
    
    //To Ladder
    chassis->translateBy(-45, 1500, false, 45, 80, .5, 1);
    // Mirror angle: (180-0)%360 = 180
    chassis->turnToAngle(180, 1200, false, 127, 1);
    chassis->translateBy(30, 1500, false, 45, 80, .5, 1);
    */
}

void blueAuton2() {
    // Add blue auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("....");
}