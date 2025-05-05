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