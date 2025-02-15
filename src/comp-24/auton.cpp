#include "robots/comp-24/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
    if(ringToKeep == Ring::Color::RED) {
        redAuton();
    } else {
        blueAuton();
    }
    // chassis.setPose(0, 0, 0);
    // while(true){
    //     lcd::print(0, "x: %f, y: %f, theta: %f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    //     delay(10);
    // }
}


// RED ASSETS
ASSET(comp_white_1_txt);
ASSET(comp_white_2_txt);
ASSET(comp_white_4_txt);
ASSET(comp_white_6_txt);
ASSET(comp_white_8_txt);

void redAuton(){
    blueAuton();
};

ASSET(mirror_comp_white_1_txt);
void blueAuton(){
    chassis.setPose(-45.5, -29.3, 100);
    actions.setIntakeSpeed(0);
    pneumatics.intakeLift.extend();
    pneumatics.mogoRushArm.extend();
    pneumatics.mogoRushClamp.extend();
    chassis.follow(comp_white_1_txt, 15, 1500, 1, 0);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -22){
            delay(50);
            pneumatics.mogoRushClamp.retract();
            chassis.cancelMotion();
        }
        delay(15);
    }
    pneumatics.mogoRushClamp.retract();
    chassis.moveToPose(-45.5, -29.3, 150, 8000, {.forwards = false,
        .maxSpeed = 127,
        .minSpeed = 127}, 0);
    chassis.tank(0, 0, 0);
    delay(500);
    pneumatics.mogoRushClamp.extend();
    chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE,
        .maxSpeed = 127/2});
    while(chassis.isInMotion()){
        if(chassis.getPose().theta <= 80){
            pneumatics.mogoRushArm.retract();
            pneumatics.mogoRushClamp.extend();
        }
            delay(15);
    }
    delay(250);
    pneumatics.mogoRushClamp.extend();
    actions.releaseIntake();

    // chassis.follow(comp_white_2_txt, 15, 5000, 0);
    chassis.moveToPose(-10, -29.3, 270, 2000, {.forwards = false, .minSpeed = 127/3}, 0);
    i_waitUntil(!chassis.isInMotion());
    pneumatics.mogoRushClamp.retract();
    pneumatics.mogoClamp.extend();
    actions.setIntakeSpeed(1);
    chassis.turnToHeading(0, 1000, {.maxSpeed = 127/2}, 0);
    actions.setArmState(Arm::State::DESCORE);
    // chassis.swingToHeading(200, lemlib::DriveSide::LEFT, 1300, {.maxSpeed = 127/2}, 0);

    // actions.setIntakeSpeed(1);
    // chassis.follow(comp_white_4_txt, 15, 8000, 1, 0);
    
    // chassis.moveToPose(-55, -55, 315, 1500, {.forwards = 0, .maxSpeed = 127/2}, 0);
    // chassis.turnToHeading(180, 1200, {.maxSpeed = 127/3}, 0);
    // actions.setIntakeSpeed(1);
    // chassis.moveToPose(-60, -72, 225, 1500, {.maxSpeed=80}, false); 
    // chassis.moveToPose(-51, -61, 225, 700, {.forwards=false, .maxSpeed=80}, false);
    // chassis.moveToPose(-60, -72, 225, 700, {.maxSpeed=80}, false);
    // chassis.moveToPose(-51, -61, 225, 700, {.forwards=false, .maxSpeed=80}, false);
    // chassis.moveToPose(-60, -72, 225, 700, {.maxSpeed=80}, false);
    // chassis.moveToPose(-51, -61, 225, 700, {.forwards=false, .maxSpeed=80}, false);
    // chassis.moveToPose(-60, -72, 225, 700, {.maxSpeed=80}, false);
    // chassis.moveToPose(-51, -61, 225, 700, {.forwards=false, .maxSpeed=80}, false);    
    // chassis.moveToPose(-60, -72, 225, 700, {.maxSpeed=80}, false);
    // chassis.moveToPose(-48, -57, 225, 700, {.forwards=false, .maxSpeed=100}, false);
}

// //SKILLS ASSETS
// ASSET(skills_white_1_txt);
// ASSET(skills_white_3_txt);
// ASSET(skills_white_4_txt);
// ASSET(skills_white_5_txt);

// // void skillsCaller() {
// //     uint32_t timeStart = pros::millis();
// //     pros::Task autotask(skillsAuton);
// //     while(pros::millis()-timeStart < 60000)
// //         delay(10);
    
// //     autotask.remove();
// //     ctrl_master.rumble("---");
// // }

// void skillsAuton(){

//     uint32_t timeStart = pros::millis();
//     chassis.setPose(-52, 27, 90);
//     in

//     chassis.turnToHeading(200, 1000, {.maxSpeed = 127/2}, false);
//     chassis.moveToPose(-19.5, 51.5, 240, 2400, {.forwards = false, .minSpeed = 45}, false);

//     mogoClamp.extend();
//     delay(250);
//     chassis.turnToHeading(200, 1000, {}, true);

//     intake.setVoltage(127);
//     chassis.follow(skills_white_1_txt, 15, 7000, true, true);    

//     while(chassis.isInMotion()){ 
//         if(chassis.getPose().x >= -1 && chassis.getPose().y <= 21){
//             intake.brake();
//         } 
//         else if(chassis.getPose().x >= -19 && chassis.getPose().x <= -12){
//             intake.brake();
//         }
//         else {
//             intake.setVoltage(127);
//         }
//         delay(10);
//     }
   
//     chassis.moveToPose(-66, 78, 320, 3300, {.maxSpeed = 127/2}, false);
    
//     delay(800);
//     intake.setVoltage(100);
//     chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 320, 1200, {.forwards = false}, false);
//     chassis.turnToHeading(135, 1000, {}, false); 
//     chassis.moveToPose(-58, 72, 135, 1000, {.forwards = false});
//     i_waitUntil(!chassis.isInMotion());
//     mogoClamp.retract();
//     intake.setVoltage(0);   
//     i_waitUntil(!chassis.isInMotion());
//     chassis.moveToPose(-48, 54.5, 135, 1000);       

//     chassis.turnToHeading(230, 1000, {}, false);   
//     chassis.moveToPose(35, 30, 310, 3000, {.forwards = false}, false);
//     mogoClamp.extend();   
//     delay(200); 
//     intake.setAutonControlFlag(true);
//     intake.setVoltage(100);

//     chassis.moveToPose(10, 64, 330, 3000, {}, false);
//     delay(500);
//     chassis.turnToHeading(90, 1000, {.maxSpeed = 60}, false);  
//     chassis.moveToPose(56, 33, 135, 2000, {}, false);
//     delay(500);
//     chassis.turnToHeading(0, 1000, {}, false);  

//     // chassis.moveToPose(53, 47, 20, 3000, {}, false);
//     // delay(300);
//     // chassis.moveToPose(56, 33, 0, 2200, {.forwards = false}, false);
//     chassis.follow(skills_white_4_txt, 15, 4000, true, false); 

//     i_waitUntil(!chassis.isInMotion());
//     delay(400);
//     chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 45, 2200, {.forwards = false}, false);
//     intake.setAutonControlFlag(false);
//     intake.setVoltage(127);
//     chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 45, 2200, {}, false);
//     delay(400);
//     chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 45, 2200, {.forwards = false}, false);
//     chassis.turnToHeading(225, 1000, {}, false);  
//     chassis.moveToPose(72, 76, 225, 1600, {.forwards = false}); 
//     i_waitUntil(!chassis.isInMotion());
//     mogoClamp.retract();    
    
//     chassis.moveToPose(56, 50, 225, 1600); 
//     i_waitUntil(!chassis.isInMotion());
//     chassis.turnToHeading(0, 800, {}, false); 
//     intake.setAutonControlFlag(true);

//     chassis.moveToPose(56, -20, 0, 1600, {.forwards = false}, false); 
//     i_waitUntil(!chassis.isInMotion());
//     mogoClamp.extend();  
//     delay(200);
//     chassis.turnToHeading(45, 1000, {}, false);  
//     intake.setAutonControlFlag(false);
//     intake.setVoltage(127);
//     chassis.moveToPose(chassis.getPose().x + 5*sqrt(2), chassis.getPose().y + 5*sqrt(2), 45, 1000, {}, false);
//     delay(400);
//     chassis.moveToPose(chassis.getPose().x - 5*sqrt(2), chassis.getPose().y - 5*sqrt(2), 45, 1000, {.forwards = false}, false);
//     chassis.moveToPose(chassis.getPose().x + 10*sqrt(2), chassis.getPose().y + 10*sqrt(2), 45, 1000, {}, false);
//     chassis.moveToPose(chassis.getPose().x - 10*sqrt(2), chassis.getPose().y - 10*sqrt(2), 45, 1000, {.forwards = false}, false);

//     chassis.follow(skills_white_5_txt, 15, 8000, true, true);    
//     while(chassis.isInMotion()){ 
//         if(chassis.getPose().x >= 3 && chassis.getPose().y <= 50){
//             mogoClamp.retract();
//             intake.setVoltage(0);
//         } 

//         delay(10);
//     }          
//     i_waitUntil(!chassis.isInMotion());

//     chassis.turnToHeading(0, 1000, {}, false); 
//     chassis.moveToPose(0, 0, 45, 6000, {.forwards = false, .minSpeed = 70}); 
//     // arm.setState(3);
//     while(true)
//     {
//         // arm.manualControl();
//         arm.setPosition(140);

//         if((pros::millis() - timeStart) > 59000)
//         {
//             arm.setPosition(0);
//             // arm.manualControl();
//         }

//         delay(10);
//     }
// }