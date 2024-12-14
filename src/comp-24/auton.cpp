#include "robots/comp-24/auton.h"
static bool isBlueAuto = 0;
void runAuton(bool isBlue)
{
    if (isBlue)
    {
        isBlueAuto = 1;
        blueAuton();
    }
    else
    {
        isBlueAuto = 0;
        redAuton();
    }
};

void skillsCaller() {
    uint32_t timeStart = pros::millis();
    pros::Task autotask(redAuton);
    while(pros::millis()-timeStart < 30000)
        delay(10);
    
    autotask.remove();
    ctrl_master.rumble("---");
}

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
    chassis.setPose(-53.5, 61, 90);
    uint32_t timeStart = pros::millis();
    intake.setAutonControlFlag(false);
    intakeMotor.move(0);
    intakeLift.extend();
    mogoRushReach.extend();
    mogoRushClamp.extend(); //reversed
    chassis.follow(comp_white_1_txt, 15, 1500, 1, 0);

    mogoRushClamp.retract();
    chassis.moveToPose(-44, 60, 90, 1700, {.forwards = false,
                                            .maxSpeed = 127,
                                            .minSpeed = 127}, 0);

    chassis.tank(0, 0, 0);
    delay(500);

    chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE,
                                    .maxSpeed = 127/2});
    while(chassis.isInMotion()){
        if(chassis.getPose().theta <= 80){
            mogoRushClamp.extend();
            mogoRushReach.retract();
        }
        delay(15);
    }
    delay(250);

    chassis.follow(comp_white_2_txt, 15, 5000, 0);
    while(chassis.isInMotion()){
        if(chassis.getPose().y <= 50){
            mogoClamp.extend();
        }
        delay(15);
    }

    chassis.moveToPose(-22, 36, 345, 3000, {.maxSpeed = 127/2}, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= 28){
            intake.setAutonControlFlag(true);
            intakeMotor.move(127);
        }
        delay(15);
    }

    intakeLift.retract();
    delay(400);
    chassis.swingToHeading(315, lemlib::DriveSide::RIGHT, 800, {}, 0);
    intake.setAutonControlFlag(true);
    intakeMotor.move(127);
    // chassis.moveToPose(-46, 48, 270, 3000, {.maxSpeed = 127/4}, 0);
    // chassis.moveToPose(-69, 69, 315, 1500, {}, 0);
    chassis.follow(comp_white_4_txt, 15, 3000, 1, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -56){
            chassis.cancelMotion();
        }
        delay(15);
    }
    chassis.moveToPose(-69, 66.5, 315, 1500, {.maxSpeed = 127/2}, 0);
    chassis.turnToHeading(315, 700, {}, 0);
    
    chassis.moveToPose(-51, 48, 315, 1000, {.forwards = 0, .maxSpeed = 127, .minSpeed = 127/3}, 0);
    // delay(100);
    
    chassis.turnToHeading(90, 700, {.minSpeed = 127/2}, 1);
    // chassis.follow(comp_white_5_txt, 15, 3000, 1, 1);
    chassis.moveToPose(-2, 59, 90, 2000, {.maxSpeed = 127, .minSpeed = 127/3}, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -28){
            intakeLift.extend();
        }
        delay(15);
    }
    delay(500);
    intakeLift.retract();
    delay(500);
    // chassis.turnToHeading(0, 700, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE}, 0);
    chassis.swingToHeading(24, lemlib::DriveSide::RIGHT, 800, {}, 0);
    chassis.follow(comp_white_6_txt, 15, 5000, 0, 0);

    mogoClamp.retract();
    
    
    // chassis.turnToHeading(118, 700, {}, 0);
    chassis.swingToHeading(120, lemlib::DriveSide::RIGHT, 800, {.minSpeed = 127/2}, 0);

    chassis.follow(comp_white_8_txt, 15, 2000, 0, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= 4){
            mogoClamp.extend();
        }
        delay(15);
    }
    intake.setAutonControlFlag(true);
    intakeMotor.move(127);
    chassis.turnToHeading(255, 700, {.minSpeed = 127/2}, 0);
    // chassis.moveToPose(-66, -5, 225, 1000, {.maxSpeed = 127/2}, 0);
    // chassis.follow(comp_white_9_txt, 15, 2500, 1, 0);
    chassis.moveToPose(-67.5, -7.5, 225, 1500, {.maxSpeed = 127, .minSpeed = 127/2}, 0);
    chassis.moveToPose(-50, 10, 225, 1500, {.forwards = 0, .minSpeed = 127}, 0);

};

ASSET(mirror_comp_white_1_txt);
ASSET(mirror_comp_white_2_txt);
void blueAuton(){
    uint32_t timeStart = pros::millis();
    chassis.setPose(53.5, 33.5, 270);
    intake.setAutonControlFlag(false);
    intakeMotor.move(0);
    intakeLift.extend();
    mogoRushReach.extend();
    mogoRushClamp.extend(); //reversed
    chassis.follow(mirror_comp_white_1_txt, 15, 1500, 1, 0);

    mogoRushClamp.retract();
    chassis.moveToPose(44, 38, 270, 1700, {.forwards = false,
                                           .maxSpeed = 127,
                                           .minSpeed = 127}, 0);

    chassis.tank(0, 0, 0);
    delay(500);

    chassis.turnToHeading(90, 1400, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE,
                                     .maxSpeed = 127/2});
    while(chassis.isInMotion()){
        if(chassis.getPose().theta <= 260){
            mogoRushClamp.extend();
            mogoRushReach.retract();
        }
        delay(15);
    }
    delay(250);
    chassis.follow(mirror_comp_white_2_txt, 15, 5000, 0, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= 50){
            mogoClamp.extend();
        }
        delay(15);
    }

    chassis.moveToPose(22, 36, 15, 2500, {.maxSpeed = 127/2}, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().y >= 28){
            intake.setAutonControlFlag(true);
            intakeMotor.move(127);
        }
        delay(15);
    }

    intakeLift.retract();
    delay(400);
    chassis.swingToHeading(45, lemlib::DriveSide::RIGHT, 800, {}, 0);
    intake.setAutonControlFlag(true);
    intakeMotor.move(127);
    /*
    // chassis.follow(comp_blue_4_txt, 15, 3000, 1, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= 56){
            chassis.cancelMotion();
        }
        delay(15);
    }
    chassis.moveToPose(69, -66.5, 45, 1500, {.maxSpeed = 127/2}, 0);
    chassis.turnToHeading(45, 700, {}, 0);
    
    chassis.moveToPose(51, -48, 45, 1000, {.forwards = false, .maxSpeed = 127, .minSpeed = 127/3}, 0);
    // delay(100);
    
    chassis.turnToHeading(270, 700, {.minSpeed = 127/2}, 1);
    // chassis.follow(comp_blue_5_txt, 15, 3000, 1, 1);
    chassis.moveToPose(2, -59, 270, 2000, {.maxSpeed = 127, .minSpeed = 127/3}, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= 28){
            intakeLift.extend();
        }
        delay(15);
    }
    delay(500);
    intakeLift.retract();
    delay(500);
    // chassis.turnToHeading(0, 700, {.direction = lemlib::AngularDirection::CW_CLOCKWISE}, 0);
    chassis.swingToHeading(336, lemlib::DriveSide::LEFT, 800, {}, 0);
    // chassis.follow(comp_blue_6_txt, 15, 5000, 0, 0);

    mogoClamp.retract();
    
    // chassis.turnToHeading(-118, 700, {}, 0);
    chassis.swingToHeading(240, lemlib::DriveSide::LEFT, 800, {.minSpeed = 127/2}, 0);

    // chassis.follow(comp_blue_8_txt, 15, 2000, 0, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().y <= -4){
            mogoClamp.extend();
        }
        delay(15);
    }
    intake.setAutonControlFlag(true);
    intakeMotor.move(127);
    chassis.turnToHeading(105, 700, {.minSpeed = 127/2}, 0);
    // chassis.moveToPose(66, 5, -225, 1000, {.maxSpeed = 127/2}, 0);
    // chassis.follow(comp_blue_9_txt, 15, 2500, 1, 0);
    chassis.moveToPose(67.5, 7.5, 135, 1500, {.maxSpeed = 127, .minSpeed = 127/2}, 0);
    chassis.moveToPose(50, -10, 135, 1500, {.forwards = false, .minSpeed = 127}, 0);
    */
}
