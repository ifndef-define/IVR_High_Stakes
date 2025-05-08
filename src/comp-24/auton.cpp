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
    pneumatics.intakeLock.extend();
    
    // Auton Code: //
    chassis->translateBy(-17, 800, false, 127, 127, .25, 1); 
    pneumatics.leftMogoRushArm.retract();
    chassis->translateBy(17, 7000, false, 127, 127, .675, 1);
    pneumatics.mogoRushTeeth.retract();
    pneumatics.mogoClamp.retract();
    
    // chassis->swingToAngle(280, Drive::DriveSide::LEFT, 2500, false, 127, .5);
    chassis->turnToAngle(280, 1200, false, 127, 1);
    chassis->moveToPose(33, 53, 999, true, 2500, true, 0, 60, 50, .675, 1);
    // chassis->translateBy(-18, 2500, false, 0, 60, .5, .5);
    
    // CLAMPING MOGO AUTO ///

    delay(800);
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    i_waitUntil(!chassis->isInMotion());

    // Clean knocked rings
    // chassis->translateBy(6, 1500, false, 0, 60);
    actions.setIntakeSpeed(1);
    chassis->swingToAngle(105, Drive::DriveSide::RIGHT, 1500, false);
    // chassis->moveToPose(17, 55, 999, false, 1500, false, 0, 45,45,.5,1);
    chassis->translateBy(7, 1500, false, 20, 40, .5, 1);//fiurst ring
    chassis->translateBy(-9, 1500, false, 20, 40, .5, 1);//fiurst ring
    chassis->translateBy(13, 1500, false, 20, 40, .5, 1);
    delay(1000);
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
    chassis->moveToPose(21, 12, 270, false, 4000, false, 0, 60, 50, .675, 1);
    chassis->turnToAngle(225, 1200, false, 127, 1);
    chassis->translateBy(17, 1500, false, 0, 45, .5, 1);//fiurst ring
    for(int i = 0; i < 3; i++) {
        delay(250);
        chassis->translateBy(-7, 1000, false, 45, 60, .5, 1);//fiurst ring
        chassis->translateBy(7, 1000, false, 45, 60, .5, 1);//fiurst ring
    }
    delay(500);
    chassis->translateBy(-45, 1500, false, 45, 80, .5, 1);//fiurst ring
    chassis->turnToAngle(0, 1200, false, 127, 1);
    chassis->translateBy(30, 1500, false, 45, 80, .5, 1);//fiurst ring



    // //To BackRings
    // chassis->translateBy(-12, 800, false);
    // chassis->turnToPoint(22, 22, 2500, false, 127, 1);
    // chassis->moveToPose(16, 20, 999, false, 2500, false, 0, 60, 60, .5, 1);

    /*
    // Sift Corner Rings
    chassis->turnToAngle(225, 1200, false, 127, 1);
    for(int i = 0; i < 4; i++) {
        chassis->moveToPose(10, 10, 999, false, 1500, false, 0, 60, 60, .5, 1);
        chassis->turnToAngle(225, 500, false, 127, 1);
        chassis->translateBy(10, 1500, false, 0, 127, .5, 1);
    }
    */

    //Alternative Sift Corner
    // chassis->turnToAngle(225, 1200, false, 127, 1);
    // chassis->moveToPose(12, 12, 999, false, 1500, false, 0, 60, 60, .5, 1);
    // pneumatics.intakeLift.retract();
    // chassis->translateBy(10, 1500, false, 0, 127, .5, 1);

    
}


void redAuton2() {
    // Add red auton 2 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble("..");
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