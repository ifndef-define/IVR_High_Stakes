#include "robots/comp-15/auton.h"
#include "string.h"
#include <filesystem>

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
    chassis.follow(comp_white_1_txt, 15, 1500);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -45){
            mogoRushReach.extend();
            mogoRushClamp.extend(); // reversed
        }
        delay(15);
    }
    // chassis.turnToHeading(95, 1000);
    mogoRushClamp.retract();
    chassis.moveToPose(-41, 59, 95, 1600, {.forwards = false, 
                                            .maxSpeed = 127, 
                                            .minSpeed = 127/1.5});

    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -32){
            mogoRushClamp.extend();
            delay(100);
            mogoRushReach.retract();
        }
        delay(15);
    }
    chassis.tank(0, 0, 1);
    
    // chassis.turnToHeading(270, 1400);
    // chassis.follow(comp_white_2_txt, 15, 2200, 0);
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().y <= 47){
    //         mogoClamp.extend();
    //         intake.setVoltage(127);
    //     }
    //     delay(15);
    // }
    // intake.setVoltage(0);
};

void blueAuton(){
    
};