#include "robots/comp-24/auton.h"

void runAuton(bool isBlue)
{
    if (isBlue)
    {
        blueAuton();
    }
    else
    {
        redAuton();
    }
};

void sift(float dist, int n, int timeout, float maxSpeed, float minSpeed)
{
    ;
    lemlib::Pose start = chassis.getPose();
    float thetaRad = start.theta * M_PI / 180;
    for (int i = 0; i < n; i++)
    {
        chassis.moveToPose(start.x + dist * std::cos(thetaRad), start.y + dist * std::sin(thetaRad), start.theta, timeout, {.forwards = true, .maxSpeed = maxSpeed, .minSpeed = minSpeed}, 0);
        delay(100);
        chassis.moveToPose(start.x - dist * std::cos(thetaRad), start.y - dist * std::sin(thetaRad), start.theta, timeout, {.forwards = false, .maxSpeed = maxSpeed, .minSpeed = minSpeed}, 0);
    }
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

// RED ASSETS
ASSET(comp_white_1_txt);
ASSET(comp_white_2_txt);
ASSET(comp_white_3_txt);
ASSET(comp_white_4_txt);
void redAuton(){
    
    intakePist.extend();
    mogoRushReach.extend();
    mogoRushClamp.extend(); //reversed
    chassis.follow(comp_white_1_txt, 15, 1500, 1, 0);

    mogoRushClamp.retract();
    chassis.moveToPose(-42.5, 59, 90, 1600, {.forwards = false,
                                            .maxSpeed = 127,
                                            .minSpeed = 127}, 0);

    chassis.tank(0, 0, 0);
    delay(500);

    chassis.turnToHeading(270, 1400,{.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE,
                                    .maxSpeed = 127/3});
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
        if(chassis.getPose().x >= -19){
            mogoClamp.extend();
        }
        delay(15);
    }

    intake.setAutonControlFlag(true);
    chassis.moveToPose(-21, 35, 345, 3000, {.maxSpeed = 127/2}, 0);

    intakePist.retract();
    delay(400);
    moveBwd(5, 800, 127, 0);
    while(chassis.isInMotion()){
        delay(15);
    }
    // intake.setAutonControlFlag(false);
    chassis.swingToHeading(300, lemlib::DriveSide::RIGHT, 800, {}, 0);
    intake.setAutonControlFlag(true);
    // chassis.moveToPose(-46, 48, 270, 3000, {.maxSpeed = 127/4}, 0);
    // chassis.moveToPose(-69, 69, 315, 1500, {}, 0);
    chassis.follow(comp_white_4_txt, 15, 3000, 1, 0);
    chassis.turnToHeading(315, 700, {}, 0);
    
    float dist = 2.7;
    int n = 3;
    int timeout = 750;
    lemlib::Pose start2 = chassis.getPose();
    for(int i = 0; i < n; i++) {
        chassis.moveToPose(start2.x-(dist*1.414), start2.y+(dist*1.414), 
            315, timeout, {.forwards=false, .maxSpeed=16});
        if(i==0) {
            // delay(300);
            // intakeLift.extend();
        }

        while(chassis.isInMotion()) {
            if(chassis.getPose().x <= chassis.getPose().x-1) {
                intakePist.retract();
                break;
            }
            delay(10);
        }
        i_waitUntil(!chassis.isInMotion());
    
        chassis.moveToPose(start2.x, start2.y, 
            315, timeout, {.maxSpeed=60}, false);

        if (i==2) {
            // delay(50);
            // intake.setAutonControlFlag(false);
            chassis.moveToPose(start2.x+6, start2.y-6, 
                315, timeout, {.forwards = false}, false);
        }
    }
    delay(100);
    
    chassis.turnToHeading(90, 700, {}, 0);
    chassis.follow(comp_white_3_txt, 15, 2000, 1, 1);
    // while(chassis.isInMotion()){
    //     arm.setPosition(16);
    //     if(intakeColor.get_proximity()<200){
    //         delay(60);
    //         intake.setAutonControlFlag(false);
    //         intakeMotor.move_relative(100, 127);
    //         chassis.cancelMotion();
    //     }
    //     delay(15);
    // }
    // intakePist.retract(); // reversed
    // chassis.moveToPose(chassis.getPose().x+4, chassis.getPose().y, 90, 1500, {.forwards = 1,
    //                                         .maxSpeed = 127/2,
    //                                         .minSpeed = 127/3}, 1);
    // while(chassis.isInMotion()){
    //     if(intakeColor.get_proximity()<200){
    //         delay(60);
    //         intake.setAutonControlFlag(false);
    //         intakeMotor.move_relative(100, 127);
    //         chassis.cancelMotion();
    //     }
    //     delay(15);
    // }
        
    // //     delay(15);
    // // }
    // chassis.swingToPoint(0, 69, lemlib::DriveSide::RIGHT, 800, 
    //     {.direction=lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed=127/2}, false);
    //     while(chassis.isInMotion()){
    //         if(chassis.getPose().theta >= 80){
    //             arm.setPosition(140);
    //         }
    //         delay(15);
    //     }
    

};

void blueAuton()
{
    while (1)
    {
        mogoClamp.extend();
        intake.setAutonControlFlag(true);
        delay(15);
    }
};