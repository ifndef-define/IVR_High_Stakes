#include "robots/comp-24/auton.h"

void auton(Ring::Color ringToKeep) {
    actions.setRingColor(ringToKeep);
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


ASSET(mirror_comp_white_1_txt);
ASSET(mirror_comp_white_2_txt);
ASSET(comp_aurum_1_txt);

void redAuton(){
    // blueAuton();
    chassis.setPose(-37, 30.5, 74);
    pneumatics.mogoRushLeftArm.extend();
    // pneumatics.mogoRushClamp.retract();
    chassis.moveToPose(-15, 36.5, 74, 1500, {.maxSpeed=127, .minSpeed=127}, false);
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x >= -16){
    //         pneumatics.mogoRushClamp.extend();
    //         chassis.cancelMotion();
    //     }
    //     delay(15);
    // }
    i_waitUntil(!chassis.isInMotion());
    pneumatics.mogoRushClamp.extend();
    delay(100);
    chassis.moveToPose(-27, 31, 80, 3000, {.forwards=false, .maxSpeed=127, .minSpeed=90}, true);
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -18){
            pneumatics.mogoRushClamp.retract();
        }
        delay(15);
    }
    i_waitUntil(!chassis.isInMotion());
    chassis.swingToHeading(230, lemlib::DriveSide::RIGHT, 2000, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed=50}, true);
    // chassis.turnToHeading(270, 2000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE}, true);
    delay(300);
    pneumatics.mogoRushLeftArm.retract();
    actions.releaseIntake();
    i_waitUntil(!chassis.isInMotion());
    chassis.moveToPose(-10, 57, 180, 2000, {.forwards=false, .maxSpeed=70}, false);
    pneumatics.mogoClamp.extend();
    delay(400);
    actions.setIntakeSpeed(1);
    chassis.swingToHeading(270, lemlib::DriveSide::RIGHT, 800, {}, false);
    chassis.moveToPose(-48, 52, 270, 2500, {.maxSpeed=35}, false);
    chassis.turnToHeading(320, 800, {}, true);
    chassis.moveToPose(-58, 68, 315, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(-50, 60, 315, 700, {.forwards=false, .maxSpeed=50}, false);
    chassis.moveToPose(-58, 68, 315, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(-50, 60, 315, 700, {.forwards=false, .maxSpeed=50}, false);
    chassis.moveToPose(-58, 68, 315, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(-50, 60, 315, 700, {.forwards=false, .maxSpeed=50}, false);
    chassis.moveToPose(-58, 68, 315, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(-55, 60, 320, 700, {.forwards = false, .maxSpeed=90}, false);
    chassis.turnToHeading(90, 1000, {}, false);

    chassis.moveToPose(-3, 60, 90, 2000, {.maxSpeed = 60}, true);
    delay(500);
    actions.setArmState(Arm::State::READY);
    pneumatics.mogoRushClamp.extend();
    i_waitUntil(!chassis.isInMotion());
    pneumatics.mogoRushClamp.retract();
    delay(500);
    chassis.moveToPose(-6, 60, 90, 2000, {.forwards = false, .maxSpeed = 20}, false);
    delay(500);
    chassis.turnToPoint(12, 76, 1000, {.maxSpeed=80}, false);
    // chassis.turnToPoint(0, 70, 1000, {}, false);
    actions.setIntakeSpeed(0);
    actions.setArmState(Arm::State::DESCORE);
    delay(1500);
    chassis.moveToPose(chassis.getPose().x - 8, chassis.getPose().y - 8, 42.5, 750, {.forwards = false}, false);
    actions.setArmState(Arm::State::DOWN);
    chassis.follow(comp_aurum_1_txt, 15, 5000, 0, 0);
    // chassis.moveToPose(-15, -52, 180, 2500, {}, false);
    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 1000, {}, true);


/*
    chassis.moveToPose(-45.5, 60, 150, 8000, {.forwards = false,
        .maxSpeed = 127,
        .minSpeed = 127}, 0);
    chassis.tank(0, 0, 0);
    delay(500);
    pneumatics.mogoRushClamp.extend();
    chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CW_CLOCKWISE,
        .maxSpeed = 127/2});
    while(chassis.isInMotion()){
        if(chassis.getPose().theta <= 170){
            // pneumatics.mogoRushArm.retract();
            pneumatics.mogoRushClamp.extend();
        }
            delay(15);
    }
    delay(250);
    pneumatics.mogoRushClamp.extend();
    actions.releaseIntake();

    // chassis.follow(comp_white_2_txt, 15, 5000, 0);

    // chassis.moveToPose(-6, -30, 270, 2000, {.forwards = false, .minSpeed = 127/3}, 0);
    chassis.follow(mirror_comp_white_2_txt, 15, 2000, 0, 0);
    i_waitUntil(!chassis.isInMotion());
    pneumatics.mogoRushClamp.retract();
    pneumatics.mogoClamp.extend();
    actions.setIntakeSpeed(1);
    chassis.moveToPose(-68, 64, 270, 2000, {.maxSpeed=80}, false); 
    chassis.moveToPose(-51, 64, 270, 700, {.forwards=false, .maxSpeed=80}, false);
    chassis.moveToPose(-68, 64, 270, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(-51, 64, 270, 700, {.forwards=false, .maxSpeed=80}, false);
    chassis.moveToPose(-65, 64, 270, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(-51, 64, 270, 700, {.forwards=false, .maxSpeed=80}, false);
    chassis.moveToPose(-65, 64, 270, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(-51, 64, 270, 700, {.forwards=false, .maxSpeed=80}, false);    
    chassis.moveToPose(-65, 64, 270, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(-48, 64, 270, 700, {.forwards=false, .maxSpeed=100}, false);

    chassis.turnToHeading(180, 1200, {.maxSpeed = 127/3}, 0);
    actions.setIntakeSpeed(0);
    chassis.moveToPose(-51, -48, 90, 8000, {.forwards = 0, .maxSpeed = 127/2}, 0);
*/
};

ASSET(comp_aurum_1b_txt);

void blueAuton(){
    // blueAuton();
    chassis.setPose(37, 30.5, 286);
    pneumatics.mogoRushRightArm.extend();
    // pneumatics.mogoRushClamp.retract();
    chassis.moveToPose(13, 36.5, 286, 1500, {.maxSpeed=127, .minSpeed=100}, false);
    // while(chassis.isInMotion()){
    //     if(chassis.getPose().x >= -16){
    //         pneumatics.mogoRushClamp.extend();
    //         chassis.cancelMotion();
    //     }
    //     delay(15);
    // }
    i_waitUntil(!chassis.isInMotion());
    pneumatics.mogoRushClamp.extend();
    delay(100);
    chassis.moveToPose(27, 31, 280, 3000, {.forwards=false, .maxSpeed=127, .minSpeed=90}, true);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= 22){
            pneumatics.mogoRushClamp.retract();
        }
        delay(15);
    }
    pneumatics.mogoRushClamp.retract();
    i_waitUntil(!chassis.isInMotion());
    chassis.swingToHeading(130, lemlib::DriveSide::LEFT, 2000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed=50}, true);
    // chassis.turnToHeading(270, 2000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE}, true);
    delay(300);
    pneumatics.mogoRushRightArm.retract();
    actions.releaseIntake();
    i_waitUntil(!chassis.isInMotion());
    chassis.moveToPose(10, 57, 180, 3000, {.forwards=false, .maxSpeed=50}, false);
    pneumatics.mogoClamp.extend();
    delay(400);
    actions.setIntakeSpeed(1);
    chassis.swingToHeading(90, lemlib::DriveSide::LEFT, 800, {}, false);
    chassis.moveToPose(48, 52, 90, 2500, {.maxSpeed=35}, false);
    chassis.turnToHeading(45, 800, {}, true);
    chassis.moveToPose(58, 62, 45, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(50, 54, 45, 700, {.forwards=false, .maxSpeed=50}, false);
    chassis.moveToPose(58, 62, 45, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(50, 54, 45, 700, {.forwards=false, .maxSpeed=50}, false);
    chassis.moveToPose(58, 62, 45, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(50, 54, 45, 700, {.forwards=false, .maxSpeed=50}, false);
    chassis.moveToPose(58, 62, 45, 700, {.maxSpeed=80}, false); 
    chassis.moveToPose(55, 54, 40, 700, {.forwards = false, .maxSpeed=90}, false);
    chassis.turnToHeading(270, 1000, {}, false);

    chassis.moveToPose(3, 60, 270, 2000, {.maxSpeed = 60}, true);
    delay(500);
    actions.setArmState(Arm::State::READY);
    i_waitUntil(!chassis.isInMotion());
    delay(500);
    chassis.moveToPose(6, 60, 270, 2000, {.forwards = false, .maxSpeed = 20}, false);
    delay(500);
    chassis.turnToPoint(-12, 76, 1000, {.maxSpeed=80}, false);
    // chassis.turnToPoint(0, 70, 1000, {}, false);
    actions.setIntakeSpeed(0);
    actions.setArmState(Arm::State::DESCORE);
    delay(1500);
    chassis.moveToPose(chassis.getPose().x + 8, chassis.getPose().y - 8, 42.5, 750, {.forwards = false}, false);
    actions.setArmState(Arm::State::DOWN);
    chassis.follow(comp_aurum_1b_txt, 15, 5000, 0, 0);
    // chassis.moveToPose(-15, -52, 180, 2500, {}, false);
    chassis.swingToHeading(90, lemlib::DriveSide::RIGHT, 1000, {}, true);
}

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