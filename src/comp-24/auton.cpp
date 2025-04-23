#include "robots/comp-24/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
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