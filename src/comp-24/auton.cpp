#include "robots/comp-24/auton.h"
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
// ASSET(comp_white_1_txt);
// ASSET(comp_white_2_txt);
void redAuton(){
    // chassis.follow(comp_white_1_txt, 15, 1500);
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

//SKILLS ASSETS
ASSET(skills_white_1_txt);
ASSET(skills_white_3_txt);
ASSET(skills_white_4_txt);
ASSET(skills_white_5_txt);

void skillsCaller() {
    uint32_t timeStart = pros::millis();
    pros::Task autotask(skillsAuton);
    while(pros::millis()-timeStart < 60000)
        delay(10);
    
    autotask.remove();
    ctrl_master.rumble("---");
}

void skillsAuton(){

    uint32_t timeStart = pros::millis();
    chassis.setPose(-52, 27, 90);
    arm.setPosition(0);

    chassis.turnToHeading(200, 1000, {.maxSpeed = 127/2}, false);
    chassis.moveToPose(-19.5, 51.5, 240, 2400, {.forwards = false, .minSpeed = 45}, false);

    mogoClamp.extend();
    delay(250);
    chassis.turnToHeading(200, 1000, {}, true);

    intake.setVoltage(127);
    chassis.follow(skills_white_1_txt, 15, 7000, true, true);    

    while(chassis.isInMotion()){ 
        if(chassis.getPose().x >= -1 && chassis.getPose().y <= 21){
            intake.brake();
        } 
        else if(chassis.getPose().x >= -19 && chassis.getPose().x <= -12){
            intake.brake();
        }
        else {
            intake.setVoltage(127);
        }
        delay(10);
    }
   
    chassis.moveToPose(-66, 78, 320, 3300, {.maxSpeed = 127/2}, false);
    
    delay(800);
    intake.setVoltage(100);
    chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 320, 1200, {.forwards = false}, false);
    chassis.turnToHeading(135, 1000, {}, false); 
    chassis.moveToPose(-58, 72, 135, 1000, {.forwards = false});
    i_waitUntil(!chassis.isInMotion());
    mogoClamp.retract();
    intake.setVoltage(0);   
    i_waitUntil(!chassis.isInMotion());
    chassis.moveToPose(-48, 54.5, 135, 1000);       

    chassis.turnToHeading(230, 1000, {}, false);   
    chassis.moveToPose(35, 30, 310, 3000, {.forwards = false}, false);
    mogoClamp.extend();   
    delay(200); 
    intake.setAutonControlFlag(true);

    chassis.moveToPose(10, 64, 330, 3000, {}, false);
    delay(500);
    chassis.turnToHeading(90, 1000, {.maxSpeed = 60}, false);  
    chassis.moveToPose(56, 33, 135, 2000, {}, false);
    delay(500);
    chassis.turnToHeading(0, 1000, {}, false);  

    // chassis.moveToPose(53, 47, 20, 3000, {}, false);
    // delay(300);
    // chassis.moveToPose(56, 33, 0, 2200, {.forwards = false}, false);
    chassis.follow(skills_white_4_txt, 15, 4000, true, false); 

    i_waitUntil(!chassis.isInMotion());
    delay(400);
    chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 45, 2200, {.forwards = false}, false);
    intake.setAutonControlFlag(false);
    intake.setVoltage(127);
    chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 45, 2200, {}, false);
    delay(400);
    chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 45, 2200, {.forwards = false}, false);
    chassis.turnToHeading(225, 1000, {}, false);  
    chassis.moveToPose(72, 76, 225, 1600, {.forwards = false}); 
    i_waitUntil(!chassis.isInMotion());
    mogoClamp.retract();    
    
    chassis.moveToPose(56, 50, 225, 1600); 
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(0, 800, {}, false); 
    intake.setAutonControlFlag(true);

    chassis.moveToPose(56, -20, 0, 1600, {.forwards = false}, false); 
    i_waitUntil(!chassis.isInMotion());
    mogoClamp.extend();  
    delay(200);
    chassis.turnToHeading(45, 1000, {}, false);  
    intake.setAutonControlFlag(false);
    intake.setVoltage(127);
    chassis.moveToPose(chassis.getPose().x + 5*sqrt(2), chassis.getPose().y + 5*sqrt(2), 45, 1000, {}, false);
    delay(400);
    chassis.moveToPose(chassis.getPose().x - 5*sqrt(2), chassis.getPose().y - 5*sqrt(2), 45, 1000, {.forwards = false}, false);
    chassis.moveToPose(chassis.getPose().x + 10*sqrt(2), chassis.getPose().y + 10*sqrt(2), 45, 1000, {}, false);
    chassis.moveToPose(chassis.getPose().x - 10*sqrt(2), chassis.getPose().y - 10*sqrt(2), 45, 1000, {.forwards = false}, false);

    chassis.follow(skills_white_5_txt, 15, 8000, true, true);    
    while(chassis.isInMotion()){ 
        if(chassis.getPose().x >= 3 && chassis.getPose().y <= 50){
            mogoClamp.retract();
            intake.setVoltage(0);
        } 

        delay(10);
    }          
    i_waitUntil(!chassis.isInMotion());

    chassis.turnToHeading(0, 1000, {}, false); 
    chassis.moveToPose(0, 0, 45, 6000, {.forwards = false, .minSpeed = 70}); 
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