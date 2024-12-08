#include "robots/comp-15/auton.h"

void runAuton(bool isBlue) {
    if(isBlue){
        blueAuton();
    } else {
        redAuton();
    }
};

//RED ASSETS
ASSET(comp_black_1_txt);
ASSET(comp_black_2_txt);

void redAuton(){
    // move to grab mogo under ladder
    chassis.follow(comp_black_1_txt, 15, 4000);
    // drop mogo rush right as auto starts
    // delay(500);
    mogoRushReach.extend();
    i_waitUntil(!chassis.isInMotion());
    // delay(80);
    mogoRushClamp.retract();
    delay(100);

    // Spin move to move goal to back clamp
    // chassis.turnToHeading(170, 1000, {.maxSpeed=40}, false);
    chassis.swingToHeading(165, lemlib::DriveSide::LEFT, 1200, {.maxSpeed=80}, false);
    mogoRushClamp.extend();
    chassis.moveToPose(chassis.getPose().x, chassis.getPose().y+5, 180, 1000, {.forwards = false}, false);
    mogoRushReach.retract();
    chassis.turnToHeading(350, 1200, {.maxSpeed=90}, false);
    chassis.follow(comp_black_2_txt, 15, 10000, false);
    while(chassis.isInMotion()){
        if(chassis.getPose().y <= -7) {
            mogoClamp.extend();
            break;
        }
        delay(10);
    }
    i_waitUntil(!chassis.isInMotion());
    intake.setVoltage(127);
    chassis.moveToPose(chassis.getPose().x+10, chassis.getPose().y, 87, 1000, {}, false);
    delay(300);
    chassis.moveToPose(chassis.getPose().x-30, chassis.getPose().y, 90, 2000, {.forwards = false}, false);
    
    

    // lcd::print(2, "X: %f, Y: %f, Theta: %f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);


    // chassis.follow(comp_black_1_txt, 15, 1500);
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x >= -50){
    //         mogoRushReach.extend();
    //         // mogoRushClamp.extend(); // reversed
    //     }
    //     delay(15);
    // }
    // mogoRushClamp.retract();
    // chassis.moveToPose(-43, 11, 90, 1600, {.forwards = false, 
    //                                         .maxSpeed = 127, 
    //                                         .minSpeed = 127/1.5});

    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x <= -32){
    //         mogoRushClamp.extend();
    //         delay(100);
    //         mogoRushReach.retract();
    //     }
    //     delay(15);
    // }

    // chassis.tank(0, 0, 0);
    // delay(500);

    // chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CW_CLOCKWISE, 
    //                                 .maxSpeed = 127/2});
    // chassis.moveToPose(-11.5, 7, 280, 1600, {.forwards = false, 
    //                                         .maxSpeed = 127/2, 
    //                                         .minSpeed = 127/3});
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x >= -20){
    //         mogoClamp.extend();
    //     }
    //     delay(15);
    // }

    // delay(250);

    // chassis.turnToHeading(200, 1000);
    // intake.setVoltage(127);
    // delay(500);
    // //chassis.follow(comp_white_3_txt, 5, 10000);
    // chassis.moveToPose(-24, 48, 270, 3000, {.maxSpeed = 127/4});
    // chassis.moveToPose(-48, 48, 270, 3000, {.maxSpeed = 127/4});

};

    // chassis.follow(comp_white_1_txt, 15, 1300);
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x >= -45){
    //         mogoRushReach.extend();
    //         mogoRushClamp.retract();
    //     }
    //     delay(15);
    // }
    // // chassis.turnToHeading(95, 1000);
    // mogoRushClamp.extend();
    // chassis.moveToPose(-40, 60, 85, 1600, {.forwards = false});
    // delay(250);
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x <= -33){
    //         mogoRushClamp.retract();
    //         delay(100);
    //         mogoRushReach.retract();
    //     }
    //     delay(15);
    // }
    // chassis.turnToHeading(270, 1000);
    // chassis.follow(comp_white_2_txt, 15, 2200);

void blueAuton(){
    
};