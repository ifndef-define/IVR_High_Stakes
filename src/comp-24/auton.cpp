#include "robots/comp-24/auton.h"

void runAuton(bool isBlue) {
    if(isBlue){
        blueAuton();
    } else {
        redAuton();
    }
};

void sift(float dist, int n, int timeout){;
    lemlib::Pose start = chassis.getPose();
    for(int i = 0; i < n; i++){
        chassis.moveToPose(start.x+dist*std::cos(start.theta), start.y+dist*std::sin(start.theta), start.theta, timeout, {}, 0);
        chassis.moveToPose(start.x-dist*std::cos(start.theta), start.y-dist*std::sin(start.theta), start.theta, timeout, {.forwards = 0}, 0);
    }
}

//RED ASSETS
ASSET(comp_white_1_txt);
ASSET(comp_white_2_txt);
ASSET(comp_white_3_txt);
void redAuton(){
    chassis.follow(comp_white_1_txt, 15, 1500);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -50){
            mogoRushReach.extend();
            mogoRushClamp.extend(); // reversed
        }
        delay(15);
    }
    mogoRushClamp.retract();
    chassis.moveToPose(-42.5, 59, 90, 1600, {.forwards = false, 
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

    chassis.tank(0, 0, 0);
    delay(500);

    chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CW_CLOCKWISE, 
                                    .maxSpeed = 127/2});
    chassis.moveToPose(-11, 55, 280, 1600, {.forwards = false, 
                                            .maxSpeed = 127/2, 
                                            .minSpeed = 127/3});
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -20){
            mogoClamp.extend();
        }
        delay(15);
    }

    delay(250);

    chassis.turnToHeading(200, 1000);
    delay(100);
    // intake.setVoltage(127);
    intake.setAutonControlFlag(true);
    
    // delay(500);
    delay(200);
    //chassis.follow(comp_white_3_txt, 5, 10000);
    // chassis.moveToPose(-24, 48, 270, 3000, {.maxSpeed = 127/4});
    // chassis.moveToPose(-46, 48, 270, 3000, {.maxSpeed = 127/4});
    

};

void blueAuton(){
    
};