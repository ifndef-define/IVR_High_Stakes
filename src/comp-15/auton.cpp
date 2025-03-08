#include "robots/comp-15/auton.h"

void auton(Ring::Color ringToKeep) {
    //Add auton code here
    // bool run = true;
    actions.setRingColor(ringToKeep);
    actions.setAutonControlFlag(true);
    // actions.setIntakeSpeed(1);

    if(ringToKeep == Ring::Color::RED) {
        redAuton();
    }else if(ringToKeep == Ring::Color::BLUE) {
        blueAuton();
    } else {
        // skillsAuton();
    }
}

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
ASSET(comp_black_2_txt);
ASSET(comp_black_4_txt);
ASSET(comp_black_5_txt);

void redAuton(){
    chassis.setPose(-49.5, -19, 75);
    actions.releaseIntake();
    pneumatics.mogoRushLeftArm.extend();
    pneumatics.mogoRushClamp.retract();
    chassis.follow(comp_black_1_txt, 15, 6000, true);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -25){
            delay(50);
            pneumatics.mogoRushClamp.extend();
        }
        delay(15);
    }
    i_waitUntil(!chassis.isInMotion());
    delay(200);
    // chassis.moveToPose(-49.5, -19, 75, 4000, {.forwards=false, .maxSpeed=80}, false);
    chassis.follow(comp_black_2_txt, 15, 6000, false, true);
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -40){
            delay(50);
            pneumatics.mogoRushClamp.retract();            
        }
        delay(15);
    }
    i_waitUntil(!chassis.isInMotion());
    pneumatics.mogoRushLeftArm.retract();
    actions.nextArmState();
    delay(500);
    chassis.turnToHeading(180, 1000, {.maxSpeed=100}, true);
    delay(200);
    pneumatics.mogoRushLeftArm.retract();
    i_waitUntil(!chassis.isInMotion());
    chassis.moveToPose(-50, 4, 180, 2000, {.forwards=false, .maxSpeed=80}, true);
    actions.setIntakeSpeed(1);
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= 0){
            pneumatics.mogoClamp.extend();           
        }
        delay(15);
    }
    i_waitUntil(!chassis.isInMotion());
    actions.setIntakeSpeed(0);
    actions.nextArmState();
    delay(200);
    chassis.turnToHeading(255, 1000, {.maxSpeed=100}, false);
    actions.setIntakeSpeed(1);
    delay(200);
    chassis.moveToPose(-53, 2, 260, 2000, {.maxSpeed=80}, true);
    delay(300);
    actions.nextArmState();
    i_waitUntil(!chassis.isInMotion());
    chassis.moveToPose(-50, 4, 270, 2000, {.forwards=false, .maxSpeed=80}, true);
    delay(500);
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(160, 1000, {.maxSpeed=100}, false);
    actions.nextArmState();
    chassis.follow(comp_black_4_txt, 15, 8000, true);
    i_waitUntil(!chassis.isInMotion());
    delay(2000);
    chassis.moveToPose(-35, -35, 110, 2000, {.forwards=false, .maxSpeed=80}, false);
    chassis.turnToHeading(225, 1000, {.maxSpeed=100}, false);
    chassis.moveToPose(-60, -65, 225, 500, {.maxSpeed=100}, false); 
    chassis.moveToPose(-50, -55, 225, 500, {.forwards=false, .maxSpeed=100}, false);
    chassis.moveToPose(-60, -65, 225, 500, {.maxSpeed=100}, false); 
    chassis.moveToPose(-50, -55, 225, 500, {.forwards=false, .maxSpeed=100}, false);
    chassis.moveToPose(-60, -65, 225, 500, {.maxSpeed=100}, false); 
    chassis.moveToPose(-50, -55, 225, 500, {.forwards=false, .maxSpeed=100}, false);
    chassis.moveToPose(-60, -65, 225, 500, {.maxSpeed=100}, false); 
    chassis.moveToPose(-45, -50, 225, 800, {.forwards=false, .maxSpeed=100}, false);
    chassis.turnToHeading(45, 1000, {.maxSpeed=100}, false);
    pneumatics.mogoClamp.retract();
    actions.setIntakeSpeed(0);
    delay(500);
    chassis.moveToPose(-80, -80, 45, 800, {.forwards=false, .maxSpeed=127}, false);
    delay(500);
    chassis.moveToPose(-20, -35, 90, 1500, {.maxSpeed=70}, false);
    chassis.turnToHeading(180, 1000, {.maxSpeed=100}, false);
    chassis.moveToPose(-20, 0, 180, 800, {.forwards=false, .maxSpeed=80}, false);
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= -15){
            pneumatics.mogoClamp.extend();           
        }
        delay(15);
    }
    delay(500);
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(90, 1000, {.maxSpeed=100}, false);








    

    
    // // drop mogo rush right as auto starts
    // // pneumatics.mogoRushArm.extend();
    // pneumatics.mogoRushClamp.extend();
    // // move to grab mogo under ladder
    // chassis.follow(comp_black_1_txt, 15, 3000, true);
    // // release intake
    // actions.releaseIntake();
    // // wait
    // i_waitUntil(!chassis.isInMotion());
    // // clamp goal
    // pneumatics.mogoRushClamp.retract();
    // delay(300);
    // // chassis.swingToHeading(200, lemlib::DriveSide::LEFT, 1000, {.maxSpeed=100}, false);
    // chassis.follow(comp_black_5_txt, 15, 3000, false, true);
    // delay(700);
    // // pneumatics.mogoRushArm.retract();
    // pneumatics.mogoRushClamp.retract();
    // i_waitUntil(!chassis.isInMotion());
    // actions.setIntakeSpeed(1);
    // pneumatics.mogoClamp.extend();
    // delay(500);
    // chassis.turnToHeading(270, 1000, {.maxSpeed=100}, false);
    // chassis.moveToPose(-58, -6, 270, 1000, {.maxSpeed=80}, false);
    // chassis.moveToPose(-55, -3, 270, 500, {.forwards=false, .maxSpeed=80}, false);    
    // chassis.turnToHeading(125, 1000, {.maxSpeed=100}, false);
    // chassis.moveToPose(-21, -50, 180, 2000, {.maxSpeed=50}, false);   
    // chassis.moveToPose(-21, -67, 180, 1200, {.maxSpeed=50}, false); 
    // chassis.moveToPose(-40, -27, 180, 1500, {.forwards=false, .maxSpeed=80}, false);
    // i_waitUntil(!chassis.isInMotion());
    // chassis.turnToHeading(225, 1000, {.maxSpeed=100}, false);
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
    // chassis.turnToHeading(45, 1000, {.maxSpeed=100}, false);
    // pneumatics.mogoClamp.retract();
    // delay(500);
    // chassis.moveToPose(-63, -72, 225, 500, {.forwards=false, .maxSpeed=127}, false);
    // delay(500);
    // actions.setIntakeSpeed(0);
    // chassis.moveToPose(-20, -20, 20, 2000, {.maxSpeed=100}, false);
    // chassis.turnToHeading(180, 1000, {.maxSpeed=100}, false);
    // chassis.moveToPose(-20, 10, 180, 2000, {.forwards=false, .maxSpeed=100}, false);
    // // actions.setArmSpeed(25);

};

// ASSET(compBlack1bcc_txt);
// ASSET(compBlack5bcc_txt);
// void blueAuton() {
//     // CC ~ 54.5 -17 270 
//     chassis.setPose(54.5, -17, 270);
//     actions.setArmState(Arm::State::DOWN);
//     pneumatics.mogoRushArm.extend();
//     pneumatics.mogoRushClamp.extend();
//     chassis.follow(compBlack1bcc_txt, 15, 3000, true, false );
//     pneumatics.mogoRushClamp.retract();
//     delay(300);
//     chassis.follow(compBlack5bcc_txt, 15, 3000, false, true);
//     delay(900);
//     pneumatics.mogoRushArm.retract();
//     pneumatics.mogoRushClamp.extend();
//     actions.releaseIntake();
//     i_waitUntil(!chassis.isInMotion());
//     // actions.setIntakeSpeed(1);
//     pneumatics.mogoClamp.extend();
//     actions.setIntakeSpeed(1);
//     delay(1500);
//     actions.setIntakeSpeed(0);
//     // delay(500);
//     // pneumatics.mogoClamp.retract();
//     // chassis.turnToHeading(90, 1000, {.maxSpeed=100}, false);
//     // chassis.moveToPose(58, 6, 90, 1000, {.maxSpeed=80}, false);
//     // delay(500);
//     // chassis.turnToHeading(235, 1000, {.maxSpeed=100}, false);
//     // chassis.moveToPose(30, -50, 180, 2000, {.maxSpeed=50}, false);   
//     // chassis.moveToPose(30, -68, 180, 1000, {.maxSpeed=50}, false); 
//     // chassis.moveToPose(42, -36, 225, 1500, {.forwards=false, .maxSpeed=80}, false);
//     // i_waitUntil(!chassis.isInMotion());
//     // chassis.turnToHeading(135, 1000, {.maxSpeed=100}, false);
//     // actions.setArmState(Arm::State::DOWN);
//     // chassis.moveToPose(66, -58, 135, 1500, {.maxSpeed=60}, false); 
//     // chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
//     // chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     // chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
//     // chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     // chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
//     // chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     // chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);    
//     // chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     // chassis.moveToPose(55, -48, 135, 700, {.forwards=false, .maxSpeed=100}, false);
//     // chassis.turnToHeading(315, 1000, {.maxSpeed=100}, false);
//     // pneumatics.mogoClamp.retract();
//     // delay(500);
//     // chassis.moveToPose(70, -62, 135, 500, {.forwards=false, .maxSpeed=127}, false);
//     // delay(500);
//     // actions.setIntakeSpeed(0);
//     // chassis.moveToPose(23, -20, 0, 2000, {.maxSpeed=100}, false);
//     // chassis.turnToHeading(180, 1000, {.maxSpeed=100}, false);
//     // chassis.moveToPose(23, -7, 180, 2000, {.forwards=false, .maxSpeed=100}, false);
// };

// ASSET(compBlack1b_txt);
// ASSET(compBlack5b_txt);
// void blueAuton() {

//     chassis.setPose(54, -13, 270);
//     arm.setPosition(0);

//     // drop mogo rush right as auto starts
//     mogoRushReach.extend();
//     mogoRushClamp.extend();
//     // move to grab mogo under ladder
//     chassis.follow(compBlack1b_txt, 15, 3000, true);
//     // release intake
//     intake.releaseIntake();
//     // wait
//     i_waitUntil(!chassis.isInMotion());
//     // clamp goal
//     mogoRushClamp.retract();
//     delay(300);
//     chassis.follow(compBlack5b_txt, 15, 3000, false, true);
//     delay(900);
//     mogoRushReach.retract();
//     mogoRushClamp.extend();
//     i_waitUntil(!chassis.isInMotion());
//     intake.setAutonControlFlag(true); 
//     mogoClamp.extend();
//     delay(500);
//     mogoRushClamp.retract();
//     chassis.turnToHeading(90, 1000, {.maxSpeed=100}, false);
//     chassis.moveToPose(58, 6, 90, 1000, {.maxSpeed=80}, false);
//     delay(500);
//     chassis.turnToHeading(235, 1000, {.maxSpeed=100}, false);
//     chassis.moveToPose(30, -50, 180, 2000, {.maxSpeed=50}, false);   
//     chassis.moveToPose(30, -68, 180, 1000, {.maxSpeed=50}, false); 
//     chassis.moveToPose(42, -36, 225, 1500, {.forwards=false, .maxSpeed=80}, false);
//     i_waitUntil(!chassis.isInMotion());
//     chassis.turnToHeading(135, 1000, {.maxSpeed=100}, false);
//     arm.setPosition(0);
//     chassis.moveToPose(66, -58, 135, 1500, {.maxSpeed=60}, false); 
//     chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
//     chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
//     chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
//     chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);    
//     chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
//     chassis.moveToPose(55, -48, 135, 700, {.forwards=false, .maxSpeed=100}, false);
//     chassis.turnToHeading(315, 1000, {.maxSpeed=100}, false);
//     mogoClamp.retract();
//     delay(500);
//     chassis.moveToPose(70, -62, 135, 500, {.forwards=false, .maxSpeed=127}, false);
//     delay(500);
//     intake.setAutonControlFlag(false); 
//     chassis.moveToPose(23, -20, 0, 2000, {.maxSpeed=100}, false);
//     chassis.turnToHeading(180, 1000, {.maxSpeed=100}, false);
//     chassis.moveToPose(23, -7, 180, 2000, {.forwards=false, .maxSpeed=100}, false);
// };

ASSET(compBlack1b_txt);
ASSET(compBlack5b_txt);
void blueAuton() {
    chassis.setPose(54, -13, 270);
    actions.setArmState(Arm::State::DOWN);

    // drop mogo rush right as auto starts
    // pneumatics.mogoRushArm.extend();
    pneumatics.mogoRushClamp.extend();
    // move to grab mogo under ladder
    chassis.follow(compBlack1b_txt, 15, 3000, true);
    // release intake
    actions.releaseIntake();
    // wait
    i_waitUntil(!chassis.isInMotion());
    // clamp goal
    pneumatics.mogoRushClamp.retract();
    delay(300);
    chassis.follow(compBlack5b_txt, 15, 3000, false, true);
    delay(900);
    // pneumatics.mogoRushArm.retract();
    pneumatics.mogoRushClamp.extend();
    i_waitUntil(!chassis.isInMotion());
    actions.setIntakeSpeed(1);
    pneumatics.mogoClamp.extend();
    delay(500);
    // pneumatics.mogoClamp.retract();
    chassis.turnToHeading(90, 1000, {.maxSpeed=100}, false);
    chassis.moveToPose(58, 6, 90, 1000, {.maxSpeed=80}, false);
    delay(500);
    chassis.turnToHeading(235, 1000, {.maxSpeed=100}, false);
    chassis.moveToPose(30, -50, 180, 2000, {.maxSpeed=50}, false);   
    chassis.moveToPose(30, -68, 180, 1000, {.maxSpeed=50}, false); 
    chassis.moveToPose(42, -36, 225, 1500, {.forwards=false, .maxSpeed=80}, false);
    i_waitUntil(!chassis.isInMotion());
    chassis.turnToHeading(135, 1000, {.maxSpeed=100}, false);
    actions.setArmState(Arm::State::DOWN);
    chassis.moveToPose(66, -58, 135, 1500, {.maxSpeed=60}, false); 
    chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
    chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
    chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);
    chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(58, -50, 135, 700, {.forwards=false, .maxSpeed=80}, false);    
    chassis.moveToPose(66, -58, 135, 700, {.maxSpeed=80}, false);
    chassis.moveToPose(55, -48, 135, 700, {.forwards=false, .maxSpeed=100}, false);
    chassis.turnToHeading(315, 1000, {.maxSpeed=100}, false);
    pneumatics.mogoClamp.retract();
    delay(500);
    chassis.moveToPose(70, -62, 135, 500, {.forwards=false, .maxSpeed=127}, false);
    delay(500);
    actions.setIntakeSpeed(0);
    chassis.moveToPose(23, -20, 0, 2000, {.maxSpeed=100}, false);
    chassis.turnToHeading(180, 1000, {.maxSpeed=100}, false);
    chassis.moveToPose(23, -7, 180, 2000, {.forwards=false, .maxSpeed=100}, false);
};

// //SKILLS ASSETS
// ASSET(skills_black_1_txt);
// //ASSET(skills_black_2_txt);
// ASSET(skills_black_3_txt);
// ASSET(skills_black_4_txt);

// void skillsAuton(){
//     uint32_t timeStart = pros::millis();

//     chassis.setPose(-52, 0, 90);
//     actions.setArmState(Arm::State::DOWN);
//     actions.setIntakeSpeed(-1);
//     delay(250);
//     actions.setIntakeSpeed(1);
//     delay(750);

//     chassis.moveToPose(-58.5, 0, 90, 1200, {.forwards = false, .maxSpeed = 60});
//     // while(chassis.isInMotion())
//     // {
//     //     if(intakeColor.get_proximity() >= 170)
//     //     {
//     //         intake.brake();
//     //     }
//     //     else
//     //     {
//     //         intake.setVoltage(127);
//     //     }
//     // }
//     // i_waitUntil(!chassis.isInMotion());
//     // intake.setAutonControlFlag(true);
//     delay(750); //tune
//     // intake.setAutonControlFlag(false);
//     actions.setIntakeSpeed(0);

//     chassis.moveToPose(-54, 0, 90, 500);
//     chassis.turnToHeading(300, 1000);
//     chassis.moveToPose(-15, -51, 310, 2800, {.forwards = false, .maxSpeed = 127/2}, false);

//     mogoClamp.extend();   
//     delay(400);

//     intake.setVoltage(127);
//     chassis.follow(skills_black_1_txt, 15, 8000, true, true);    

//     while(chassis.isInMotion()){ 
//         if(chassis.getPose().x >= 0 && chassis.getPose().y >= -27){
//             intake.brake();
//         } 
//         else if(chassis.getPose().x >= -20 && chassis.getPose().x <= -10){
//             intake.brake();
//         }
//         else {
//             intake.setVoltage(127);
//         }
//         delay(10);
//     }          
    
//     chassis.moveToPose(-60, -63.267, 235, 2500, {.maxSpeed = 127/2}, false);

//     delay(400);
//     chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 225, 2200, {.forwards = false}, false);
//     chassis.turnToHeading(45, 1000, {}, false); 
//     chassis.moveToPose(-67, -67, 45, 1600, {.forwards = false}); 
//     i_waitUntil(!chassis.isInMotion());
//     mogoClamp.retract();
//     intake.setVoltage(0);   
//     i_waitUntil(!chassis.isInMotion());
//     chassis.moveToPose(-50, -51.5, 45, 1000);       

//     chassis.turnToHeading(230, 1000, {}, false);   
//     chassis.moveToPose(36, -24, 235, 3000, {.forwards = false, .maxSpeed = 127/2}, true);
//     while(chassis.isInMotion())
//     {
//         if(chassis.getPose().x > 33)
//         {
//             mogoClamp.extend(); 
//         }
//     }
      
//     delay(200);
//     chassis.turnToHeading(210, 1000, {}, false);   
//     intake.setAutonControlFlag(true);

//     chassis.moveToPose(4, -56, 210, 3000, {}, false);
//     delay(500);
//     chassis.turnToHeading(75, 1000, {.maxSpeed = 60}, false);  
//     chassis.moveToPose(50, -28, 45, 3000, {.maxSpeed = 60}, false);
//     delay(500);
//     chassis.turnToHeading(180, 1000, {}, false);  

//     chassis.follow(skills_black_4_txt, 15, 7000, true, false);  

//     i_waitUntil(!chassis.isInMotion());
//     delay(400);
//     chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 135, 2200, {.forwards = false}, false);
//     intake.setAutonControlFlag(false);
//     intake.setVoltage(127);
//     chassis.moveToPose(chassis.getPose().x + 7*sqrt(2), chassis.getPose().y - 7*sqrt(2), 135, 2200, {}, false);
//     delay(400);
//     chassis.moveToPose(chassis.getPose().x - 7*sqrt(2), chassis.getPose().y + 7*sqrt(2), 135, 2200, {.forwards = false}, false);
//     chassis.turnToHeading(315, 2500, {.direction = lemlib::AngularDirection::CW_CLOCKWISE}, false);  
//     chassis.moveToPose(67, -67, 315, 1600, {.forwards = false}); 
//     i_waitUntil(!chassis.isInMotion());
//     mogoClamp.retract();    
//     intake.setVoltage(0);
    
//     chassis.moveToPose(50, -50, 315, 1600); 
//     i_waitUntil(!chassis.isInMotion());
//     chassis.turnToHeading(135, 1000, {}, false); 
//     chassis.moveToPose(0, 0, 315, 6000, {.forwards = false, .maxSpeed = 70}); 
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
// };