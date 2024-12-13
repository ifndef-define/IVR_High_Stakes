#include "robots/comp-15/auton.h"

void redAuton();
void blueAuton();

void runAuton(bool isBlue) {
    if(isBlue){
        uint32_t timestart = pros::millis();
        pros::Task blueTask(blueAuton);
        while(pros::millis()-timestart < 30000) {
            delay(10);
        }
        blueTask.remove();
        for (int i=0;i<3;i++) {
            ctrl_master.rumble(".");
            delay(100);
        }
    } else {
        uint32_t timestart = pros::millis();
        pros::Task redTask(redAuton);
        while(pros::millis()-timestart < 30000) {
            delay(10);
        }
        redTask.remove();
        for (int i=0;i<3;i++) {
            ctrl_master.rumble(".");
            delay(100);
        }
    }
};

//RED ASSETS
ASSET(comp_black_1_txt);
ASSET(comp_black_2_txt);
ASSET(comp_black_3_txt);

void redAuton(){
    // drop mogo rush right as auto starts
    mogoRushReach.extend();
    mogoRushClamp.extend();
    // move to grab mogo under ladder
    chassis.follow(comp_black_1_txt, 15, 3000, true);
    intake.setAutonControlFlag(true);
	delay(80);
	intake.setAutonControlFlag(false);
    i_waitUntil(!chassis.isInMotion());
    // clamp goal
    mogoRushClamp.retract();
    // Spin move to move goal to back clamp
    chassis.swingToHeading(170, lemlib::DriveSide::LEFT, 1000, {.maxSpeed=100}, false);
    // let go of goal
    mogoRushClamp.extend();
    // move back away from goal
    delay(100);
    // chassis.moveToPose(chassis.getPose().x, chassis.getPose().y+5, 180, 650, {.forwards = false}, false);
    // pull back mogo rush
    chassis.turnToHeading(349, 1200, {.maxSpeed=100});
    delay(100);
    mogoRushReach.retract();
    i_waitUntil(!chassis.isInMotion());
    // about face
    // move to grab second mogo
    chassis.moveToPose(chassis.getPose().x-2, -55.5, 0, 2500, {.forwards=false, .maxSpeed=127/2});
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
    chassis.swingToHeading(87, lemlib::DriveSide::LEFT, 800, 
        {.direction=lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed=127/2}, false);
    // intake on
    intake.setAutonControlFlag(true);
    // move to stack
    chassis.moveToPose(chassis.getPose().x+10, chassis.getPose().y, 90, 900, {}, false);
    // pull back to next stack
    chassis.moveToPose(chassis.getPose().x-35, chassis.getPose().y, 95, 1500, {.forwards = false}, false);
    // lift intake
    intakeLift.extend();
    // turn to face goal
    chassis.swingToHeading(47, lemlib::DriveSide::LEFT, 1000, {.maxSpeed=127/2}, false);
    // move to stack to grab red ring
    intake.setAutonControlFlag(false);
    chassis.moveToPose(chassis.getPose().x+4.5, chassis.getPose().y+4, 43, 1000, {.maxSpeed=127/2}, false);

    // check when to drop intake
    // i_waitUntil(!chassis.isInMotion());
    // delay to allow ring intake
    intakeLift.retract();
    intake.setAutonControlFlag(true);
    delay(100);
    intake.setAutonControlFlag(false);
    // pull back slighty
    // pros::Task intakeTask([=] {
    //     delay(300);
    //     intake.setVoltage(-127);
    //     delay(300);
    //     intake.setVoltage(127);
    //     delay(300);
    //     intake.setVoltage(127);
    //     delay(300);
    //     intake.setVoltage(0);
    // });

    // intake.setVoltage(-127);
    // delay(450);
    // intake.setVoltage(0);
    chassis.moveToPose(chassis.getPose().x-5, chassis.getPose().y-5, 42, 750, {.maxSpeed=127/2});
    lemlib::Pose start1 = chassis.getPose();
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= start1.x-1) {
            // start intake
            intake.setAutonControlFlag(true);
            break;
        }
        delay(10);
    }
    // turn to face next stack
    chassis.turnToHeading(300, 1000, {.maxSpeed=127/2}, false);
    intakeLift.extend();
    // move to stack, and corner stack
    chassis.moveToPose(chassis.getPose().x-15, chassis.getPose().y+8, 300, 1000, {.maxSpeed=127/2});
    intakeLift.retract();
    chassis.turnToHeading(225, 1000, {.maxSpeed=127/2}, false);
    delay(300);
    chassis.moveToPose(chassis.getPose().x-17, chassis.getPose().y-17, 225, 1000, {.maxSpeed=127/2}, false);
    // chassis.follow(comp_black_3_txt, 12, 4000);
    // chassis.tank(0, 0);
    // delay(1000);
    
    // // intake
    // // chassis.setPose(0,0,225);
    // // intake.setAutonControlFlag(true);
    // // mogoClamp.extend();
    float dist = 3.3;
    int n = 4;
    int timeout = 750;
    lemlib::Pose start2 = chassis.getPose();
    for(int i = 0; i < n; i++) {
        if (i==n-1) {
            start2.x += 2;
            start2.y += 2;     
        }
            chassis.moveToPose(start2.x+(dist*1.414), start2.y+(dist*1.414), 
                225, timeout, {.forwards=false, .maxSpeed=20},0);
    
            delay(400);
        chassis.moveToPose(start2.x-1, start2.y-1, 
            225, timeout, {.maxSpeed=40}, false);

        if (i==n-1) {
            chassis.moveToPose(start2.x+6, start2.y+6, 
                225, timeout, {.forwards = false}, false);
        }
    }

    chassis.turnToHeading(53, 1000, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed=127/2}, false);
    mogoClamp.retract();
    chassis.moveToPose(chassis.getPose().x-10, chassis.getPose().y-10, 53, 1300, {.forwards=false, .minSpeed=50}, false);
    intake.setAutonControlFlag(false);
    chassis.moveToPose(chassis.getPose().x+13, chassis.getPose().y+13, 53, 2000, {}, false);
    chassis.turnToPoint(0, -48, 1200, {.forwards=false, .maxSpeed=(int)ceil(127/1.5)}, false);
    chassis.moveToPose(0, -50, 270, 2000, {.forwards=false, .maxSpeed=127/2}, false);
    chassis.tank(0,0);
    
    // chassis.moveToPose(-54, 10, 180, 2000, {.forwards=false, .maxSpeed=127/1.5}, false);
    // mogoClamp.extend();
    chassis.cancelAllMotions();
    // chassis.moveToPose(chassis.getPose().x+10, chassis.getPose().y+10, 53, 2000, {}, false);
    // chassis.turnToHeading(340, 1000, {.maxSpeed=127/2}, false);
    // chassis.moveToPoint(chassis.getPose().x, -2.5, 3000, {.maxSpeed=127/2});
    // // while(chassis.isInMotion()){
    // //     if(intakeColor.get_proximity() < 50) {
    //         intake.setAutonControlFlag(false);
    // //         break;
    // //     }
    // //     delay(10);
    // // }
    // i_waitUntil(!chassis.isInMotion());
    // // intake.setAutonControlFlag(false);
    // chassis.turnToHeading(90, 1000, {.maxSpeed=127/2});
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().theta >= 85){
    //          intake.setAutonControlFlag(true);   
    //         break;
    //     }
    //     delay(10);
    // }
    // // chassis.turnToPoint(-70, 2, 1000, {.forwards=false, .maxSpeed=(int)ceil(127/1.5)}, false);
    // chassis.moveToPose(chassis.getPose().x-3.5, chassis.getPose().y, 90, 1000, {.forwards=false,.maxSpeed=127/3}, false);
    // delay(3000);
    // intake.setAutonControlFlag(false);
    // chassis.moveToPose(chassis.getPose().x+6, chassis.getPose().y, 90, 1000, {.forwards=false,.maxSpeed=127/3});
    // chassis.turnToHeading(270, 1000, {.maxSpeed=127/3}, false);
    // chassis.moveToPose(chassis.getPose().x-30, chassis.getPose().y, 290, 2000, {.forwards=false,.maxSpeed=127/2});
    // chassis.moveToPose(chassis.getPose().x-5, chassis.getPose().y+50, 340, 2000, {.maxSpeed=127/1.5}, false);

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

//SKILLS ASSETS
ASSET(skills_black_1_txt);
ASSET(skills_black_2_txt);
ASSET(skills_black_3_txt);
ASSET(skills_black_4_txt);

void skillsAuton(){
    chassis.setPose(-52, 0, 90);
    intake.setVoltage(-127);
    delay(250);
    intake.setVoltage(127);
    delay(750);

    chassis.moveToPose(-59, 0, 90, 1200, {.forwards = false, .maxSpeed = 60});

    delay(1500);
    intake.setVoltage(0);

    chassis.moveToPose(-54, 0, 90, 500);
    chassis.turnToHeading(300, 1000);
    chassis.moveToPose(-17, -52, 310, 2800, {.forwards = false, .minSpeed = 45}, false);

    mogoClamp.extend();   
    delay(400);

    intake.setVoltage(127);
    chassis.follow(skills_black_1_txt, 15, 7000, true, true);    

    while(chassis.isInMotion()){ 
        if(chassis.getPose().x >= -1 && chassis.getPose().y >= -15){
            intake.brake();
        } else {
            intake.setVoltage(127);
        }
        delay(10);
    }          
    i_waitUntil(!chassis.isInMotion());
    chassis.follow(skills_black_2_txt, 15, 3200, true, false);

    delay(400);
    chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 225, 2200, {.forwards = false}, false);
    chassis.turnToHeading(45, 1000, {}, false); 
    chassis.moveToPose(-67, -67, 45, 1600, {.forwards = false}); 
    i_waitUntil(!chassis.isInMotion());
    mogoClamp.retract();
    intake.setVoltage(0);   
    i_waitUntil(!chassis.isInMotion());
    chassis.moveToPose(-50, -51.5, 45, 1000);       

    chassis.turnToHeading(230, 1000, {}, false);   
    chassis.moveToPose(35, -25, 230, 3000, {.forwards = false, .minSpeed = 55}, false);
    mogoClamp.extend();   
    delay(200);
    chassis.turnToHeading(210, 1000, {}, false);   
    intake.setAutonControlFlag(true);
    intake.setVoltage(127);

    chassis.moveToPose(6, -60, 210, 3000, {}, false);
    delay(500);
    chassis.turnToHeading(75, 1000, {}, false);  
    chassis.moveToPose(50, -28, 45, 3000, {}, false);
    delay(500);
    chassis.turnToHeading(180, 1000, {}, false);  

    chassis.follow(skills_black_4_txt, 15, 7000, true, false);  

    i_waitUntil(!chassis.isInMotion());
    delay(400);
    chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 135, 2200, {.forwards = false}, false);
    intake.setAutonControlFlag(false);
    intake.setVoltage(127);
    chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 135, 2200, {}, false);
    delay(400);
    chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 135, 2200, {.forwards = false}, false);
    chassis.turnToHeading(315, 1000, {}, false);  
    chassis.moveToPose(67, -67, 315, 1600, {.forwards = false}); 
    i_waitUntil(!chassis.isInMotion());
    mogoClamp.retract();    
    
    chassis.moveToPose(50, -50, 315, 1600); 
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(135, 1000, {}, false); 
    chassis.moveToPose(0, 0, 315, 6000, {.forwards = false, .maxSpeed = 70}); 
    arm.setState(3);
    while(true)
    {
        arm.manualControl();
        delay(10);
    }
};