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
    odom::setPos(odom::r_coord(51.25, 35.25, 297.95));
    pneumatics.leftMogoRushArm.extend();
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble(".");
    // Auton Code: //
    // chassis->moveToPose(0, 0, 332.05, true);
    // while(chassis->inMotion()) {
    //     if(odom::getPos().y > 0) {
    //         // chassis->cancelMotion();
    //         break;
    //     }
    //     pros::delay(10);
    // }
    chassis->translateBy(-17, 800, false, 127, 127, .25, 1); 
    pneumatics.leftMogoRushArm.retract();
    chassis->translateBy(17, 6500, false, 127, 127, .5, 1);
    pneumatics.mogoRushTeeth.retract();
    pneumatics.mogoClamp.retract(); // DO NOT REMOVE
    delay(500);
    // chassis->swingToAngle(280, Drive::DriveSide::LEFT, 2500, false, 127, .5);
    chassis->turnToAngle(280, 2500, false, 127, 1);
    chassis->translateBy(-24, 2500, true);
    delay(650);
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    delay(650);
    chassis->turnToAngle(165, 2500, false, 127, 1);
    pneumatics.intakeLift.extend();
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