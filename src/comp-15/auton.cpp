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
ASSET(comp_white_2_txt);
void redAuton(){
    chassis.follow(comp_white_1_txt, 15, 1300);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -45){
            mogoRushReach.extend();
            mogoRushClamp.retract();
        }
        delay(15);
    }
    // chassis.turnToHeading(95, 1000);
    mogoRushClamp.extend();
    chassis.moveToPose(-40, 60, 85, 1600, {.forwards = false});
    delay(250);
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -33){
            mogoRushClamp.retract();
            delay(100);
            mogoRushReach.retract();
        }
        delay(15);
    }
    chassis.turnToHeading(270, 1000);
    chassis.follow(comp_white_2_txt, 15, 2200);
};

void blueAuton(){
    
};