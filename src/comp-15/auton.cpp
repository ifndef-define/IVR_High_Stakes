#include "robots/comp-15/auton.h"

void runAuton(bool isBlue) {
    if(isBlue){
        blueAuton();
    } else {
        redAuton();
    }
};

//RED ASSETS
ASSET(comp_white_1_txt);
void redAuton(){
    chassis.follow(comp_white_1_txt, 15, 1500);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -45){
            mogoRushReach.extend();
        }
        delay(15);
    }
    delay(300);
    mogoRushClamp.extend();
};

void blueAuton(){
    
};