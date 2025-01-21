#include "robots/comp-15/auton.h"

// void redAuton();
// void blueAuton();

// // Timing Test Control
// void runAuton(bool isBlue) {
//     if(isBlue){
//         uint32_t timestart = pros::millis();
//         pros::Task blueTask(blueAuton);
//         while(pros::millis()-timestart < 30000) {
//             delay(10);
//         }
//         blueTask.remove();
//         for (int i=0;i<3;i++) {
//             ctrl_master.rumble(".");
//             delay(100);
//         }
//     } else {
//         uint32_t timestart = pros::millis();
//         pros::Task redTask(redAuton);
//         while(pros::millis()-timestart < 30000) {
//             delay(10);
//         }
//         redTask.remove();
//         for (int i=0;i<3;i++) {
//             ctrl_master.rumble(".");
//             delay(100);
//         }
//     }
// };

// RED AUTO ASSETS
ASSET(comp_black_1_txt);
ASSET(comp_black_4_txt);

void redAuton(){
    chassis.setPose(-54, 13, 90);

    // drop mogo rush right as auto starts
    mogoRushReach.extend();
    mogoRushClamp.extend();
    // move to grab mogo under ladder
    chassis.follow(comp_black_1_txt, 15, 3000, true);
    // wait
    i_waitUntil(!chassis.isInMotion());
    // clamp goal
    mogoRushClamp.retract();
    // Spin move to move goal to back clamp
    chassis.swingToHeading(170, lemlib::DriveSide::LEFT, 1000, {.maxSpeed=100}, false);
    // let go of goal
    mogoRushClamp.extend();
    delay(30);
    // about face
    chassis.turnToHeading(349, 1200, {.maxSpeed=100});
    delay(100);
    mogoRushReach.retract();
    i_waitUntil(!chassis.isInMotion());
    mogoRushClamp.extend();
    // move to grab second mogo
    chassis.moveToPose(chassis.getPose().x-2, -55.5, 0, 2500, {.forwards=false, .maxSpeed=127/2});
    // allow mogo to slide into clamp
    while(chassis.isInMotion()){
        if(chassis.getPose().y <= -27) {
            mogoClamp.extend();
            // release intake
            intake.releaseIntake();
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

    // drop intake on red top ring
    intakeLift.retract();
    
    // lightly intake
    intake.setAutonControlFlag(true);
    delay(300);
    intake.setAutonControlFlag(false);
    // pull back slighty
    chassis.moveToPose(chassis.getPose().x-5, chassis.getPose().y-5, 42, 750, {.maxSpeed=127/2});
    delay(100);
    // start intake
    intake.setAutonControlFlag(true);
    // turn to knock ring out
    chassis.turnToHeading(300, 1000, {.maxSpeed=127/2}, false);
    // move to stack, and corner stack
    chassis.moveToPose(chassis.getPose().x-15, chassis.getPose().y+8, 300, 1000, {.maxSpeed=127/2});
    // turn to corner stack
    chassis.turnToHeading(225, 1000, {.maxSpeed=127/2}, false);
    delay(300);
    // move into stack
    chassis.moveToPose(chassis.getPose().x-17, chassis.getPose().y-17, 225, 1000, {.maxSpeed=127/2}, false);

    // cycle through the ring stack
    float dist = 3.3;
    int n = 3;
    int timeout = 750;
    lemlib::Pose start2 = chassis.getPose();
    for(int i = 0; i < n; i++) {
        // last loop pull out more
        if (i==n-1) {
            chassis.moveToPose(start2.x+(dist*1.414)+1, start2.y+(dist*1.414)+1, 
                225, timeout, {.forwards=false, .maxSpeed=20},SYNC);
        }
        else if (i==0) { // first loop pull out less
            chassis.moveToPose(start2.x+(dist*1.414)-.5, start2.y+(dist*1.414)-.5, 
                225, timeout, {.forwards=false, .maxSpeed=20},SYNC);
        } 
    
        // let intake to clean
        delay(400);

        // move forward
        chassis.moveToPose(start2.x-1, start2.y-1, 
            225, timeout, {.maxSpeed=40}, false);

        // last loop move back to goal
        if (i==n-1) {
            chassis.moveToPose(start2.x+7, start2.y+7, 
                225, timeout, {.forwards = false}, false);
        }
    }
    chassis.turnToHeading(90, 1000, {.maxSpeed=(int)ceil(127/1.5)}, false);
    intake.setAutonControlFlag(false);
    intakeLift.extend();
    // Go to ladder
    chassis.moveToPose(-26, chassis.getPose().y, 90, 2000, {}, false);
    // chassis.turnToHeading(0, 1000, {.maxSpeed=127/2}, false);
    chassis.moveToPoint(-22, -15, 3000, {.maxSpeed=127/2}, false);
    doinker.extend();       
    chassis.tank(30,0);
    delay(1500);
    chassis.cancelAllMotions();
    chassis.tank(0,0);

    // // about face
    // chassis.turnToHeading(53, 1000, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed=127/2}, false);
    // // drop mogo
    // mogoClamp.retract();
    // // push into corner
    // chassis.moveToPose(chassis.getPose().x-10, chassis.getPose().y-10, 53, 1300, {.forwards=false, .minSpeed=50}, false);
    // // stop intake
    // intake.setAutonControlFlag(false);
    // // move to bottom of high stake
    // chassis.moveToPose(chassis.getPose().x+13, chassis.getPose().y+13, 53, 2000, {}, false);
    // chassis.turnToPoint(0, -24, 1000, {.maxSpeed=(int)ceil(127/1.5)}, false);
    // // use intake up to prevent breaking bot
    // intakeLift.extend();
    // // move to ladder
    // chassis.moveToPose(0, -20, 45, 2000, {}, false);
    // // slow turn to ensure contact
    // chassis.tank(10,0);
    // // stop all motion
    // chassis.cancelAllMotions();

};

ASSET(comp_black_1_blue_txt);
void blueAuton() {
    chassis.setPose(54, -13, 270);

        // drop mogo rush right as auto starts
    mogoRushReach.extend();
    mogoRushClamp.extend();
    // move to grab mogo under ladder
    chassis.follow(comp_black_1_blue_txt, 15, 3000, true);
    // wait
    i_waitUntil(!chassis.isInMotion());
    // clamp goal
    mogoRushClamp.retract();
    // Spin move to move goal to back clamp
    chassis.swingToHeading(350, lemlib::DriveSide::LEFT, 1000, {.maxSpeed=100}, false);
    // let go of goal
    mogoRushClamp.extend();
    delay(30);
    // about face
    chassis.turnToHeading(169, 1200, {.direction=lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed=100});
    delay(100);
    mogoRushReach.retract();
    i_waitUntil(!chassis.isInMotion());
    // move to grab second mogo
    chassis.moveToPose(chassis.getPose().x-2, 24, 180, 2500, {.forwards=false, .maxSpeed=127/2});
    // allow mogo to slide into clamp
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= 20) {
            mogoClamp.extend();
            // release intake
            intake.releaseIntake();
            break;
        }
        delay(10);
    }
    // wait till mogo clamp
    i_waitUntil(!chassis.isInMotion());
    // another about face for mirror difference
    chassis.turnToHeading(358, 1000, {.direction=lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed=127/2});
    // continue to rings
    chassis.moveToPose(chassis.getPose().x-2, -55.5, 0, 3500, {.forwards=false, .maxSpeed=127/2});
    // swing to face two stack on auto line
    chassis.swingToHeading(273, lemlib::DriveSide::RIGHT, 800, 
        {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed=127/2}, false);
    // intake on
    intake.setAutonControlFlag(true);
    // the ring we want is on top this top
    intakeLift.extend();
    // move to stack
    chassis.moveToPose(chassis.getPose().x-10, chassis.getPose().y, 270, 900, {}, false);
    // pull back to next stack
    chassis.moveToPose(chassis.getPose().x+35, chassis.getPose().y, 265, 1500, {.forwards = false}, false);
    // lift intake
    intakeLift.extend();
    // turn to face goal
    chassis.swingToHeading(313, lemlib::DriveSide::RIGHT, 1000, {.maxSpeed=127/2}, false);
    // move to stack to grab red ring
    intake.setAutonControlFlag(false);
    chassis.moveToPose(chassis.getPose().x-4.5, chassis.getPose().y+4, 313, 1000, {.maxSpeed=127/2}, false);

    // drop intake on red top ring
    intakeLift.retract();
    
    // lightly intake
    intake.setAutonControlFlag(true);
    delay(300);
    intake.setAutonControlFlag(false);
    // pull back slighty
    chassis.moveToPose(chassis.getPose().x+5, chassis.getPose().y-5, 42, 750, {.maxSpeed=127/2});
    delay(100);
    // start intake
    intake.setAutonControlFlag(true);
    // turn to knock ring out
    chassis.turnToHeading(70, 1000, {.maxSpeed=127/2}, false);
    // move to stack, and corner stack
    chassis.moveToPose(chassis.getPose().x+15, chassis.getPose().y+8, 70, 1000, {.maxSpeed=127/2});
    // turn to corner stack
    chassis.turnToHeading(135, 1000, {.maxSpeed=127/2}, false);
    delay(300);
    // move into stack
    chassis.moveToPose(chassis.getPose().x+17, chassis.getPose().y-17, 225, 1000, {.maxSpeed=127/2}, false);

    // cycle through the ring stack
    float dist = 3.3;
    int n = 3;
    int timeout = 750;
    lemlib::Pose start2 = chassis.getPose();
    for(int i = 0; i < n; i++) {
        // last loop pull out more
        if (i==n-1) {
            chassis.moveToPose(start2.x+(dist*1.414)-1, start2.y+(dist*1.414)+1, 
                225, timeout, {.forwards=false, .maxSpeed=20},SYNC);
        }
        else if (i==0) { // first loop pull out less
            chassis.moveToPose(start2.x+(dist*1.414)-.5, start2.y+(dist*1.414)-.5, 
                225, timeout, {.forwards=false, .maxSpeed=20},SYNC);
        } 
    
        // let intake to clean
        delay(400);

        // move forward
        chassis.moveToPose(start2.x+1, start2.y-1, 
            135, timeout, {.maxSpeed=40}, false);

        // last loop move back to goal
        if (i==n-1) {
            chassis.moveToPose(start2.x-7, start2.y+7, 
                135, timeout, {.forwards = false}, false);
        }
    }
    chassis.turnToHeading(0, 1000, {.maxSpeed=(int)ceil(127/1.5)}, false);
    intake.setAutonControlFlag(false);
    intakeLift.extend();
    // Go to ladder
    chassis.moveToPose(26, chassis.getPose().y, 90, 2000, {}, false);
    // chassis.turnToHeading(0, 1000, {.maxSpeed=127/2}, false);
    chassis.moveToPoint(22, -15, 3000, {.maxSpeed=127/2}, false);
    doinker.extend();       
    chassis.tank(0,30);
    delay(1500);
    chassis.cancelAllMotions();
    chassis.tank(0,0);

};

//SKILLS ASSETS
ASSET(skills_black_1_txt);
//ASSET(skills_black_2_txt);
ASSET(skills_black_3_txt);
ASSET(skills_black_4_txt);

void skillsAuton(){

    uint32_t timeStart = pros::millis();

    
    chassis.setPose(-52, 0, 90);
    arm.setPosition(0);
    intake.setVoltage(-127);
    delay(250);
    intake.setVoltage(127);
    delay(600);

    chassis.moveToPose(-58.5, 0, 90, 1200, {.forwards = false, .maxSpeed = 60});
    // while(chassis.isInMotion())
    // {
    //     if(intakeColor.get_proximity() >= 170)
    //     {
    //         intake.brake();
    //     }
    //     else
    //     {
    //         intake.setVoltage(127);
    //     }
    // }
    // i_waitUntil(!chassis.isInMotion());
    // intake.setAutonControlFlag(true);
    delay(1500); //tune
    // intake.setAutonControlFlag(false);
    intake.setVoltage(0);

    chassis.moveToPose(-54, 0, 90, 500);
    chassis.turnToHeading(300, 1000);
    chassis.moveToPose(-20, -51, 310, 2800, {.forwards = false, .maxSpeed = 127/2}, false);

    mogoClamp.extend();   
    delay(400);

    intake.setVoltage(127);
    chassis.follow(skills_black_1_txt, 15, 8000, true, true);    

    while(chassis.isInMotion()){ 
        if(chassis.getPose().x >= 0 && chassis.getPose().y >= -27){
            intake.brake();
        } 
        else if(chassis.getPose().x >= -20 && chassis.getPose().x <= -10){
            intake.brake();
        }
        else {
            intake.setVoltage(127);
        }
        delay(10);
    }          
    
    chassis.moveToPose(-63, -66.267, 235, 3000, {.maxSpeed = 127/2}, false);

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
    chassis.moveToPose(38, -22, 235, 3000, {.forwards = false, .maxSpeed = 127/2}, false);
    mogoClamp.extend();
    // while(chassis.isInMotion())
    // {
    //     if(chassis.getPose().x > 35)
    //     {
    //         mogoClamp.extend(); 
    //     }
    //     delay(10);
    // }
      
    delay(600);
    chassis.turnToHeading(210, 1000, {.maxSpeed=127/2}, false);   
    intake.setAutonControlFlag(true);

    chassis.moveToPose(4, -56, 210, 3000, {}, false);
    delay(500);
    chassis.turnToHeading(75, 1000, {.maxSpeed = 60}, false);  
    chassis.moveToPose(50, -28, 45, 3000, {.maxSpeed = 60}, false);
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
    chassis.turnToHeading(315, 2500, {.direction = lemlib::AngularDirection::CW_CLOCKWISE}, false);  
    chassis.moveToPose(67, -67, 315, 1600, {.forwards = false}); 
    i_waitUntil(!chassis.isInMotion());
    mogoClamp.retract();    
    intake.setVoltage(0);
    
    chassis.moveToPose(50, -50, 315, 1600); 
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(135, 1000, {}, false); 
    chassis.moveToPose(0, 0, 315, 6000, {.forwards = false, .maxSpeed = 70}); 
    // arm.setState(3);
    while(true)
    {
        // arm.manualControl();
        arm.setPosition(140);

        if((pros::millis() - timeStart) > 59000)
        {
            arm.setPosition(0);
            // arm.manualControl();
        }

        delay(10);
    }
};