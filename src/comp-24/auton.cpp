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
    chassis.moveToPose(-42.5, 61, 90, 1600, {.forwards = false, 
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

    //chassis.tank(0, 0, 1);
    delay(250);
    
    chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CW_CLOCKWISE, 
                                    .maxSpeed = 127/2});
    chassis.moveToPose(-15, 56, 280, 1600, {.forwards = false, 
                                            .maxSpeed = 127/2, 
                                            .minSpeed = 127/3});
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -23){
            mogoClamp.extend();
            intake.setVoltage(127);
        }
        delay(15);
    }

    delay(500);

    chassis.turnToHeading(200, 1000);
    //intake.setVoltage(0);
};

void blueAuton(){
    
};