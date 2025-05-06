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
Open Mogo Auton
==========================================
- MogoClamp down
- translate by to the middle mogo (async)
- mogoRush down
- wait til translate by is done
- translate back to ladder ring (async)
- release goal by ladder
- mogoRush up
- wait for translate to finish
- back up to goal
- autoclamp goal
- intake on
- spin to outer ring stack
- intake lift
- translate to outer ring stack (async)
- wait for translate to finish
- drop intake down
- translate a bit back
- move to inner ring stack
- turn to corner
- translate to corner (async)
- intake on
- cycle through rings
- back up to inner ring stack
- swing to neutral stack
- translate to neutral stack (async)
- intake on
- wait for translate to finish
- translat a bit back
- turn to face ladder
- translate to ladder (async) 
- intake off
- arm to ready
- end of auton
*/
void redAuton1() {
    // Add red auton 1 code here
    pneumatics.mogoClamp.extend(); // DO NOT REMOVE
    ctrler.rumble(".");
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