#include "robots/comp-24/auton.h"

void auton(Ring::Color ringToKeep) {
    actions.setRingColor(ringToKeep);
    //Add auton code here
    if(ringToKeep == Ring::Color::RED) {
        redAuton();
    } else {
        blueAuton();
    }
}

void redAuton(){
    
}

void blueAuton(){
    
}