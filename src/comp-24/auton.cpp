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
        if(chassis.getPose().x >= -50){
            mogoRushReach.extend();
            mogoRushClamp.extend(); // reversed
        }
        delay(15);
    }
    // chassis.turnToHeading(95, 1000);
    mogoRushClamp.retract();
    chassis.moveToPose(-43, 60, 85, 1600, {.forwards = false, 
                                            .maxSpeed = 127, 
                                            .minSpeed = 127/1.5});

    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -28){
            
        }
        delay(15);
    }
    mogoRushClamp.extend();
    delay(100);
    mogoRushReach.retract();
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