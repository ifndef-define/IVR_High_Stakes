#include "robots/comp-15/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
    actions.setRingColor(ringToKeep);
    if(ringToKeep == Ring::Color::RED) {
        redAuton();
    }else if(ringToKeep == Ring::Color::BLUE) {
        blueAuton();
    }
}

void redAuton(){
    
}

void blueAuton() {
   
}