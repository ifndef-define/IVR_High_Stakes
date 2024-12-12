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
ASSET(comp_white_5_txt);
ASSET(comp_white_4_txt);
ASSET(comp_white_6_txt);
void redAuton(){
    intakeLift.extend();
    mogoRushReach.extend();
    mogoRushClamp.extend(); //reversed
    chassis.follow(comp_white_1_txt, 15, 1500, 1, 0);

    mogoRushClamp.retract();
    chassis.moveToPose(-42, 60, 90, 1600, {.forwards = false,
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

    intakeLift.retract();
    delay(400);
    // moveBwd(5, 800, 127, 0);
    // while(chassis.isInMotion()){
    //     delay(15);
    // }
    // intake.setAutonControlFlag(false);
    chassis.swingToHeading(300, lemlib::DriveSide::RIGHT, 800, {}, 0);
    intake.setAutonControlFlag(true);
    // chassis.moveToPose(-46, 48, 270, 3000, {.maxSpeed = 127/4}, 0);
    // chassis.moveToPose(-69, 69, 315, 1500, {}, 0);
    chassis.follow(comp_white_4_txt, 15, 3000, 1, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().x <= -56){
            chassis.cancelMotion();
        }
        delay(15);
    }
    chassis.moveToPose(-67, 67, 315, 1000, {.maxSpeed = 127/2}, 1);
    chassis.turnToHeading(315, 700, {}, 1);
    // chassis.turnToPoint(-72, 72, 700, {}, 1);
    
    float dist = 3;
    int n = 3;
    int timeout = 750;
    lemlib::Pose start2 = chassis.getPose();
    for(int i = 0; i < n; i++) {
        if (i==2) {
            start2.x += 2;
            start2.y += 2;     
        }
            chassis.moveToPose(start2.x+(dist*1.414), start2.y-(dist*1.414), 
                315, timeout, {.forwards=false, .maxSpeed=20},0);
    
        chassis.moveToPose(start2.x-1, start2.y+1, 
            315, timeout, {.maxSpeed=40}, false);
            delay(300);

        if (i==2) {
            chassis.moveToPose(start2.x+16, start2.y-16, 
                315, timeout, {.forwards = false}, false);
        }
    }

    delay(100);
    
    chassis.turnToHeading(90, 1200, {}, 1);
    // chassis.follow(comp_white_5_txt, 15, 3000, 1, 1);
    chassis.moveToPose(-5, 60, 90, 1500, {.forwards = 1,
                                            .maxSpeed = 127/2}, 1);
    while(chassis.isInMotion()){
        if(chassis.getPose().x >= -25){
            intakeLift.extend();
        }
        delay(15);
    }
    delay(500);
    intakeLift.retract();
    delay(500);
    // chassis.moveToPose(-5, 59, 90, 1000, {}, 1);
    chassis.swingToPoint(0, 70, lemlib::DriveSide::RIGHT, 700, 
        {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed=127/2}, 0);

    chassis.moveToPose(chassis.getPose().x-(2*1.414), chassis.getPose().y-(2*1.414), 
                chassis.getPose().theta, timeout, {.forwards=false, .maxSpeed=16},0);
    
    static bool inArm = false;
    static int curTime = pros::millis();

    static vector<int> blueRange = {115, 270};
	static vector<int> redRange = {300, 30};
    do {
        if(intakeColor.get_proximity()<200){
            if(intakeColor.get_hue() >= redRange[0] && intakeColor.get_hue() <= redRange[1]){
                delay(60);
                intake.setAutonControlFlag(false);
                intakeMotor.move_relative(100, 127);
                delay(300);       
                inArm = true;
            } else {
                intake.setAutonControlFlag(true);
            }
        } else {
            arm.setPosition(16);
        }
        delay(15);
    } while((pros::millis()-curTime) < 4000);

    static int curTime1 = pros::millis();
    while(inArm && (arm.getNormalizedAngle()-160) > 10 || (pros::millis()-curTime1) < 4000){
        arm.setPosition(160);
        delay(15);
    }

    chassis.follow(comp_white_6_txt, 15, 10000, 0, 0);
    while(chassis.isInMotion()){
        arm.setPosition(0);
        delay(15);
    }


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