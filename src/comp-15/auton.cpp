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
ASSET(comp_black_3_txt);

void redAuton(){
    intakeLift.retract();
    // drop mogo rush right as auto starts
    mogoRushReach.extend();
    mogoRushClamp.extend();
    // move to grab mogo under ladder
    chassis.follow(comp_black_1_txt, 15, 4000, true, false);
    // clamp goal
    mogoRushClamp.retract();
    // Spin move to move goal to back clamp
    chassis.swingToHeading(170, lemlib::DriveSide::LEFT, 1200, {.maxSpeed=80}, false);
    // let go of goal
    mogoRushClamp.extend();
    // move back away from goal
    chassis.moveToPose(chassis.getPose().x, chassis.getPose().y+5, 180, 1000, {.forwards = false}, false);
    // pull back mogo rush
    mogoRushReach.retract();
    // about face
    chassis.turnToHeading(349, 1200, {.maxSpeed=90}, false);
    // move to grab second mogo
    chassis.moveToPose(chassis.getPose().x+1, -54, 0, 3500, {.forwards=false, .maxSpeed=127/1.5});
    mogoRushClamp.extend();
    // allow mogo to slide into clamp
    while(chassis.isInMotion()){
        if(chassis.getPose().y <= -27) {
            mogoClamp.extend();
            break;
        }
        delay(10);
    }
    // wait till at back wall
    i_waitUntil(!chassis.isInMotion());
    // swing to face two stack on auto line
    chassis.swingToHeading(87, lemlib::DriveSide::LEFT, 1500, 
        {.direction=lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed=127/2}, false);
    // intake on
    intake.setAutonControlFlag(true);
    // move to stack
    chassis.moveToPose(chassis.getPose().x+10, chassis.getPose().y, 90, 1000, {}, false);
    // pull back to next stack
    chassis.moveToPose(chassis.getPose().x-33, chassis.getPose().y, 95, 2000, {.forwards = false}, false);
    // lift intake
    intakeLift.extend();
    // turn to face goal
    chassis.swingToHeading(48, lemlib::DriveSide::LEFT, 1500, {.maxSpeed=127/2}, false);
    // move to stack to grab red ring
    chassis.moveToPose(chassis.getPose().x+4, chassis.getPose().y+4, 48, 2000, {.maxSpeed=127/4});

    // check when to drop intake
    lemlib::Pose start = chassis.getPose();
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= start.x+2) {
            // drop intake
            intakeLift.retract();
            break;
        }
        delay(10);
    }
    i_waitUntil(!chassis.isInMotion());
    // delay to allow ring intake
    delay(150);
    intake.setAutonControlFlag(false);
    // pull back slighty
    chassis.moveToPose(chassis.getPose().x-5, chassis.getPose().y-5, 45, 1000, {.maxSpeed=127/2});
    lemlib::Pose start1 = chassis.getPose();
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= start1.x-2) {
            // start intake
            intake.setAutonControlFlag(true);
            break;
        }
        delay(10);
    }
    // turn to face next stack
    chassis.turnToHeading(300, 2000, {.maxSpeed=127/2}, false);
    intakeLift.extend();
    // move to stack, and corner stack
    chassis.moveToPose(chassis.getPose().x-14, chassis.getPose().y+7, 300, 2000, {.maxSpeed=127/3});
    intakeLift.retract();
    delay(1000);
    chassis.turnToHeading(225, 1500, {.maxSpeed=127/2}, false);
    chassis.moveToPose(chassis.getPose().x-17, chassis.getPose().y-17, 225, 2000, {.maxSpeed=127/3, .minSpeed=127/6}, false);
    // chassis.follow(comp_black_3_txt, 12, 4000);
    // chassis.tank(0, 0);
    // delay(1000);
    
    // // intake
    // // chassis.setPose(0,0,225);
    // // intake.setAutonControlFlag(true);
    // // mogoClamp.extend();
    float dist = 5;
    int n = 3;
    int timeout = 1500;
    lemlib::Pose start2 = chassis.getPose();
    for(int i = 0; i < n; i++) {
        chassis.moveToPose(start2.x+(dist*1.414), start2.y+(dist*1.414), 
            225, timeout, {.forwards=false, .maxSpeed=30}, false);
        if( i==0 ) {
            delay(100);
            intakeLift.extend();
        }

        chassis.moveToPose(start2.x, start2.y, 
            225, timeout, {.maxSpeed=60}, false);

        if (i==1) {
            delay(100);
            intakeLift.retract();
        }
        
        if (i==2) {
            delay(50);
            intake.setAutonControlFlag(false);
            chassis.moveToPose(start2.x+4, start2.y+4, 
                225, timeout, {.forwards = false}, false);
        }
    }

    // chassis.moveToPose(chassis.getPose().x+10, chassis.getPose().y+8, 45, 2000, {.maxSpeed=127/4}, false);
    // i_waitUntil(!chassis.isInMotion());
    // chassis.moveToPose(chassis.getPose().x-24, chassis.getPose().y, 270, 3000, {.maxSpeed=127/3}, false);
    // chassis.turnToHeading(225, 1500, {.maxSpeed=127/2}, false);
    // chassis.moveToPose(chassis.getPose().x-25, chassis.getPose().y-25, 230, 3000, {.maxSpeed=127/3}, false);

    // lcd::print(2, "X: %f, Y: %f, Theta: %f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);

    // intake.setAutonControlFlag(false);
    

};

void blueAuton(){
    
};