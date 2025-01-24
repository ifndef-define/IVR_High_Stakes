#include "robots/comp-24/auton.h"
#include "string.h"
#include <filesystem>

void runAuton(bool isBlue) {
    if(isBlue){
        blueAuton();
    }
    else
    {
        redAuton();
    }
};

// void skillsCaller() {
//     uint32_t timeStart = pros::millis();
//     pros::Task autotask(redAuton);
//     while(pros::millis()-timeStart < 30000)
//         delay(10);
    
//     autotask.remove();
//     ctrl_master.rumble("---");
// }

void moveFwd(float dist, int timeout, float maxSpeed, float minSpeed)
{
    lemlib::Pose start = chassis.getPose();
    float thetaRad = start.theta * M_PI / 180;
    chassis.moveToPose(start.x - dist * std::cos(thetaRad), start.y - dist * std::sin(thetaRad), start.theta, timeout, {.maxSpeed = maxSpeed, .minSpeed = minSpeed});
}

void moveBwd(float dist, int timeout, float maxSpeed, float minSpeed)
{
    lemlib::Pose start = chassis.getPose();
    float thetaRad = start.theta * M_PI / 180;
    chassis.moveToPose(start.x + dist * std::cos(thetaRad), start.y + dist * std::sin(thetaRad), start.theta, timeout, {.forwards = 0, .maxSpeed = maxSpeed, .minSpeed = minSpeed});
}

float convertAngle(float angle) {
    return fmod((360 + fmod(angle,360)),360);
}

// RED ASSETS
ASSET(comp_white_1_txt);
ASSET(comp_white_2_txt);
ASSET(comp_white_5_txt);
ASSET(comp_white_4_txt);
ASSET(comp_white_6_txt);
ASSET(comp_white_7_txt);
ASSET(comp_white_8_txt);
ASSET(comp_white_9_txt);
void redAuton(){
    chassis.setPose(-40, 63, 90);
    arm.setPosition(0);
    uint32_t timeStart = pros::millis();
    intake.setAutonControlFlag(false);
    intakeMotor.move(0);
    // intakeLift.extend();
    mogoRushClamp.extend(); //reversed
    // chassis.moveToPose(-9, 60, 120, 1700, {.maxSpeed = 127,
    //                                         .minSpeed = 90}, 0);
    chassis.follow(comp_white_1_txt, 15, 3000, 1);
    delay(150);
    mogoRushReach.extend();
    i_waitUntil(!chassis.isInMotion());
    delay(150);
    mogoRushClamp.retract();
    chassis.moveToPose(-33, 64, 80, 2500, {.forwards = false,
                                            .maxSpeed = 127,
                                            .minSpeed = 80}, true);

    delay(1000);
    mogoRushClamp.extend();
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(270, 1500,{.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE,
                                    .maxSpeed = 127/3}, true);
    delay(500);
    mogoRushReach.retract();

    // chassis.follow(comp_white_2_txt, 15, 5000, 0);
    chassis.moveToPose(-6, 66, 270, 2000, {.forwards = false, .minSpeed = 127/3}, 0);
    i_waitUntil(!chassis.isInMotion());
    mogoRushClamp.retract();
    mogoClamp.extend();
    chassis.swingToHeading(230, lemlib::DriveSide::RIGHT, 1300, {.maxSpeed = 127/2}, 0);

    intake.setAutonControlFlag(true);
    // chassis.moveToPose(-46, 48, 270, 3000, {.maxSpeed = 127/4}, 0);
    // chassis.moveToPose(-69, 69, 315, 1500, {}, 0);
    chassis.follow(comp_white_4_txt, 15, 7000, 1, 1);
    
    chassis.moveToPose(-55, 55, 315, 1500, {.forwards = 0, .maxSpeed = 127/2}, 0);
    chassis.turnToHeading(180, 1200, {.maxSpeed = 127/3}, 0);
    intake.setAutonControlFlag(true);
    
    chassis.moveToPose(-55, 7, 180, 4000, {.forwards = 1, .maxSpeed = 127, .minSpeed = 127/3}, 0);
    // delay(100);
    
    chassis.turnToHeading(275, 2000, {.maxSpeed = 127/3}, 1);
    i_waitUntil(!chassis.isInMotion());
    chassis.tank(70, 70, false);
    delay(900);
    chassis.tank(0, 0, false);
    int counter = 0;
	while(counter < 35)
	{
		arm.setPosition(105);
		counter++;
		delay(15);
	}
    chassis.tank(-70, -70, false);
    delay(500);
    arm.setPosition(0);
    delay(1000);
    chassis.tank(0, 0, false);
    intake.setAutonControlFlag(false);
};

ASSET(comp_white_1b_txt);
ASSET(comp_white_4b_txt);

void blueAuton(){
    chassis.setPose(40, 63, 270);
    arm.setPosition(0);
    uint32_t timeStart = pros::millis();
    intake.setAutonControlFlag(false);
    intakeMotor.move(0);
    // intakeLift.extend();
    mogoRushClamp.extend(); //reversed
    // chassis.moveToPose(-9, 60, 120, 1700, {.maxSpeed = 127,
    //                                         .minSpeed = 90}, 0);
    chassis.follow(comp_white_1b_txt, 15, 3000, 1);
    delay(150);
    mogoRushReach.extend();
    i_waitUntil(!chassis.isInMotion());
    delay(150);
    mogoRushClamp.retract();
    chassis.moveToPose(33, 32, 280, 2500, {.forwards = false,
                                            .maxSpeed = 127,
                                            .minSpeed = 80}, true);

    delay(1000);
    mogoRushClamp.extend();
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(90, 1500,{.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE,
                                    .maxSpeed = 127/3}, true);
    delay(500);
    mogoRushReach.retract();

    // chassis.follow(comp_white_2_txt, 15, 5000, 0);
    chassis.moveToPose(6, 30, 90, 2000, {.forwards = false, .minSpeed = 127/3}, 0);
    i_waitUntil(!chassis.isInMotion());
    mogoRushClamp.retract();
    mogoClamp.extend();
    chassis.swingToHeading(50, lemlib::DriveSide::RIGHT, 1300, {.maxSpeed = 127/2}, 0);

    intake.setAutonControlFlag(true);
    // chassis.moveToPose(-46, 48, 270, 3000, {.maxSpeed = 127/4}, 0);
    // chassis.moveToPose(-69, 69, 315, 1500, {}, 0);
    chassis.follow(comp_white_4b_txt, 15, 7000, 1, 1);
    
    chassis.moveToPose(55, 55, 45, 1500, {.forwards = 0, .maxSpeed = 127/2}, 0);
    chassis.turnToHeading(180, 1200, {.maxSpeed = 127/3}, 0);
    intake.setAutonControlFlag(true);
    
    chassis.moveToPose(55, 7, 180, 4000, {.forwards = 1, .maxSpeed = 127, .minSpeed = 127/3}, 0);
    // delay(100);
    
    chassis.turnToHeading(85, 2000, {.maxSpeed = 127/3}, 1);
    i_waitUntil(!chassis.isInMotion());
    chassis.tank(70, 70, false);
    delay(900);
    chassis.tank(0, 0, false);
    int counter = 0;
	while(counter < 35)
	{
		arm.setPosition(105);
		counter++;
		delay(15);
	}
    chassis.tank(-70, -70, false);
    delay(500);
    arm.setPosition(0);
    delay(1000);
    chassis.tank(0, 0, false);
    intake.setAutonControlFlag(false);
}

//SKILLS ASSETS
ASSET(skills_white_1_txt);
ASSET(skills_white_3_txt);
ASSET(skills_white_4_txt);
ASSET(skills_white_5_txt);

// void skillsCaller() {
//     uint32_t timeStart = pros::millis();
//     pros::Task autotask(skillsAuton);
//     while(pros::millis()-timeStart < 60000)
//         delay(10);
    
//     autotask.remove();
//     ctrl_master.rumble("---");
// }

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
    intake.setVoltage(100);

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
}